// EXAMPLE: Interrupt
// Last update: Oct 20, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// The Interface-Shield has an output interrupt that can be connected to inerrupt pin D2 on the uC.
// This example shows how to use this interrupt feature.
// Note that the interrupt pin is normally HIGH and changes to LOW when one of the button states changes.
//
// HARDWARE CONFIGURATION
// Use Uno R3, Leonardo, Mega, or any other similar uC operating on 5V logic level and has same pinout.
// Onboard selection pins of the Interface-Shield:
// - "PWR" connect -> this powers ON the entire shield
// - "LCD D9" leave unconnected -> this can dim the LCD backlight via digital pin D9. See example "LCD Backlight Dimming" for this feature.
// - "LCD Pot" connect -> this uses the onboard potentiometer to adjust the LCD backlight brightness. Turn the pot to adjust brightness as desired.
// - "D2" connected -> this is used to send interrupt to D2 on uC. 

#include "PTSolns_InterfaceShield.h"

Interface interface;

const byte interruptPin = 2; // This pin is fixed in the hardware of the Interface-Shield.

void setup(){
  interface.begin();         // Use the default address by leaving argument blank (0x27)
  interface.backlight(ON);   // Turn ON LCD backlight.
  interface.print("Testing Interrupt");

  interface.debounce(ON,100); // Turn ON debouncing and set debouncing time to 100ms. See example "Debouncing" for more details on this.
  
  interface.interrupt(HIGH); // This pulls up the internal uC resistor for pin D2. Same as pinMode(interruptPin, INPUT_PULLUP).
  attachInterrupt(digitalPinToInterrupt(interruptPin), func, FALLING); // We are detecting a FALLING state. 
  Serial.begin(9600);
}

void loop(){
  // Cycling through the LEDs On and Off
  interface.digitalWrite(LED1, HIGH);
  delay(200);
  interface.digitalWrite(LED2, HIGH);
  delay(200);
  interface.digitalWrite(LED3, HIGH);
  delay(200);
  interface.digitalWrite(LED4, HIGH);
  delay(200);
  interface.digitalWrite(LED4, LOW);
  delay(200);
  interface.digitalWrite(LED3, LOW);
  delay(200);
  interface.digitalWrite(LED2, LOW);
  delay(200);
  interface.digitalWrite(LED1, LOW);
  delay(200);
}

void func(){
  Serial.println("Interrupt!");
  // NOTE: A limitation to this interrupt routine.
  // No function calls should be made to any library commands as this may freeze the code.
  // Instead keep this interrupt routine as simple and short as possible.
  // You can set flags, invert volatile state variables, etc.
}
