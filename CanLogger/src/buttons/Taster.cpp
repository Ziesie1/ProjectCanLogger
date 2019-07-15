#include <Arduino.h>
#include "buttons/Taster.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

constexpr int tasterPin = PC4;
int zustandTaster = 0;
int zustandTasteralt = 0;
unsigned long aktuelleZeitTaster = 0;
unsigned long alteZeitTaster = 0;
unsigned long entprellZeitTaster = 50;
unsigned long diff = 0;

// Interruptroutine, die aufgrund der Pinbelegung nicht genutzt werden kann
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

//Initialisierung des Tasters
void initTaster()
{
    pinMode(tasterPin,INPUT);
    digitalWrite(tasterPin,HIGH);

    scom<<"Taster initialisiert"<<endz;
}

//Entprellen: Zustand wird nach der eingestellten Entprellzeit erneut abgefragt und ggf. korrigiert
//Der Tasterausgang wird beim Betätigen auf Masse gezogen -> Verneinung beim Lesen des Tasters
boolean entprellen(int pin)
{
    boolean zustand = !digitalRead(pin);
    delay(entprellZeitTaster);
    if(!digitalRead(pin) == zustand)
    {
        return zustand;
    }
    else
    {
        return !digitalRead(pin);
    }
}

//Der Zustand ( gedrückt == 1 ; unbetätigt == 0) wird beim Wechsel im seriellen Monitor ausgegeben
void auswertungTaster()
{
    zustandTaster = entprellen(tasterPin);
    if(zustandTaster != zustandTasteralt)
    {
        scom<<zustandTaster<<endz;
        zustandTasteralt = zustandTaster;
        
    }
}

