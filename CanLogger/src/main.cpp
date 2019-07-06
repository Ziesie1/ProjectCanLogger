#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "can.h"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  display.begin();
  display.fillScreen(ILI9341_BLACK);

  display.setTextSize(3);
  display.print("Test eines Textes...");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");
  delay(3000);

  if(Serial.available() > 0){
    char x=Serial.read();
    Serial.print("char recieved: ");
    Serial.println(x);
    Canmsg msg{};
    msg.PrintSerial();
    //Serial.println(static_cast<String>(msg)); //soll später PrintSerial ersetzen
  }
}
