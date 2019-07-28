#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "display/screenBuffer.hpp"
#include "utilities.hpp"

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

  if((CanUtility_Init() != HAL_OK) || (CanUtility_EnableRecieve() != HAL_OK))
  {
    while(1){}
  }
  
  screenBufferInit();

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");
  
  Serial.println("µC initialized");
}


void loop() {
  //Serial.println(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, 0));
  while(Canmsg_bufferCanRecPointer > 0)
  {
    processCanMessage();
  }
  makeBufferVisible();
  printScreenBufferUserViewSerial();
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



