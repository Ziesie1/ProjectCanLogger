#ifndef SCREENBUFFER_HPP
#define SCREENBUFFER_HPP

#include "can/Canmsg.hpp"

//Debug:
void printScreenBufferSerial(void);
void printScreenBufferUserViewSerial(void);

//behandlung einzelner Nachrichten (interne)
//void sortCanMessageIntoBuffer(Canmsg const& msg);
//void insertMessageHere(Canmsg const& msg, int pos);

// steuern des frontendbuffer
void screenBuffer_enableUpdate(void);
void screenBuffer_disableUpdate(void);
void screenBuffer_clearScreenBuffer(void);

// initialisierungsfunktionen
void screenBufferInit(void);
void screenBufferDeinit(void);

// loop zum bearbeiten der Nachrichten
void loopScreenBuffer(void);

// request information while running
bool screenBuffer_hasSomethingChanged(void);
bool screenBuffer_hasThisMessageChanged(int pos);
Canmsg const* screenBuffer_getMessageAtPosition(int pos);
int screenBuffer_getFillLevel(void);

constexpr int SCREEN_BUFFER_SIZE = 10;
//extern int screenBufferFillLevel;
//extern int screenBufferUserViewFillLevel;
//extern Canmsg** screenBuffer;
//extern Canmsg** screenBufferUserView;

#endif //SCREENBUFFER_HPP