#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SdFat.h>
#include "Canmsg.h"
#include "serial/SerialCommunikation.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

constexpr byte SD_CS = PB6;
SdFat sdKarte{};
#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))

using namespace utilities;

void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");

  // Erster Schreibvorgang auf der SD-Karte
  delay(1000);
  if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
    scom.printError("Bei der Initialisierung der SD-Karte trat ein Fehler auf.\nIst die Karte eingesteckt?");
    //sdKarte.initErrorPrint();
  }

  if (!sdKarte.exists("/CPP4Live")) {
    if (!sdKarte.mkdir("/CPP4Live")) {
      scom.printError("Der Ordner /CPP4Live konnte nicht erstellt werden.");
      //sdKarte.errorPrint();
    }else{
      scom.printDebug("/CPP4Live wurde erstellt");
    }
  }else{
    scom.printDebug("/CPP4Live existiert bereits");
  }

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
