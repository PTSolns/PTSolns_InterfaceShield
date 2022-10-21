// EXAMPLE: Debouncing
// Last update: Oct 22, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example demonstrates how the debouncing function can be implemented to ignore false triggers caused by switch bounce.
// At first try this sketch with debounce turned OFF (in setup loop).
// Press the B1 several times and eventually you will see that a single press caused two or even three recordings.
// This means the switched bounced in such a way that the sketch considered it another press. The debouncing function can remove this.
// Now turn the debouncing function ON (in setup loop). Reupload the sketch and start pressing the button again.
// Observe that multiple triggers on a single press happen less now. The debouncing function is working.
// The tricky part is setting the debouncing duration in which to ignore successive false triggers. 
// The default is 100ms, and this seems to work well. Make this time too large and some button presses might start to get missed.
// Make this time too small, and false triggers may start to happen again.
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

int test_button = B1;  // Change this to any of {B1, B2, B3, B4} to test different buttons.
int debounce_time = 100; // Default = 100ms
int buttonState;
int buttonState_previous;
unsigned long current_time;

void setup(){
  interface.begin(); // Use the default address by leaving argument blank (0x27). See "IO Expander Address" example for details.
  interface.debounce(ON, debounce_time); // Turn ON or OFF the debounce function. If devounce it not called, it is OFF by default.

  Serial.begin(9600); // Open Serial Monitor to see results.
}

void loop(){
  // Read button state
  buttonState = interface.digitalRead(test_button);
  
  if (buttonState > buttonState_previous){ // This only detects FALLING
    current_time = millis();
    // Print to Serial Monitor
    Serial.print("Button @ t=");
    Serial.println(current_time);
    // Print to LCD
    interface.clear();
    interface.print(current_time);
  }

  buttonState_previous = buttonState;
}
