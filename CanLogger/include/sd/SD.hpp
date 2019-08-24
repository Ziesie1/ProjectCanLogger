#ifndef SD_HPP
#define SD_HPP

#include <string.h>
#include "can/Canmsg.hpp"

void init_SD();
void startSD();
void closeSD();
void createNewCanLogFile();
String getFullLogFilePath();
bool saveNewCanMessage(Canmsg const& msg);


#endif