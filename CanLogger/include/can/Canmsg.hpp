/* 
Um die CAN-funktion nutzen zu können muss in der Detei "stm32f3xx_hal_conf.h" 
(C:\Users\<User>\.platformio\packages\framework-arduinoststm32\variants\NUCLEO_F303RE)
die Zeile 55 entkommentiert werden ("//" löschen). 
Die Zeile muss dann lauten: "#define HAL_CAN_MODULE_ENABLED"
*/

#ifndef CANMSG_HPP
#define CANMSG_HPP

class Canmsg
{
public:
	static constexpr byte maxLength = 8;
	static constexpr char16_t maxStdId = 0x7ff;
	static constexpr char32_t maxExtId = 0x3ffff;

private:
    char16_t stdIdentifier;	//Standart identifier or MSB of the extended identifier
	char32_t extIdentifier; //LSBs of the Extended identifier
	bool isExtIdentifier; //Message has extended identifier
	bool rtr;
	char16_t time;
	uint8_t canLength;
	uint8_t canBytes[maxLength];
	
public:	
	Canmsg();
	Canmsg(Canmsg const& other);
	Canmsg(char16_t stdId, char32_t extId, bool isExtId, bool rtr, char16_t time, 
			uint8_t canLength, uint8_t const * const data);  
	Canmsg(char16_t stdId, char32_t extId, bool isExtId, bool rtr, char16_t time, 
			uint8_t canLength, uint8_t databit0 = 0x00, uint8_t databit1 = 0x00, 
			uint8_t databit2 = 0x00, uint8_t databit3 = 0x00, uint8_t databit4 = 0x00, 
			uint8_t databit5 = 0x00, uint8_t databit6 = 0x00, uint8_t databit7 = 0x00);  
	Canmsg& operator= (Canmsg const& other);
	explicit operator String() const;
	
	// Get-Funktionen:
	char16_t GetStdIdentifier() const;
	char32_t GetExtIdentifier() const;
	bool GetIsExtIdentifier() const;
	bool GetRtr() const;
	char16_t GetTime() const;
	uint8_t GetCanLength() const;
	uint8_t GetCanByte(int const idx) const;
	
	// Bus-Funktionen:
	void Send(void) const;// noch nicht verwendbar
	void Recieve(bool const fifo);// noch nicht verwendbar
};

constexpr int Canmsg_CAN_BUFFER_REC_SIZE = 50;
extern Canmsg Canmsg_bufferCanRecMessages[Canmsg_CAN_BUFFER_REC_SIZE];
extern int Canmsg_bufferCanRecPointer;

#endif //CANMSG