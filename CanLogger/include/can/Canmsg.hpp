/* 
Um die CAN-funktion nutzen zu können muss in der Detei "stm32f3xx_hal_conf.h" 
(C:\Users\<User>\.platformio\packages\framework-arduinoststm32\variants\NUCLEO_F303RE)
die Zeile 55 entkommentiert werden ("//" löschen). 
Die Zeile muss dann lauten: "#define HAL_CAN_MODULE_ENABLED"
*/

#ifndef CANMSG_HPP
#define CANMSG_HPP

#include <Arduino.h>

class Canmsg
{
public:
	static constexpr uint8_t maxLength = 8;
	static constexpr uint16_t maxTime = 0xffff;
	static constexpr uint16_t maxStdId = 0x7ff;
	static constexpr uint32_t maxExtId = 0x3ffff;
	static constexpr uint8_t maxDataVal = 0xff;

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
	Canmsg(bool const empty);
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