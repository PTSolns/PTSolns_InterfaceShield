// EXAMPLE: LCD Basics
// Last update: Oct 21, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates basic commands to work with the Interface-Shield.
//
// HARDWARE CONFIGURATION
// Use Uno R3, Leonardo, Mega, or any other similar uC operating on 5V logic level and has same pinout.
// Onboard selection pins of the Interface-Shield:
// - "PWR" ........ connect -> powers ON the entire shield
// - "LCD D9" not connected -> can dim the LCD backlight via digital pin D9. See example "LCD Backlight Dimming" for this feature.
// - "LCD Pot" .... connect -> uses the onboard potentiometer to adjust the LCD backlight brightness. Turn the pot to adjust brightness as desired.
// - "D2" ... not connected -> used to send interrupt to D2 on uC. See example "Interrupt" for this feature.

#include "PTSolns_InterfaceShield.h"

Interface interface;
    
void setup(){
  interface.begin();  // Use the default address by leaving argument blank (0x27). See "IO Expander Address" example for details.
}

void loop(){
  // All the standard LCD commands apply.
  interface.setCursor(0,0);
  interface.print("Hello World!");
  delay(2000);
  
  interface.clear(); 
  interface.setCursor(0,1);
  interface.print("Hello Universe!");
  delay(2000);

  interface.clear();
  interface.backlight(OFF);
  delay(2000);

  interface.backlight(ON);
  interface.setCursor(0,0);
  interface.print("...");
  delay(2000);

  // All LCD commands include:
  // interface.setCurser(0,0); // NOTE: The Interface-Shield uses the 1602 LCD.
  // interface.print("Hello World");
  // interface.autoscroll();
  // interface.noAutoscroll();
  // interface.clear();
  // interface.noBlink();
  // interface.blink();
  // interface.noCursor();
  // interface.cursor();
  // interface.noDisplay();
  // interface.Display();
  // interface.scrollDisplayLeft();
  // interface.scrollDisplayRight();
}
