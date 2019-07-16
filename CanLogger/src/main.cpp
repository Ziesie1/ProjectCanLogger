#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

using namespace utilities; // für scom


void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten
  
  init_SD();

  initEncoder();
  initTaster();
  createNewCanLogFile();


  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(2);
  display.print(getFullLogFilePath());

  scom << "CanLogger ist Initialisiert" << endz;
}

void loop() {
  loopTaster();
  
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    /*
      Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
    */
    scom << "Zeichen empfangen:" << inChar << endz;

    Canmsg msg{};
    scom << static_cast<String>(msg) << endz;
    saveNewCanMessage(msg);
  }
  
}



