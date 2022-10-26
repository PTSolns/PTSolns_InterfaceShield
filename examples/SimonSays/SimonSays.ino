// EXAMPLE: Simon Says
// Last update: Oct 26, 2022
// contact@PTSolns.com
//
// DESCRIPTION
// Do what Simons says! ... Upload this sketch and play
// B1 == Top Left
// B2 == Bottom Left
// B3 == Bottom Right
// B4 == Top Right
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

long randomNum;
unsigned long timer;
unsigned long duration = 1000; // The initial time [ms] the player has to press the correct button.
int flag = 0;
int buttonState;
int RoundMax = 7; // Set the maximum number of rounds per level
int Round = 1;
int level = 1;

byte customSquare[] = {
  B00000,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000
};

byte soSad[] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B01110,
  B10001,
  B10001,
  B00000
};

byte soHappy[] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B10001,
  B01110,
  B00000
};
    
void setup(){
  interface.begin();  // Use the default address by leaving argument blank (0x27). See "IO Expander Address" example for details.

  interface.createChar(0,customSquare);
  interface.createChar(1,soSad);
  interface.createChar(2,soHappy);

  randomSeed(analogRead(0));

  interface.setCursor(3,0);
  interface.print("SIMON SAYS");
  delay(1500);
  interface.clear();
  interface.setCursor(4,1);
  interface.print("Ready...");
  interface.digitalWrite(LED1, HIGH);
  interface.digitalWrite(LED2, HIGH);
  interface.digitalWrite(LED3, HIGH);
  interface.digitalWrite(LED4, HIGH);
  delay(500);
  interface.digitalWrite(LED1, LOW);
  interface.digitalWrite(LED2, LOW);
  interface.digitalWrite(LED3, LOW);
  interface.digitalWrite(LED4, LOW);
  delay(1000);
  interface.clear();
  interface.setCursor(6,1);
  interface.print("Set...");
  interface.digitalWrite(LED1, HIGH);
  interface.digitalWrite(LED2, HIGH);
  interface.digitalWrite(LED3, HIGH);
  interface.digitalWrite(LED4, HIGH);
  delay(500);
  interface.digitalWrite(LED1, LOW);
  interface.digitalWrite(LED2, LOW);
  interface.digitalWrite(LED3, LOW);
  interface.digitalWrite(LED4, LOW);
  delay(1000);
  interface.clear();
  interface.setCursor(8,1);
  interface.print("GO");
  interface.digitalWrite(LED1, HIGH);
  interface.digitalWrite(LED2, HIGH);
  interface.digitalWrite(LED3, HIGH);
  interface.digitalWrite(LED4, HIGH);
  delay(500);
  interface.digitalWrite(LED1, LOW);
  interface.digitalWrite(LED2, LOW);
  interface.digitalWrite(LED3, LOW);
  interface.digitalWrite(LED4, LOW);
  interface.clear();
  delay(500);
}

void loop(){

  if (Round == RoundMax){
    Round = 1;                 // Reset Round
    level = level + 1;         // Advance to next level
    duration = duration - 200; // Reduce duration timer ... Impossible to go past level 5!
    interface.clear();
    interface.setCursor(3,0);
    interface.print("Next Level");
    delay(1500);
    interface.clear();
    interface.setCursor(7,0);
    interface.write(byte(2));  // A small reward
    delay(1000);
    interface.setCursor(3,1);
    interface.print("Faster NOW");
    
    delay(2000);
    interface.clear();
  }

  interface.setCursor(4,0);
  interface.print("Round ");
  interface.setCursor(10,0);
  interface.print(Round);
  interface.setCursor(4,1);
  interface.print("Level ");
  interface.setCursor(10,1);
  interface.print(level);

  randomNum = random(1,5); // Generate pseudo random number between 1 and 4
  timer = millis();        // Start timer
  flag = 0;                // Status flag

  // Top left
  if (randomNum == 1){        
    interface.setCursor(0,0);
    interface.write(byte(0));

    while (((millis() - timer) < duration) ) {
      buttonState = interface.digitalRead(B1);
      if (buttonState == HIGH){
        flag = 1;
        interface.digitalWrite(LED1, HIGH);
        delay(50);
        interface.digitalWrite(LED1, LOW);
        interface.setCursor(0,0);
        interface.print(" ");
      }
    }

  // Bottom left
  } else if (randomNum == 2){ 
    interface.setCursor(0,2);
    interface.write(byte(0));

    while (((millis() - timer) < duration) ) {
      buttonState = interface.digitalRead(B2);
      if (buttonState == HIGH){
        flag = 1;
        interface.digitalWrite(LED2, HIGH);
        delay(50);
        interface.digitalWrite(LED2, LOW);
        interface.setCursor(0,2);
        interface.print(" ");
      }
    }

  // Bottom right
  } else if (randomNum == 3){ 
    interface.setCursor(15,2);
    interface.write(byte(0));

    while (((millis() - timer) < duration) ) {
      buttonState = interface.digitalRead(B3);
      if (buttonState == HIGH){
        flag = 1;
        interface.digitalWrite(LED3, HIGH);
        delay(50);
        interface.digitalWrite(LED3, LOW);
        interface.setCursor(15,2);
        interface.print(" ");
      }
    }

  // Top right
  } else if (randomNum == 4){ 
    interface.setCursor(15,0);
    interface.write(byte(0));

    while (((millis() - timer) < duration) ) {
      buttonState = interface.digitalRead(B4);
      if (buttonState == HIGH){
        flag = 1;
        interface.digitalWrite(LED4, HIGH);
        delay(50);
        interface.digitalWrite(LED4, LOW);
        interface.setCursor(15,0);
        interface.print(" ");
      }
    }
    
  }

  // YOU LOST loop
  if (flag == 0) { 
    interface.clear();
    interface.setCursor(0,0);
    interface.write(byte(1));
    interface.setCursor(0,1);
    interface.write(byte(1));
    interface.setCursor(15,1);
    interface.write(byte(1));
    interface.setCursor(15,0);
    interface.write(byte(1));
    while (1){               // Stuck in infinity loop until user presses RST
      interface.setCursor(3,0);
      interface.print("YOU LOST!");
      interface.digitalWrite(LED1, HIGH);
      interface.digitalWrite(LED2, HIGH);
      interface.digitalWrite(LED3, HIGH);
      interface.digitalWrite(LED4, HIGH);
      interface.setCursor(1,1);
      interface.print("  Press RST   ");
      delay(500);
      interface.setCursor(3,0);
      interface.print("          ");
      interface.digitalWrite(LED1, LOW);
      interface.digitalWrite(LED2, LOW);
      interface.digitalWrite(LED3, LOW);
      interface.digitalWrite(LED4, LOW);
      delay(500);

      interface.setCursor(3,0);
      interface.print("YOU LOST!");
      interface.digitalWrite(LED1, HIGH);
      interface.digitalWrite(LED2, HIGH);
      interface.digitalWrite(LED3, HIGH);
      interface.digitalWrite(LED4, HIGH);
      interface.setCursor(1,1);
      interface.print("              ");
      interface.setCursor(3,1);
      interface.print("L ");
      interface.setCursor(5,1);
      interface.print(level);
      interface.setCursor(9,1);
      interface.print("R ");
      interface.setCursor(11,1);
      interface.print(Round);
      delay(500);
      interface.setCursor(3,0);
      interface.print("          ");
      interface.digitalWrite(LED1, LOW);
      interface.digitalWrite(LED2, LOW);
      interface.digitalWrite(LED3, LOW);
      interface.digitalWrite(LED4, LOW);
      delay(500);
    }
  } else {
    Round = Round + 1; // increment Round counter
  }
  
  delay(400);
}
