#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Canmsg.h"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial



void setup() {
  Serial.begin(115200);

  display.begin();
  display.fillScreen(ILI9341_BLACK);

  display.setTextSize(3);
  display.print("Test eines Textes...");
}

void loop() {

  if(Serial.available() > 0){
    char x=Serial.read();
    Serial.print("char recieved: ");
    Serial.println(x);
    Canmsg msg{};
    msg.PrintSerial();
    //Serial.println(static_cast<String>(msg)); //soll später PrintSerial ersetzen
  }
}
