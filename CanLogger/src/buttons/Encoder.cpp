#include <Arduino.h>
#include "buttons/Encoder.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

//Variablen zur Auswertung des Drehencoders
namespace encoder{
  constexpr int ENCODER_PIN_A = PC7;
  constexpr int ENCODER_PIN_B = PC6;
  constexpr int ENCODER_PIN_TASTER = PB4;

  constexpr unsigned long entprellZeitEncoder = 50; // in ms
  constexpr unsigned long entprellZeitEncoderTaster = 50; // in ms

  volatile int encoderPos = 0;
  volatile bool tasterSet = false;
  volatile bool tasterPressed = false;       
  volatile unsigned long alteZeitEncoder = 0;
  volatile unsigned long alteZeitEncoderTaster = 0;
}

using namespace encoder;

//Interrupt Service Routinen
//Um ein Prellen zu verhindern wird die Aufrufzeit des Interrupts verwendet
void doEncoderA()
{
  unsigned long diffEncoder = millis()-alteZeitEncoder;
  if( diffEncoder > entprellZeitEncoder)
  {
    if(digitalRead(ENCODER_PIN_B) == HIGH ) 
    { 
      encoderPos ++;
    }
    else
    {
      encoderPos--;
    }
  
    alteZeitEncoder = millis();
    scom.println(encoderPos); // später auskommentieren
   }
}


void doEncoderTast()
{
  unsigned long diffEncoderTaster = millis() - alteZeitEncoderTaster;
  if(diffEncoderTaster > entprellZeitEncoderTaster)
  {
    tasterSet = !tasterSet;
    if(tasterSet)
    {
      tasterPressed = true;
    }
    alteZeitEncoderTaster = millis();

    scom<<"Interrupt ausgelöst"<<endz; // später auskommentieren
    scom<<tasterSet<<endz; // später auskommentieren
  }
}

void initEncoder()
{
  pinMode (ENCODER_PIN_A, INPUT);
  pinMode (ENCODER_PIN_B, INPUT);
  pinMode (ENCODER_PIN_TASTER, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A),doEncoderA,FALLING);//Auswertung von PinB ist beim verbauten Encoder aufgrund der Rastpunkte nicht sinnvoll
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_TASTER),doEncoderTast,CHANGE);
  
  scom.printDebug("Encoder ist Initialisiert");
}

int getEncoderValue()
{
  return encoderPos;
}

bool wasEncoderButtonPressed()
{
  bool ret = tasterPressed;
  tasterPressed = false;
  return ret;
}