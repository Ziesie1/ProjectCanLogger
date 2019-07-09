#include <Arduino.h>
#include <SdFat.h>
#include "sd/SD.hpp"
#include "serial/SerialCommunication.hpp"

using namespace utilities;

#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))

//Definitionen
constexpr byte SD_CS = PB6;

SdFat sdKarte {};

void init_SD()
{
    // Erster Schreibvorgang auf der SD-Karte
    if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
        scom.printError("Bei der Initialisierung der SD-Karte trat ein Fehler auf.\nIst die Karte eingesteckt?");
        //sdKarte.initErrorPrint();
    }

    if (!sdKarte.exists("/CPP4Live")) {
        if (!sdKarte.mkdir("/CPP4Live")) {
            scom.printError("Der Ordner /CPP4Live konnte nicht erstellt werden.");
            //sdKarte.errorPrint();
        }else{
            scom.printDebug("/CPP4Live wurde erstellt");
        }
    }else{
        scom.printDebug("/CPP4Live existiert bereits");
    }
}