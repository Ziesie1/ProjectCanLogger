#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SdFat.h>
#include "Canmsg.h"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

constexpr byte SD_CS = PB6;
SdFat sdKarte{};
#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))


void setup() {
  Serial.begin(115200);

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");

  // Erster Schreibvorgang auf der SD-Karte
  delay(1000);
  if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
    Serial.println("### Fehler beim Initialsieren der SD-Karte.");
    sdKarte.initErrorPrint();
  }

  if (!sdKarte.exists("/CPP4Live")) {
    if (!sdKarte.mkdir("/CPP4Live")) {
      Serial.println("### Fehler beim erstellen von Ordner CPP4Live.");
      sdKarte.errorPrint();
    }
  }


}

void loop() {

}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    /*
      Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
    */
    Serial.print("char recieved: ");
    Serial.println(inChar);
    Canmsg msg{};
    Serial.println(msg);
  }
}
