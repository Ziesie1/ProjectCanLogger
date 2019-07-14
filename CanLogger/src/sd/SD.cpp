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
XFile canLogFile {sdKarte};

void init_SD()
{
    // SD-Karte Initialisiern
    if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
        scom.printError("Bei der Initialisierung der SD-Karte trat ein Fehler auf.\nIst die Karte eingesteckt?");
        //sdKarte.initErrorPrint();
    }

    // Ordner /can_log erstellen
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

}

void createNewCanLogFile()
{
    canLogFile.setFilePath(DIRECOTRY_CAN);

    int number = 0;
    do  // freie log_X.txt Datei finden
    {
        String str = FILE_NAME_CAN;
        str += number;
        str += FILE_TYPE_CAN;
        canLogFile.setFileName(str);
        number++;
    } while (canLogFile.exists());
    
    scom.printDebug(canLogFile.getFileName());
     
    canLogFile.writeStr("identifier(dez);RTR bit;time stamp(hex);data length(dez);byte 1(hex);byte 2(hex);byte 3(hex);byte 4(hex);byte 5(hex);byte 6(hex);byte 7(hex);byte 8(hex)\n");

}

String getFullLogFilePath()
{
    return canLogFile.getTotalFilePath();
}


void saveNewCanMessage(Canmsg const& msg)
{
    bool result = canLogFile.appendStr(static_cast<String>(msg)+'\n');
    if(result == false)
    {
        String str = "Die folgende Can-Nachricht konnte nicht gespeichert werden:\n";
        str += static_cast<String>(msg);
        str += "\nIst die Speicherkarte noch eingesteckt?";
        scom.printError(str);
    }
}
