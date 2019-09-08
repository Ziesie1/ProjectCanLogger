#include <Arduino.h>
#include "buttons/Encoder.hpp"
#include "utilities/SerialCommunication.hpp"

using namespace utilities;

//Variablen zur Auswertung des Drehencoders
namespace encoder{
  constexpr int ENCODER_PIN_A = PC7;
  constexpr int ENCODER_PIN_B = PC6;
  constexpr int ENCODER_PIN_TASTER = PB4;

  constexpr unsigned long ENTRPRELL_ZEIT_ENCODER = 50; // in ms
  constexpr unsigned long ENTPRELL_ZEIT_ENCODER_TASTER = 50; // in ms

  volatile int encoderPos = 0;
  volatile bool encoderPosChanged = false;
  volatile bool tasterSet = false;
  volatile bool tasterPressed = false;
  volatile unsigned long alteZeitEncoder = 0;
  volatile unsigned long alteZeitEncoderTaster = 0;
}

using namespace encoder;

// Interrupt Service Routinen
// Um ein Prellen zu verhindern wird die Aufrufzeit des Interrupts verwendet
void doEncoderA()
{
  if (millis() - alteZeitEncoder > ENTRPRELL_ZEIT_ENCODER)
  {
    encoderPosChanged = true;
    if(digitalRead(ENCODER_PIN_B) == HIGH ) 
    { 
      encoderPos ++;
    }
    else
    {
      encoderPos--;
    }
  
    alteZeitEncoder = millis();
    //scom << encoderPos << endz; // später auskommentieren
   }
}

// Zur Auswertung des Tasters im Encoder
void doEncoderTast()
{
  if(millis() - alteZeitEncoderTaster > ENTPRELL_ZEIT_ENCODER_TASTER)
  {
    tasterSet = !tasterSet;
    if(tasterSet)
    {
      tasterPressed = true;
    }
    alteZeitEncoderTaster = millis();

    //scom<<"Interrupt ausgelöst"<<endz; // später auskommentieren
    //scom<<tasterSet<<endz; // später auskommentieren
  }
}

// Initialisierung des Encoders
void initEncoder()
{
  pinMode (ENCODER_PIN_A, INPUT);
  pinMode (ENCODER_PIN_B, INPUT);
  pinMode (ENCODER_PIN_TASTER, INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A),doEncoderA,FALLING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_TASTER),doEncoderTast,CHANGE);
  
  scom.printDebug("Encoder ist Initialisiert");
}

/*
    Gibt die relative Position des Encoders zurück.
    Dabei werden die inkrementalen Schritte seid der letzen Abfrage gezählt.
*/
int getRelativeEncoderPos()
{
  encoderPosChanged = false;
  int ret = encoderPos;
  encoderPos = 0;
  return ret;
}


bool wasEncoderButtonPressed()
{
  bool ret = tasterPressed;
  tasterPressed = false;
  return ret;
}

bool hasEncoderPosChanged()
{
  bool ret = encoderPosChanged;
  encoderPosChanged = false;
  return ret;
 
}

/*
  Setzt den aktuellen Status des Inkrementalgers zurück.
*/
void resetEncoderStates()
{
  encoderPosChanged = false;
  encoderPos = 0;
  tasterPressed = false;
}