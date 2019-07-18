/* 
Um die CAN-funktion nutzen zu können muss in der Detei "stm32f3xx_hal_conf.h" 
(C:\Users\<User>\.platformio\packages\framework-arduinoststm32\variants\NUCLEO_F303RE)
die Zeile 55 entkommentiert werden ("//" löschen). 
Die Zeile muss dann lauten: "#define HAL_CAN_MODULE_ENABLED"
*/

#ifndef CANMSG_HPP
#define CANMSG_HPP

#define CAN_MSG_CAN_BUFFER_REC_SIZE 50// umändern auf constexpr

class Canmsg
{
public:
	static constexpr byte maxLength = 8;

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
    explicit operator String() const;  //Der compiler erkennt String aus mir unbekannten Gründen nicht
	//operator= (Canmsg &const other);// noch nicht implementiert
	
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

extern Canmsg Canmsg_bufferCanRecMessages[CAN_MSG_CAN_BUFFER_REC_SIZE];
extern int Canmsg_bufferCanRecPointer;

#endif //CANMSG