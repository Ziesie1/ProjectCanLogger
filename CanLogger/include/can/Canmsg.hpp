/* 
Um die CAN-funktion nutzen zu können muss "HAL_CAN_MODULE_ENABLED" definiert sein
*/

#ifndef CANMSG_HPP
#define CANMSG_HPP

#include <Arduino.h>

class Canmsg
{
public:
	static constexpr uint8_t maxLength = 8; //highest possbible value for canLength
	static constexpr uint16_t maxTime = 0xffff; //highest possbible value for time
	static constexpr uint16_t maxStdId = 0x7ff; //highest possbible value for stdIdentifier
	static constexpr uint32_t maxExtId = 0x3ffff; //highest possbible value for extIdentifier
	static constexpr uint32_t maxFullId = 0x1fffffff; //highest possbible value for the combination of stdIdentifier and extIdentifier
	static constexpr uint8_t maxDataVal = 0xff; //highest possbible value of the single data Bytes

private:
    uint32_t stdIdentifier;	//Standart identifier or MSB of the extended identifier
	uint32_t extIdentifier; //LSBs of the Extended identifier
	bool isExtIdentifier; //Message has extended identifier
	bool rtr;
	uint16_t time;
	uint8_t canLength = 0;
	uint8_t* data = nullptr;
	
	void destroy(void);
	void moveDestroy(void);
	
public:	
	Canmsg();
	Canmsg(uint16_t stdId, uint32_t extId, bool isExtId, bool rtr, uint16_t time, 
		   uint8_t canLength, uint8_t const * const data);  
	Canmsg(uint16_t stdId, uint32_t extId, bool isExtId, bool rtr, uint16_t time,
		   uint8_t canLength, uint8_t databit0 = 0x00, uint8_t databit1 = 0x00, 
		   uint8_t databit2 = 0x00, uint8_t databit3 = 0x00, uint8_t databit4 = 0x00, 
		   uint8_t databit5 = 0x00, uint8_t databit6 = 0x00, uint8_t databit7 = 0x00);  

	~Canmsg();
	Canmsg(Canmsg const& other);
	Canmsg& operator= (Canmsg const& other);
	Canmsg(Canmsg && other);
	Canmsg& operator= (Canmsg && other);
	
	explicit operator String() const;
	uint8_t operator[](int idx) const;
	bool operator==(Canmsg const& other) const;
	bool operator!=(Canmsg const& other) const;

	// Get-Funktionen:
	uint16_t GetStdIdentifier() const;
	uint32_t GetExtIdentifier() const;
	uint32_t GetFullId() const;
	bool GetIsExtIdentifier() const;
	bool GetRtr() const;
	uint16_t GetTime() const;
	uint8_t GetCanLength() const;
	uint8_t GetCanByte(int const idx) const;
	
	// Bus-Funktionen:
	friend HAL_StatusTypeDef CanUtility_RecieveMessage(bool const fifo, Canmsg *const msg);
	friend HAL_StatusTypeDef CanUtility_SendMessage(Canmsg *const msg);
};

#endif //CANMSG