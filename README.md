# PTSolns Interface-Shield

Library for PTSolns Interface-Shield

The Interface-Shield allows the user to interact with their project visually and physically in an easy-to-use shield. The shield offers an LCD with four programmable push buttons, four programmable LEDs, an interrupt pin and various configuration modes. The following outlines the key features of the Interface-Shield:

- Interface with microcontroller via I2C bus, without using any other digital or analog pins.
- Onboard main power ON/OFF
- 1602 LCD with backlight
- Turn ON/OFF LCD backlight via software
- Two ways to dim LCD backlight:
  1.  PWM digital pin D9 on microcontroller
  2.  Onboard potentiometer
- Onboard LCD contrast potentiometer
- Four programmable LEDs
- Four programmable push buttons with RC debouncing filters (software debouncing also available)
- Footprints on the back of the PCB for optional SMD RC debouncing filter for each of the push buttons
- Output interrupt pin (if enabled, connected to pin D2 on microcontroller)
- Eight available addresses, with address legend printed on back of PCB

Various examples are included in this package. For any comments or questions please contact us at contact@PTSolns.com

## Limitations
We realize that nothing is perfect. While we try to improve on the following limitations, we feel it is responsible to inform the user of known limitations.
- Any interrupt function that is triggered by the interrupt pin (which is a signal output by the I/O expander to pin D2) must be kept as simple and clean as possible. The user should avoid making calls to functions of the Interface-Shield library within this interrupt function. Instead the user should use flags, volatile state changes, and similar within the interrupt function.
- Many successive rapid and aggressive push button presses may result in the microcontroller freezing. If this happens simply press the restart button on the microcontroller.

## Acknowledgements
The Interface-Shield library incorporates large parts of the following libraries. We wish to give credit to their respective authors.
- PCA95x5.h by Hideaki Tai, MIT License, https://github.com/hideakitai/PCA95x5
- LiquidCrystal.h by Hans-Christoph Steiner, GNU Lesser General Public License (LGPL), https://github.com/arduino-libraries/LiquidCrystal
