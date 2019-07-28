#include <Arduino.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"
#include "display/ILI9341.hpp"
#include "display/DisplayImages.hpp"

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
  display.fillScreen(BLUE100);
  
  display.drawFillRect2(50,50+80-1,40,50-1,ILI9341::makeColor(100,0,0));
  display.drawFillRect(50,50,80,80,ILI9341::makeColor(0,100,0));
  display.drawEmptyRect(50-2,40-2,80+4,80+10+4,ILI9341::makeColor(160,0,255),2);

  display.drawOnePixel(230,150,YELLOW100);
  display.drawHorizontalLine(0,229,150, RED100);
  display.drawOnePixel(20,300,YELLOW100);
  display.drawVerticalLine(20,0,299, RED100);
  
  display.printChar8x16(50,200,'X', RED100, BLUE100);
  display.printChar16x32(50+8+1,200,'X',RED100, BLUE100);
  display.printChar16x24(50+8+16+1,200,'X',RED100, BLUE100);
  display.printChar12x24(50+8+16+16+1,200,'X',RED100, BLUE100);
  display.printString(50,200+32+1,"Hallo", RED100, BLUE100, 3);

  delay(3000);
  display.rotateDisplay(false);
  delay(3000);
  display.rotateDisplay(true);


  display.drawEmptyRect2(50,200,154,190,BLACK,2);
  display.printString8x16(50+4,154+2+1,"Test", RED100, BLUE100);

  Color cross = WHITE;
  Color backround = RED100;
  uint16_t const test[100] = 
  {
    cross,backround,backround,backround,backround,backround,backround,backround,backround,cross,
    backround,cross,backround,backround,backround,backround,backround,backround,cross,backround,
    backround,backround,cross,backround,backround,backround,backround,cross,backround,backround,
    backround,backround,backround,cross,backround,backround,cross,backround,backround,backround,
    backround,backround,backround,backround,cross,cross,backround,backround,backround,backround,
    backround,backround,backround,backround,cross,cross,backround,backround,backround,backround,
    backround,backround,backround,cross,backround,backround,cross,backround,backround,backround,
    backround,backround,cross,backround,backround,backround,backround,cross,backround,backround,
    backround,cross,backround,backround,backround,backround,backround,backround,cross,backround,
    cross,backround,backround,backround,backround,backround,backround,backround,backround,cross
  };
  display.drawBmp(150,50,10,10,test,4);

  display.drawBmp(0, 0, 80, 80, image_data_Ostfalia_logo_80x80,1);
  
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



