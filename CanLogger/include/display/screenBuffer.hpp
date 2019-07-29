#ifndef SCREENBUFFER_HPP
#define SCREENBUFFER_HPP

#include "can/Canmsg.hpp"

void printScreenBufferSerial(void);
void printScreenBufferUserViewSerial(void);
void sortCanMessageIntoBuffer(Canmsg const& msg);
void insertMessageHere(Canmsg const& msg, int pos);
void makeBufferVisible(void);
void clearBuffer(void);
void screenBufferInit(void);
void screenBufferDeinit(void);

constexpr int SCREEN_BUFFER_SIZE = 10;
extern int screenBufferFillLevel;
extern int screenBufferUserViewFillLevel;
extern Canmsg* screenBuffer;
extern Canmsg* screenBufferUserView;

#endif //SCREENBUFFER_HPP