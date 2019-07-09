#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "sd/SD.hpp"
#include "Canmsg.h"
#include "serial/SerialCommunication.hpp"

Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PA10, PB5, PC8}; // use Software Serial
//Adafruit_ILI9341 display = Adafruit_ILI9341{PC9, PA8, PC8}; // use Hardware Serial

using namespace utilities; // für scom

//Variablen zur Auswertung des Drehencoders
int oldval = 1;
constexpr int encoderPinA = PC7;
constexpr int encoderPinB = PC6;
volatile int encoderPos = 0;
int encoder0PinALast = LOW;
boolean rotating = false;
boolean A_set = false;            
boolean B_set = false;

void doEncoderA(){

  if ( rotating ) {
    delay (1);  // wait a little until the bouncing is done
    if( digitalRead(encoderPinA) != A_set ) {  // debounce once more
      A_set = !A_set;
      if ( A_set && !B_set ){ 
        encoderPos ++;
      rotating = false; 
      } // no more debouncing until loop() hits again
    }
  }
}
void doEncoderB(){

  if ( rotating ){
    delay (1);
    if( digitalRead(encoderPinB) != B_set ) {
      B_set = !B_set;
      //  adjust counter - 1 if B leads A
      if( B_set && !A_set ){ 
        encoderPos --;
        rotating = false;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  scom.workWith(Serial); // scom Hardwareserial zuweisen
  scom.showDebugMessages(true); // Debugmodus einschalten
  
  init_SD();

  display.begin();
  display.fillScreen(ILI9341_BLACK);
  display.setTextSize(3);
  display.print("Test eines Textes...");

  pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);

  //digitalWrite(encoderPinA,HIGH);
  //digitalWrite(encoderPinB,HIGH);

  //attachInterrupt(0,doEncoderA,CHANGE);
  //attachInterrupt(1,doEncoderB,CHANGE);

  scom << "CanLogger ist Initialisiert" << endz;
}

void loop() {
  
  rotating = true;
  if(oldval != encoderPos)
  {
    Serial.println(encoderPos);
    oldval = encoderPos;
  }
  
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



