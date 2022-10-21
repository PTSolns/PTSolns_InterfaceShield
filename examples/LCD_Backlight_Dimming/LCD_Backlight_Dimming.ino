// EXAMPLE: LCD Backlight Dimming
// Last update: Oct 21, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates how to dim the backlight of the LCD on the Interface-Shield using the digital pin D9 on the uC.
//
// HARDWARE CONFIGURATION
// Use Uno R3, Leonardo, Mega, or any other similar uC operating on 5V logic level and has same pinout.
// Onboard selection pins of the Interface-Shield:
// - "PWR" ........ connect -> powers ON the entire shield
// - "LCD D9" ..... connect -> can dim the LCD backlight via digital pin D9. 
// - "LCD Pot" not unconnec -> uses the onboard potentiometer to adjust the LCD backlight brightness. Turn the pot to adjust brightness as desired.
// - "D2" ... not connected -> used to send interrupt to D2 on uC. See example "Interrupt" for this feature.

#include "PTSolns_InterfaceShield.h"

Interface interface;

int D9 = 9; // The Interface-Shield hardware selection dictates this to be pin D9.
int dimLevel = 0;
int dimChange = 5;
   
void setup(){
  interface.begin();  // Use the default address by leaving argument blank (0x27). See "IO Expander Address" example for details.
  interface.print("Hello World!");

  pinMode(D9, OUTPUT);
}

void loop(){
  analogWrite(D9, dimLevel);
  
  dimLevel = dimLevel + dimChange;

  if (dimLevel <= 0 || dimLevel >= 255){
    dimChange = -dimChange;
  }

  delay(50);
}
