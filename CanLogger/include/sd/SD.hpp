#ifndef SD_HPP
#define SD_HPP

#include "Canmsg.h"

// SD-Karte wird initialisiert und eine Ordnerstuktur angelegt
// Muss immer nach dem erneuten Einstecken der SD-Karte aufgerufen werden
void init_SD();
// Neues Logfile zur Speicherung wird angelegt.
void createNewCanLogFile();
// Gibt den Namen des aktuellen LogFiles zurück
String getFullLogFilePath();
// Speichert eine übergebene Canmsg auf der SD-Karte
void saveNewCanMessage(Canmsg const& msg);


#endif