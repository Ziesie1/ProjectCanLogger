#ifndef SCREENBUFFER_HPP
#define SCREENBUFFER_HPP

#include "can/Canmsg.hpp"

constexpr int SCREEN_BUFFER_SIZE = 18; 
/*
size of the "screenBuffer" and "screenBufferUserView"
also known as Anzeigebuffer
*/

//Debug:
void printScreenBufferSerial(void);
void printScreenBufferUserViewSerial(void);

// initialisierungsfunktionen
void screenBufferInit(void);
void screenBufferDeinit(void);

// loop zum bearbeiten der Nachrichten
void loopScreenBuffer(void);

// steuern des frontendbuffer
bool screenBuffer_enableUpdate(void);
bool screenBuffer_disableUpdate(void);
bool screenBuffer_clearScreenBuffer(void);

// request information while running
bool screenBuffer_hasSomethingChanged(void);
bool screenBuffer_hasThisMessageChanged(int pos);
bool screenBuffer_getMessageAtPosition(Canmsg & msg, int pos);
int screenBuffer_getFillLevel(void);
bool screenBuffer_updateStatus(void);

#endif //SCREENBUFFER_HPP