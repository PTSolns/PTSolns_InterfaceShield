// EXAMPLE: LCD Basics
// Last update: Oct 26, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates how one might make a menu to access and show certain data.
// Please excuse the simplicity and limitations in this example... it is not perfect, but shows the concepts.
// Enter             -> B1
// Move up in menu   -> B2
// Move down in menu -> B3
// Enter menu        -> B4
//
// HARDWARE
// Use Uno R3, Leonardo, Mega, or any other similar uC operating on 5V logic level and has same pinout.
// Onboard selection pins of the Interface-Shield:
// - "PWR" ........ connect -> powers ON the entire shield
// - "LCD D9" not connected -> can dim the LCD backlight via digital pin D9. See example "LCD Backlight Dimming" for this feature.
// - "LCD Pot" .... connect -> uses the onboard potentiometer to adjust the LCD backlight brightness. Turn the pot to adjust brightness as desired.
// - "D2" ......... connect -> used to send interrupt to D2 on uC. See example "Interrupt" for this feature.

#include "PTSolns_InterfaceShield.h"

Interface interface;

int menu_flag = 0;
int menu_selection = 0;
int index = 0;
int indexOld = 0;
String menu[4] = {"No data ...", "Temp. = XXX", "Time = YYY", "Relay = ZZZ"}; // Fake data just for demonstration.
String print_text;
const byte interruptPin = 2;
unsigned long select_timer = 0;
unsigned long select_duration = 1000;

byte arrow[] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B11111,
  B01000,
  B00100,
  B00000
};

void setup(){
  interface.begin(); // Use the default address by leaving argument blank (0x27). See "IO Expander Address" example for details.
  interface.interrupt(HIGH);

  menu_flag = 0;
  menu_selection = 0;
  index = 0;

  interface.createChar(0, arrow);

  attachInterrupt(digitalPinToInterrupt(interruptPin), func, FALLING); // See "Interrupt" example for detals.
}

void loop(){
  // Normal program operation... Here you can do the rest of the sketch, such as getting the data you want to display.
  if (menu_flag == 0){
    interface.setCursor(0,0);
    interface.print("Normal operation");

    if (menu_selection == 0){
      print_text = menu[0];
    } else if (menu_selection == 1){
      // Here get the data you want to display. As an example, we are showing temperature.
      print_text = menu[1];
    } else if (menu_selection == 2){
      // Here get the data you want to display. As an example, we are showing time.
      print_text = menu[2];
    } else if (menu_selection == 3){
      // Here get the data you want to display. As an example, we are relay status.
      print_text = menu[3];
    }
    interface.setCursor(0,1);
    interface.print(print_text);
  }

  // If Select (B4) is pressed, and then held down.
  if (menu_flag == 1){
    if ((millis() - select_timer) > select_duration){
      if (interface.digitalRead(B4) == HIGH){
        menu_flag = 2;
        interface.clear();
      } else {
        menu_flag = 0;
      }
    }
  }

  if (menu_flag == 2){
    if (index != indexOld){ // Only clear LCD when index changed.
      interface.setCursor(0,0);
      interface.print("                ");
      interface.setCursor(0,1);
      interface.print("                ");
    }
    interface.setCursor(0,0);
    interface.print(menu[(index)%4]);
    interface.setCursor(15,0);
    interface.write(byte(0));
    interface.setCursor(0,1);
    interface.print(menu[(index + 1)%4]);


    indexOld = index;
    
    if (interface.digitalRead(B2) == HIGH){ // Move menu up
      index = (index + 1)%4;
    } else if (interface.digitalRead(B3) == HIGH){ // Move menu down
      index = (index - 1 + 4)%4;
    } else if (interface.digitalRead(B1) == HIGH){ // Exit back to Main if Enter is pressed
      menu_flag = 0;
      interface.noBlink();
      menu_selection = index;
    }
  }
  delay(150);
}

void func(){
  // Only enter select while in Normal Operation. If already in Select mode, pressing down B4 again does not trigger this loop again.
  if (menu_flag == 0){
    menu_flag = 1; // Select has been pressed. 
    select_timer = millis(); // Start a timer.
  }
}
