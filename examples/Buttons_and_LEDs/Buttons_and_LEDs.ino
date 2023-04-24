// EXAMPLE: Buttons and LEDs
// Last update: Oct 21, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// The Interface-Shield has four programmable LEDs and four programmable push buttons.
// This example demonstrates how to control these LEDs and buttons.
// LED LED1 is OFF while button B1 is not pressed. LED1 turns ON when B1 pressed. LCD prints state accordingly. Similarly for the other three LEDs & buttons.
// Note that the names for the four LEDs and the four buttons are fixed. They are as follows:
// LED1, LED2, LED3, LED4, B1, B2, B3, B4.
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
  // LED1 and Button B1
  int buttonStateB1 = interface.digitalRead(B1);
  interface.setCursor(0, 0);
  interface.print(buttonStateB1 ? "LED1 1" : "LED1 0");
  if (buttonStateB1 == HIGH){
    interface.digitalWrite(LED1, HIGH);
  } else{
    interface.digitalWrite(LED1, LOW);
  }

  // LED2 and Button B2
  int buttonStateB2 = interface.digitalRead(B2);
  interface.setCursor(8, 0);
  interface.print(buttonStateB2 ? "LED2 1" : "LED2 0");
  if (buttonStateB2 == HIGH){
    interface.digitalWrite(LED2, HIGH);
  } else{
    interface.digitalWrite(LED2, LOW);
  }

  // LED3 and Button B3
  int buttonStateB3 = interface.digitalRead(B3);
  interface.setCursor(0, 1);
  interface.print(buttonStateB3 ? "LED3 1" : "LED3 0");
  if (buttonStateB3 == HIGH){
    interface.digitalWrite(LED3, HIGH);
  } else{
    interface.digitalWrite(LED3, LOW);
  }

  // LED4 and Button B4
  int buttonStateB4 = interface.digitalRead(B4);
  interface.setCursor(8, 1);
  interface.print(buttonStateB4 ? "LED4 1" : "LED4 0");
  if (buttonStateB4 == HIGH){
    interface.digitalWrite(LED4, HIGH);
  } else{
    interface.digitalWrite(LED4, LOW);
  }

  delay(50);
}
