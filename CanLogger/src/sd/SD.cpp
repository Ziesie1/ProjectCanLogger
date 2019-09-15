#include <Arduino.h>
#include <SdFat.h>
#include "sd/SD.hpp"
#include "sd/XFile.hpp"
#include "utilities/SerialCommunication.hpp"
#include "utilities/utilities.hpp"


//Definitionen
namespace SD_Card{
    constexpr byte SD_CS = PB6;
    constexpr char DIRECOTRY_CAN[] = "/can_log";
    constexpr char FILE_NAME_CAN[] = "log_";
    constexpr char FILE_TYPE_CAN[] = ".txt";

    bool sdCardInitialized = false;
    SdFat sdKarte {};
    XFile canLogFile {sdKarte};
}

using namespace SD_Card;
using namespace utilities;

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
    sdCardInitialized = true;

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
    Start the process for the SD-Card.
    Must be called bevore Canmessages can be saved to the SD-Card.
    createNewCanLogFile() must be called bevore!
    input:
    return:
*/
void startSD()
{
    canLogFile.open();
}

/*
    Close the process for the SD-Card.
    Must be called before removing the SD-Card.
    All data in cache will be written to the SD-Card.
    input:
    return:
*/
void closeSD()
{
    sdCardInitialized = false;
    canLogFile.close();
}


/*
    Create a new Canlog file in the default folder.
*/
void createNewCanLogFile()
{
    if(sdCardInitialized)
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
        
        canLogFile.open();

        bool result = canLogFile.appendStrLn("identifier(hex);RTR bit;time stamp(hex);data length(hex);byte 1(hex);byte 2(hex);byte 3(hex);byte 4(hex);byte 5(hex);byte 6(hex);byte 7(hex);byte 8(hex)");
        if(result == false)
        {
            String str = "Das Canlog file konnte nicht beschrieben werden:\n";
            str += "\nIst die Speicherkarte noch eingesteckt?";
            scom.printError(str);
        }

        canLogFile.close();
    }
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
bool saveNewCanMessage(Canmsg const& msg)
{
    if(sdCardInitialized)
    {
        String output = "";
        AddZerosToString(output,msg.GetFullId(),msg.maxFullId,HEX); // nullen hinzufügen
        output += String(msg.GetFullId(), HEX) + ";";
        output += String(msg.GetRtr()) + ";";
        
        AddZerosToString(output,msg.GetTime(),msg.maxTime,HEX); // nullen hinzufügen
        output += String(msg.GetTime(),HEX) + ";";
        output += String(msg.GetCanLength(), HEX) + ";"; // nur von 0 - 8
        for(int i = 0; i < 8; i++)
        {
            if(i < msg.GetCanLength())
            {
                AddZerosToString(output,msg.GetCanByte(i),msg.maxDataVal,HEX);
                output += String(msg.GetCanByte(i),HEX);
            }else{
                AddZerosToString(output,0,msg.maxDataVal,HEX);
                output += "0";
            }
            if(i < 8-1)
                output += ";";
        }

        bool result = canLogFile.appendStrLn(output);
        if(result == false)
        {
            String str = "Die folgende Can-Nachricht konnte nicht gespeichert werden:\n";
            str += static_cast<String>(msg);
            str += "\nIst die Speicherkarte noch eingesteckt?";
            scom.printError(str);
        }
        return result;
    }
    return false;
}
