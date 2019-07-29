#include <Arduino.h>
#include "buttons/Taster.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

namespace singleTaster{
    constexpr int TASTER_PIN = PC4;
    constexpr unsigned long ENTPRELL_ZEIT_TASTER = 50;

    bool zustandTasteralt = false;
    unsigned long alteZeitTaster = 0;

    bool tasterPressed = false;
}

using namespace singleTaster;

// Initialisierung des Tasters
void initTaster()
{
    pinMode(TASTER_PIN, INPUT);
    scom.printDebug("Taster initialisiert");
}

// muss für die funktionalität des Tasters ständig aufgerufen werden
void loopTaster()
{
    if(millis() - alteZeitTaster >= ENTPRELL_ZEIT_TASTER)
    {
        bool pinStatus = !digitalRead(TASTER_PIN);
        if(pinStatus != zustandTasteralt)
        {
            zustandTasteralt = pinStatus;
            alteZeitTaster = millis();
            if(pinStatus)
            {
                tasterPressed = true;
            }
            //scom << "Zustand Taster2: " << pinStatus << endz; // später auskommentieren
        }
    }
}

bool wasSingleTasterPressed()
{
    bool ret = tasterPressed;
    tasterPressed = false;
    return ret;
}

