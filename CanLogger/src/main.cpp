#include <Arduino.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"
#include "display/ILI9341.hpp"

using namespace utilities; // für scom

ILI9341 display {PC9, PC8, PA10, PA8, PB5, true};

void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten
  
  init_SD();
  initEncoder();
  initTaster();
  display.init();
  display.fillScreen(BLAU50);
  display.drawFillRect2(50,50+80-1,40,50-1,ILI9341::makeColor(100,0,0));
  display.drawFillRect(50,50,80,80,ILI9341::makeColor(0,100,0));
  display.drawOnePixel(230,150,GELB100);
  display.drawHorizontalLine(0,229,150,ROT100);
  display.drawOnePixel(20,300,GELB100);
  display.drawVerticalLine(20,0,299,ROT100);
  
  display.printChar8x16(50,200,'X',ROT100, BLAU50);
  display.printChar16x32(50+8+1,200,'X',ROT100, BLAU50);
  display.printChar16x24(50+8+16+1,200,'X',ROT100, BLAU50);
  display.printChar12x24(50+8+16+16+1,200,'X',ROT100, BLAU50);
  display.printString(50,200+32+1,"Hallo", ROT100, BLAU50, 3);

  uint16_t const test[100] = 
  {
    WEISS,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,WEISS,
    ROT100,WEISS,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,WEISS,ROT100,
    ROT100,ROT100,WEISS,ROT100,ROT100,ROT100,ROT100,WEISS,ROT100,ROT100,
    ROT100,ROT100,ROT100,WEISS,ROT100,ROT100,WEISS,ROT100,ROT100,ROT100,
    ROT100,ROT100,ROT100,ROT100,WEISS,WEISS,ROT100,ROT100,ROT100,ROT100,
    ROT100,ROT100,ROT100,ROT100,WEISS,WEISS,ROT100,ROT100,ROT100,ROT100,
    ROT100,ROT100,ROT100,WEISS,ROT100,ROT100,WEISS,ROT100,ROT100,ROT100,
    ROT100,ROT100,WEISS,ROT100,ROT100,ROT100,ROT100,WEISS,ROT100,ROT100,
    ROT100,WEISS,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,WEISS,ROT100,
    WEISS,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,ROT100,WEISS
  };
  display.drawBmp(150,50,10,10,test,4);

  //initTFT();
  createNewCanLogFile();
	
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



