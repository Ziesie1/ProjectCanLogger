#ifndef SD_HPP
#define SD_HPP

#include <string.h>
#include "can/Canmsg.hpp"

void init_SD();
void createNewCanLogFile();
void startWriting();
void stopWriting();
String getFullLogFilePath();
bool saveNewCanMessage(Canmsg const& msg);


#endif