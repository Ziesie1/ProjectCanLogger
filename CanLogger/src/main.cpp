#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

using namespace utilities; // für scom





void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten
  
  init_SD();

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");

  

  scom << "CanLogger ist Initialisiert" << endz;
}

void loop() {
  
  
  
}



void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    /*
      Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
    */
    scom << "Zeichen empfangen:" << inChar << endz;

    Canmsg msg{};
    scom << msg << endz;
  }
  
}



