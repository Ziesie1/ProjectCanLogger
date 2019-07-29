#include <Arduino.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"
#include "display/ILI9341.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/HomePage.hpp"

using namespace utilities; // für scom

ILI9341 display {PC9, PC8, PA10, PA8, PB5, true};
DisplayPageManager pageManager {};

void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten
  
  init_SD();
  initEncoder();
  initTaster();
  display.init();
 
  pageManager.openNewPage(new HomePage{display}); // Startseite setzen

  createNewCanLogFile();
	
  scom << "CanLogger ist Initialisiert" << endz;
}

void loop() {
  loopTaster();
  pageManager.loop();
  
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



