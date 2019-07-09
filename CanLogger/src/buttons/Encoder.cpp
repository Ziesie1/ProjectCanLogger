#include <Arduino.h>
#include "buttons/Encoder.hpp"

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


void initEncoder()
{
    pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);

  //digitalWrite(encoderPinA,HIGH);
  //digitalWrite(encoderPinB,HIGH);

  //attachInterrupt(0,doEncoderA,CHANGE);
  //attachInterrupt(1,doEncoderB,CHANGE);
}
void loopEncoder()
{
  rotating = true;
  if(oldval != encoderPos)
  {
    Serial.println(encoderPos);
    oldval = encoderPos;
  }
}