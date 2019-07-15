#include <Arduino.h>
#include "buttons/Encoder.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

//Variablen zur Auswertung des Drehencoders
constexpr int encoderPinA = PC7;
constexpr int encoderPinB = PC6;
constexpr int encoderPinTaster = PB4;
volatile int encoderPos = 0;
volatile boolean tasterSet = false;            
unsigned long aktuelleZeitEncoder = 0;
unsigned long aktuelleZeitEncoderTaster = 0;
unsigned long alteZeitEncoder = 0;
unsigned long alteZeitEncoderTaster = 0;
unsigned long entprellZeitEncoder = 50;
unsigned long entprellZeitEncoderTaster = 50;
unsigned long diffEncoder;
unsigned long diffEncoderTaster;


//Interrupt Service Routinen
//Um ein Prellen zu verhindern wird die Aufrufzeit des Interrupts verwendet
void doEncoderA()
{
  aktuelleZeitEncoder = millis();
  diffEncoder = aktuelleZeitEncoder-alteZeitEncoder;
  if( diffEncoder > entprellZeitEncoder)
  {
    if(digitalRead(encoderPinB) == HIGH ) 
    { 
    
      encoderPos ++;
      
    }
    else
    {
      encoderPos--;
    }
  
    
    alteZeitEncoder = aktuelleZeitEncoder;
    scom.println(encoderPos);
   }
  


}


void doEncoderTast()
{
  aktuelleZeitEncoderTaster = millis();
  diffEncoderTaster = aktuelleZeitEncoderTaster - alteZeitEncoderTaster;
  if(diffEncoderTaster > entprellZeitEncoderTaster)
  {
    if(tasterSet)
    {
      tasterSet = false;
    }
    else
    {
      tasterSet = true;
    }
   
    	alteZeitEncoderTaster = aktuelleZeitEncoderTaster;
      scom<<"Interrupt ausgelöst"<<endz;
      scom<<tasterSet<<endz;
  }

  
}

void initEncoder()
{
  
  

  pinMode (encoderPinA, INPUT);
  pinMode (encoderPinB, INPUT);
  pinMode (encoderPinTaster, INPUT);

  digitalWrite(encoderPinA,LOW);
  digitalWrite(encoderPinB,LOW);
  digitalWrite(encoderPinTaster,LOW);

  attachInterrupt(digitalPinToInterrupt(encoderPinA),doEncoderA,FALLING);//Auswertung von PinB ist beim verbauten Encoder aufgrund der Rastpunkte nicht sinnvoll
  attachInterrupt(digitalPinToInterrupt(encoderPinTaster),doEncoderTast,CHANGE);
  
  scom << "Encoder ist Initialisiert" << endz;
}

int getEncoderValue()
{
  return encoderPos;
}