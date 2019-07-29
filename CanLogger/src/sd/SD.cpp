#include <Arduino.h>
#include <SdFat.h>
#include "sd/SD.hpp"
#include "sd/XFile.hpp"
#include "utilities/SerialCommunication.hpp"


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

/*
    Initialized the SD-Card and create default folder.
    Must recalled after reinserting the SD-Card.
*/
void init_SD()
{
    // SD-Karte Initialisiern
    if (!sdKarte.begin(SD_CS, SD_SCK_MHZ(18))) {
        scom.printError("Bei der Initialisierung der SD-Karte trat ein Fehler auf.\nIst die Karte eingesteckt?");
        //sdKarte.initErrorPrint();
        return;
    }

    // Ordner /can_log erstellen
    String debugInfo = "SD-Karte initialisiert. Info: ";
    if (!sdKarte.exists(DIRECOTRY_CAN)) {
        if (!sdKarte.mkdir(DIRECOTRY_CAN)) {
            String str = "Der Ordner ";
            str += DIRECOTRY_CAN;
            str += " konnte nicht erstellt werden.";
            scom.printError(str);
            //sdKarte.errorPrint();
            return;
        }else{
            debugInfo += DIRECOTRY_CAN;
            debugInfo += " wurde erstellt.";
        }
    }else{
        debugInfo += DIRECOTRY_CAN;
        debugInfo += " existierte bereits.";
    }

    scom.printDebug(debugInfo);
}

/*
    Create a new Canlog file in the default folder.
*/
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
    
    String ausgabe = "Neues Speicherziel: ";
    ausgabe += canLogFile.getFileName();
    scom.printDebug(ausgabe);
     
    canLogFile.writeStr("identifier(dez);RTR bit;time stamp(hex);data length(dez);byte 1(hex);byte 2(hex);byte 3(hex);byte 4(hex);byte 5(hex);byte 6(hex);byte 7(hex);byte 8(hex)\n");
}

/*
    Returns the full filepath of the current Canlog file.
*/
String getFullLogFilePath()
{
    return canLogFile.getTotalFilePath();
}

/*
    Save the Canmessage in the current Canlog file.
*/
void saveNewCanMessage(Canmsg const& msg)
{
    String output = String(msg.GetStdIdentifier()) + ";";
    output += String(msg.GetRtr()) + ";";
    output += String(msg.GetTime(),HEX) + ";";
    output += String(msg.GetCanLength()) + ";";
    for(int i = 0; i < 8; i++)
        output += String(msg.GetCanByte(i),HEX) + ";";
    output.remove(output.length()-1); // letztes Semikolon entfernen

    bool result = canLogFile.appendStrLn(output);
    if(result == false)
    {
        String str = "Die folgende Can-Nachricht konnte nicht gespeichert werden:\n";
        str += static_cast<String>(msg);
        str += "\nIst die Speicherkarte noch eingesteckt?";
        scom.printError(str);
    }
}
