// Special thanks to our "You rock, Hall of Fame" Kickstarter supporters:
// Guy Pilens
// Megan
// David G. Simpson

#include "PTSolns_InterfaceShield.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

void i2c_recover_bus() {
    pinMode(SDA, INPUT_PULLUP);
    pinMode(SCL, INPUT_PULLUP);
    delayMicroseconds(5);

    if (digitalRead(SDA) == LOW) {
        pinMode(SCL, OUTPUT);

        for (int i = 0; i < 9; i++) {
            digitalWrite(SCL, HIGH);
            delayMicroseconds(5);
            digitalWrite(SCL, LOW);
            delayMicroseconds(5);

            if (digitalRead(SDA) == HIGH) {
                break;
            }
        }

        pinMode(SDA, OUTPUT);
        digitalWrite(SDA, HIGH);
        digitalWrite(SCL, HIGH);
        delayMicroseconds(5);
    }

    pinMode(SDA, INPUT_PULLUP);
    pinMode(SCL, INPUT_PULLUP);
}

Interface::Interface()
{
}

void Interface::init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
    _rs_pin = rs;
    _rw_pin = rw;
    _enable_pin = enable;

    _data_pins[0] = d0;
    _data_pins[1] = d1;
    _data_pins[2] = d2;
    _data_pins[3] = d3;
    _data_pins[4] = d4;
    _data_pins[5] = d5;
    _data_pins[6] = d6;
    _data_pins[7] = d7;

    if (fourbitmode)
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else
        _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

    beginLCD(16, 1);
}

void Interface::beginLCD(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    this->ExpanderPinMode(_rs_pin, OUTPUT);
    if (_rw_pin != 255) {
        this->ExpanderPinMode(_rw_pin, OUTPUT);
    }
    this->ExpanderPinMode(_enable_pin, OUTPUT);

    for (int i = 0; i < ((_displayfunction & LCD_8BITMODE) ? 8 : 4); ++i)
    {
        this->ExpanderPinMode(_data_pins[i], OUTPUT);
    }

    delayMicroseconds(50000);
    this->PinWrite(_rs_pin, LOW);
    this->PinWrite(_enable_pin, LOW);
    if (_rw_pin != 255) {
        this->PinWrite(_rw_pin, LOW);
    }

    if (!(_displayfunction & LCD_8BITMODE)) {
        write4bits(0x03);
        delayMicroseconds(4500);
        write4bits(0x03);
        delayMicroseconds(4500);
        write4bits(0x03);
        delayMicroseconds(150);
        write4bits(0x02);
    }
    else {
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(4500);
        command(LCD_FUNCTIONSET | _displayfunction);
        delayMicroseconds(150);
        command(LCD_FUNCTIONSET | _displayfunction);
    }

    command(LCD_FUNCTIONSET | _displayfunction);

    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    clear();

    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    command(LCD_ENTRYMODESET | _displaymode);

}

void Interface::setRowOffsets(int row0, int row1, int row2, int row3)
{
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

void Interface::clear()
{
    command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);
}

void Interface::home()
{
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);
}

void Interface::setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
    if (row >= max_lines) {
        row = max_lines - 1;
    }
    if (row >= _numlines) {
        row = _numlines - 1;
    }

    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

void Interface::noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Interface::display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Interface::noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Interface::cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Interface::noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void Interface::blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void Interface::scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void Interface::scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void Interface::leftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void Interface::rightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void Interface::autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void Interface::noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

void Interface::createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7;
    command(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

inline void Interface::command(uint8_t value) {
    send(value, LOW);
}

inline size_t Interface::write(uint8_t value) {
    send(value, HIGH);
    return 1;
}

void Interface::send(uint8_t value, uint8_t mode) {
    this->PinWrite(_rs_pin, mode);

    if (_rw_pin != 255) {
        this->PinWrite(_rw_pin, LOW);
    }

    if (_displayfunction & LCD_8BITMODE) {
        write8bits(value);
    }
    else {
        write4bits(value >> 4);
        write4bits(value);
    }
}

void Interface::pulseEnable(void) {
    this->PinWrite(_enable_pin, LOW);
    delayMicroseconds(1);
    this->PinWrite(_enable_pin, HIGH);
    delayMicroseconds(1);
    this->PinWrite(_enable_pin, LOW);
    delayMicroseconds(100);
}

void Interface::write4bits(uint8_t value) {
    for (int i = 0; i < 4; i++) {
        this->PinWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

void Interface::write8bits(uint8_t value) {
    for (int i = 0; i < 8; i++) {
        this->PinWrite(_data_pins[i], (value >> i) & 0x01);
    }

    pulseEnable();
}

uint8_t Interface::begin()
{
    return InitialSetup();
}

uint8_t Interface::begin(uint8_t addr)
{
    if (0x20 <= addr && 0x27 >= addr)
        this->addr = addr;
    else
        return 2;

    return InitialSetup();
}

void Interface::setClock(unsigned long speed)
{
    Wire.setClock(speed);
}

uint8_t Interface::backlight(bool state)
{
    int err = 0;
    err += ExpanderPinMode(3, OUTPUT);
    err += PinWrite(3, state);
    return (err > 0) * 10;
}

void Interface::interrupt(bool state)
{
    pinMode(2, state ? INPUT_PULLUP : INPUT);
}

uint8_t Interface::digitalWrite(uint8_t pin, bool value)
{
    if (pin > 3) return 40;
    uint8_t pin1 = pin + 8;
    if (value == LOW)
    {
        this->output.w |= (1 << pin1);
    }
    else
    {
        this->output.w &= ~(1 << pin1);
    }
    return write_impl() * 40;
}

bool Interface::digitalRead(uint8_t pin)
{
    if (pin > 3) return 0;
    uint8_t pin1 = pin + 8 + 4;
    uint16_t v = read();
    bool readState = !((v & (1 << pin1)) ? HIGH : LOW);

    if (!debounceEnabled) return readState;

    unsigned int timeNow = millis();
    if ((timeNow - btnTimes[pin]) > debounceTime)
    {
        btnTimes[pin] = timeNow;
        btnStates[pin] = readState;
    }
    return btnStates[pin];
}

void Interface::debounce(bool state, int ms)
{
    debounceEnabled = state;
    debounceTime = ms;
}

void Interface::debounce(bool state)
{
    debounceEnabled = state;
}

uint8_t Interface::InitialSetup()
{
    i2c_recover_bus();

    Wire.begin();
    int err = 0;
    err += ExpanderPinMode(8, OUTPUT);
    err += ExpanderPinMode(9, OUTPUT);
    err += ExpanderPinMode(10, OUTPUT);
    err += ExpanderPinMode(11, OUTPUT);

    err += ExpanderPinMode(12, INPUT);
    err += ExpanderPinMode(13, INPUT);
    err += ExpanderPinMode(14, INPUT);
    err += ExpanderPinMode(15, INPUT);

    setClock(100000L);
    init(1, 0, 1, 2, 4, 5, 6, 7, 0, 0, 0, 0);
    beginLCD(16, 2);
    return (err > 0) * 1;
}

bool Interface::ExpanderPinMode(uint8_t pin, uint8_t mode)
{
    if (mode == INPUT)
    {
        this->dir.w |= (1 << pin);
    }
    else
    {
        this->dir.w &= ~(1 << pin);
    }
    return direction_impl();
}

uint8_t Interface::PinWrite(uint8_t pin, bool value)
{
    if (value == HIGH)
    {
        this->output.w |= (1 << pin);
    }
    else
    {
        this->output.w &= ~(1 << pin);
    }
    return write_impl() * 40;
}

uint16_t Interface::read() {
    read_bytes(this->addr, Reg::INPUT_PORT_0, this->input.b, 2);
    return this->input.w;
}

bool Interface::write(const uint16_t value)
{
    this->output.w = value;
    return write_impl();
}

bool Interface::write(const Port::Port port, const Level::Level level)
{
    if (level == Level::H)
    {
        this->output.w |= (1 << port);
    }
    else
    {
        this->output.w &= ~(1 << port);
    }
    return write_impl();
}

bool Interface::polarity(const uint16_t value)
{
    this->pol.w = value;
    return polarity_impl();
}

bool Interface::polarity(const Port::Port port, const Polarity::Polarity pol)
{
    if (pol == Polarity::INVERTED)
    {
        this->pol.w |= (1 << port);
    }
    else
    {
        this->pol.w &= ~(1 << port);
    }
    return polarity_impl();
}

bool Interface::direction(const uint16_t value)
{
    this->dir.w = value;
    return direction_impl();
}

bool Interface::direction(const Port::Port port, const Direction::Direction dir)
{
    if (dir == Direction::IN)
    {
        this->dir.w |= (1 << port);
    }
    else
    {
        this->dir.w &= ~(1 << port);
    }
    return direction_impl();
}

bool Interface::write_impl()
{
    return write_bytes(this->addr, Reg::OUTPUT_PORT_0, this->output.b, 2);
}

bool Interface::polarity_impl()
{
    return write_bytes(this->addr, Reg::POLARITY_INVERSION_PORT_0, this->pol.b, 2);
}

bool Interface::direction_impl()
{
    return write_bytes(this->addr, Reg::CONFIGURATION_PORT_0, this->dir.b, 2);
}

int8_t Interface::read_bytes(const uint8_t dev, const uint8_t reg, uint8_t* data, const uint8_t size)
{
    Wire.beginTransmission(dev);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(dev, size);
    int8_t count = 0;
    while (Wire.available()) data[count++] = Wire.read();
    return count;
}

bool Interface::write_bytes(const uint8_t dev, const uint8_t reg, const uint8_t* data, const uint8_t size)
{
    Wire.beginTransmission(dev);
    Wire.write(reg);
    for (uint8_t i = 0; i < size; i++) Wire.write(data[i]);
    status = Wire.endTransmission();
    return (status == 0);
}
