#ifndef CANMSG
#define CANMSG

class Canmsg
{
    static constexpr char maxLength = 8;
    char16_t stdIdentifier;	//Standart identifier or MSB of the extended identifier
	char32_t extIdentifier; //LSBs of the Extended identifier
	bool isExtIdentifier; //Message has extended identifier
	bool rtr;
	char16_t time;
	unsigned char canLength;
	unsigned char canBytes[maxLength];
public:
    Canmsg();
    void PrintSerial(void) const;
    //operator String() const;  //Der compiler erkennt String aus mir unbekannten Gründen nicht
};

#endif //CANMSG