#include <Arduino.h>
#include "buttons/Taster.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

constexpr int tasterPin = PC4;
volatile boolean zustandTaster = false;
unsigned long aktuelleZeitTaster = 0;
unsigned long alteZeitTaster = 0;
unsigned long entprellZeitTaster = 50;
unsigned long diff = 0;

void doTasterPin()
{
    aktuelleZeitTaster = millis();
    diff = aktuelleZeitTaster - alteZeitTaster;
    if(diff > entprellZeitTaster)
    {
        if(zustandTaster)
        {
            zustandTaster = false;
        }
        else
        {
            zustandTaster = true;
        }
        
        scom<<"Interrupt Taster 2"<<endz;
        scom<<zustandTaster<<endz;
    }
}

void initTaster()
{
    pinMode(tasterPin,INPUT);
    digitalWrite(tasterPin,LOW);

    attachInterrupt(digitalPinToInterrupt(tasterPin),doTasterPin,CHANGE);

    scom<<"Taster initialisiert"<<endz;
}
