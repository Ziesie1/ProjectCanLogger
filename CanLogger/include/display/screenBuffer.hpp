#ifndef SCREENBUFFER_HPP
#define SCREENBUFFER_HPP

#include "can/Canmsg.hpp"

//Debug:
void printScreenBufferSerial(void);
void printScreenBufferUserViewSerial(void);

//behandlung einzelner Nachrichten
void sortCanMessageIntoBuffer(Canmsg const& msg);
void insertMessageHere(Canmsg const& msg, int pos);

// kopieren der Nachrichten aus dem back- zum frontendbuffer
extern bool updateUserView;

// initialisierungsfunktionen
void screenBufferInit(void);
void screenBufferDeinit(void);

// loop zum bearbeiten der Nachrichten
void loopScreenBuffer(void);

constexpr int SCREEN_BUFFER_SIZE = 10;
extern int screenBufferFillLevel;
extern int screenBufferUserViewFillLevel;
extern Canmsg* screenBuffer;
extern Canmsg* screenBufferUserView;

#endif //SCREENBUFFER_HPP