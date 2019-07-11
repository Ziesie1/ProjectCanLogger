#include <Arduino.h>
#include <SdFat.h>
#include "sd/SD.hpp"
#include "serial/SerialCommunication.hpp"
#include "sd/XFile.hpp"

using namespace utilities;

#define errorExit(msg) errorHalt(F(msg))
#define initError(msg) initErrorHalt(F(msg))

//Definitionen
constexpr byte SD_CS = PB6;
constexpr char DIRECOTRY_CAN[] = "/can_log";
constexpr char FILE_NAME_CAN[] = "log_";
constexpr char FILE_TYPE_CAN[] = ".txt";

SdFat sdKarte {};

void init_SD()
{
    // Erster Schreibvorgang auf der SD-Karte
    if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
        scom.printError("Bei der Initialisierung der SD-Karte trat ein Fehler auf.\nIst die Karte eingesteckt?");
        //sdKarte.initErrorPrint();
    }

    if (!sdKarte.exists(DIRECOTRY_CAN)) {
        if (!sdKarte.mkdir(DIRECOTRY_CAN)) {
            String str = "Der Ordner ";
            str += DIRECOTRY_CAN;
            str += " konnte nicht erstellt werden.";
            scom.printError(str);
            //sdKarte.errorPrint();
        }else{
            String str = DIRECOTRY_CAN;
            str += " wurde erstellt.";
            scom.printDebug(str);
        }
    }else{
        String str = DIRECOTRY_CAN;
        str += " existiert bereits.";
        scom.printDebug(str);
    }

    if (sdKarte.chdir(DIRECOTRY_CAN)) { // Verzeichnis gesetzt
        String dateiName = FILE_NAME_CAN;
        dateiName += 0;
        dateiName += FILE_TYPE_CAN;

        XFile file1;
        if (!file1.open(dateiName.c_str(), O_RDWR | O_CREAT | O_TRUNC)) {
            String str = dateiName;
            str += " konnte nicht geöffnet/erstellt werden.";
            scom.printError(str);
        }

        // write data to /Dir1/test.bin on sd1
        String serialAusgabe = "";
        file1.writeStr("Dies ist ein erster test, ob die Datei richtig beschrieben wird!\n");
        uint32_t zeit = millis();
        for(int i = 0; i < 10; i++)
        {
            String ausgabe = "Ich bin auf das Ergebnis gespannt. [";
            ausgabe += i;
            ausgabe += "]\n";

            zeit = millis();
            file1.appendStr(ausgabe);
            serialAusgabe += String(millis()-zeit);
            serialAusgabe += " ms\n";
            
        }
        scom.printDebug(serialAusgabe);
        


    }
}
