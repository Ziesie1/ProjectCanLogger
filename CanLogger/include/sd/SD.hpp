#ifndef SD_HPP
#define SD_HPP

#include "can/Canmsg.hpp"

void init_SD();
void createNewCanLogFile();
String getFullLogFilePath();
void saveNewCanMessage(Canmsg const& msg);


#endif