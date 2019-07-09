/* 
Um die CAN-funktion nutzen zu können muss in der Detei "stm32f3xx_hal_conf.h" 
(C:\Users\<User>\.platformio\packages\framework-arduinoststm32\variants\NUCLEO_F303RE)
die Zeile 55 entkommentiert werden ("//" löschen). 
Die Zeile muss dann lauten: "#define HAL_CAN_MODULE_ENABLED"
*/

#ifndef CANMSG_HPP
#define CANMSG_HPP
#include <string.h>

#define CAN_MSG_CAN_BUFFER_REC_SIZE 50// umändern auf constexpr


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
	Canmsg();
    explicit operator String() const;  //Der compiler erkennt String aus mir unbekannten Gründen nicht
	//operator= (Canmsg &const other);
	void Send(void) const;
	void Recieve(bool const fifo);
};

namespace CANutil
{
	void Init(void);
	bool CheckMailbox(bool const fifo);
	static Canmsg bufferCanRecMessages[CAN_MSG_CAN_BUFFER_REC_SIZE];
	static int bufferCanRecPointer;
	void CanCheck(void);
}

#endif //CANMSG