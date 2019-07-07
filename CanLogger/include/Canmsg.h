#ifndef CANMSG_HPP
#define CANMSG_HPP

#include <string.h>

class Canmsg
{
    static constexpr byte maxLength = 8;
    char16_t stdIdentifier;	//Standart identifier or MSB of the extended identifier
	char32_t extIdentifier; //LSBs of the Extended identifier
	bool isExtIdentifier; //Message has extended identifier
	bool rtr;
	char16_t time;
	byte canLength;
	byte canBytes[maxLength];
	
public:
    Canmsg(); // Zu Testzwecken
    operator String() const;
};

#endif //CANMSG