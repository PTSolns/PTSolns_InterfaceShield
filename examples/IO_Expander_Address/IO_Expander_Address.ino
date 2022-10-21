// EXAMPLE: IO Expander Address
// Last update: Oct 19, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// This example shows how to set the address for the IO Expander onboard the Interface-Shield.
//
// HARDWARE CONFIGURATION
// Use Uno R3, Leonardo, Mega, or any other similar uC operating on 5V logic level and has same pinout.
// Onboard selection pins of the Interface-Shield:
// - "PWR" connect -> this powers ON the entire shield
// - "LCD D9" leave unconnected -> this can dim the LCD backlight via digital pin D9. See example "LCD Backlight Dimming" for this feature.
// - "LCD Pot" connect -> this uses the onboard potentiometer to adjust the LCD backlight brightness. Turn the pot to adjust brightness as desired.
// - "D2" leave unconnected -> this is used to send interrupt to D2 on uC. See example "Interrupt" for this feature.
// NOTE: For this example, the only important setting is "PWR".

#include "PTSolns_InterfaceShield.h"

Interface interface;

int error_code = -1;

void setup(){
  error_code = interface.begin(0x29);   // Use the default address 0x27.
  // NOTE: The PCA9535 I/O Expander onboard the Interface-Shield has eight available addresses. 
  // The default address is 0x27, which is when jumper bridges for A0, A1 and A2 on the back of the Interface-Shield are left open.

  // Address Legend:
  // A0=0, A1=0, A2=0 -> 0x27
  // A0=1, A1=0, A2=0 -> 0x26
  // A0=0, A1=1, A2=0 -> 0x25
  // A0=1, A1=1, A2=0 -> 0x24
  // A0=0, A1=0, A2=1 -> 0x23
  // A0=1, A1=0, A2=1 -> 0x22
  // A0=0, A1=1, A2=1 -> 0x21
  // A0=1, A1=1, A2=1 -> 0x20
  // Where 0=solder bridge open (pulled high), 1=solder bridge closed (pulled low).

  // Only this range of available addresses are allowed. 
  // With the Interface-Shield stacked and PWR ON run this script and check output on LCD.
  // If LCD reads "Address worked", then you've set the correct address and all is working as it should.
  // Otherwise, check on the back of the Interface-Shield if any of the three jumper bridges for A0, A1, A2 are closed. If so, change the address accordingly.
  // Consult the "Address Legend" on the back of the Interface-Shield to determine correct address. 
  // Otherwise run example "i2c_scanner" and check Serial Monitor for address output.

  // You can also open the Serial Monitor and see the output of the error_code.
  // error_code = 0 means intiialization of the I/O expander is successful.
  // error_code = 1 means wrong address specified.
  // error_code = 2 means address specified is outside range of avaiable addresses.
  
  Serial.begin(9600);
  Serial.print(error_code);
  interface.backlight(ON);
  interface.print("Address worked");
}

void loop(){
  // Nothing to do in the main loop for this example.
}
