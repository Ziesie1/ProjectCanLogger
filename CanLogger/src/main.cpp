#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

using namespace utilities; // für scom

void setup() {
  HAL_Init();
  //HAL_InitTick(14);
  SystemClock_Config();
  Serial.begin(115200);
  //scom.workWith(Serial); // scom Hardwareserial zuweisen
  //scom.showDebugMessages(true); // Debugmodus einschalten
  
  //init_SD();
  CanUtility_Init();

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");

  //scom << "CanLogger is initialised" << endz;
  Serial.println("µC initialisiert");
}


void loop() {
  //Serial.println(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, 0));
  while(Canmsg_bufferCanRecPointer > 0)
  {
    Canmsg_bufferCanRecPointer--;
    Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer].Send();
    Serial.print("Empfangene Nachricht: ");
    Serial.println(static_cast<String>(Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer]));
  }
  delay(5000);
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    /*
      Eine Ausgabe nicht beim Interrupt erlaubt, hier nur Testweise. // Bis das gesamt Konzept feststeht.
    */
    scom << "Charakter recieved:" << inChar << endz;

    Canmsg msg{};
    scom << static_cast<String>(msg) << endz;
  }
  
}



