#include <Arduino.h>
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"

Canmsg Canmsg_bufferCanRecMessages[Canmsg_CAN_BUFFER_REC_SIZE];
int Canmsg_bufferCanRecPointer;

/*
    Canmsg::Canmsg()
    Zu Testzwecken, Standartwerte
*/
Canmsg::Canmsg()
    : stdIdentifier{0x100},extIdentifier{0},isExtIdentifier{false},
        rtr{false},time{0x1000},canLength{maxLength}
{
    for(byte i=0;i<this->maxLength;i++)
    {
        canBytes[i]=static_cast<char>(0x01+(0x22*i));
    }
}

Canmsg::Canmsg(char16_t stdId, char32_t extId, bool isExtId, bool rtr, char16_t time, 
                uint8_t canLength, uint8_t const * const data)
    :Canmsg{stdId, extId, isExtId, rtr, time, canLength}
{
    if(data != nullptr)
    {
        for(uint8_t i=0; i<this->canLength; i++)
        {
            this->canBytes[i] = data[i];
        }
    }
    else
    {
        this->canLength = 0;
    }
    for(uint8_t i=this->canLength; i<maxLength; i++)
    {
        this->canBytes[i] = 0x00;
    }
}

Canmsg::Canmsg(char16_t stdId, char32_t extId, bool isExtId, bool rtr, char16_t time, 
			    uint8_t canLength, uint8_t databit0, uint8_t databit1, 
			    uint8_t databit2, uint8_t databit3, uint8_t databit4, 
			    uint8_t databit5, uint8_t databit6, uint8_t databit7)
{
    if(stdId <= maxStdId)
    {
        this->stdIdentifier = stdId;
    }
    else
    {
        this->stdIdentifier = maxStdId;
    }
    if(isExtId)
    {
        if(extId <= maxExtId)
        {
            this->extIdentifier = extId;
        }
        else
        {
            this->extIdentifier = maxExtId;
        }
    }
    else
    {
        this->extIdentifier = 0;
    }

    if(canLength<maxLength)
    {
        this->canLength = canLength;
    }
    else
    {
        this->canLength = maxLength;
    }
    
    uint8_t data[8] = {databit0, databit1, databit2, databit3, 
                        databit4, databit5, databit6, databit7};
    for(uint8_t i=0; i<this->canLength; i++)
    {
        this->canBytes[i] = data[i];
    }
}
	

Canmsg::Canmsg(Canmsg const& other)
{
    *this = other;
}

Canmsg& Canmsg::operator= (Canmsg const& other)
{
    this->stdIdentifier = other.stdIdentifier;
    this->extIdentifier = other.extIdentifier;
    this->isExtIdentifier = other.isExtIdentifier;
    this->rtr = other.rtr;
    this->time = other.time;
    this->canLength = other.canLength;
    for(byte i=0; i<maxLength; i++)
    {
        this->canBytes[i] = other.canBytes[i];
    }
    return *this;
}	

void AddZerosToStringHex(String& s,int const var,int const maxStellen)
{
  for(int i=0x10; i<pow(0x10,maxStellen); i*=0x10)
  {
    if(var < i)
    {
      s+="0";
    }
  }
}

Canmsg::operator String() const
{
    String s="Identifier: ";
    if(this->isExtIdentifier)
    {
      char32_t fullId = this->stdIdentifier<<18|this->extIdentifier;
      AddZerosToStringHex(s, fullId, 8);
      s+=String(fullId, HEX);
    }
    else
    {
      AddZerosToStringHex(s, this->stdIdentifier, 3);
      s+=String(this->stdIdentifier,HEX);
    }
    s+="h RTR: ";
    s+=String(this->rtr);
    s+=" Time: ";
    AddZerosToStringHex(s, this->time, 4);
    s+=String(this->time,HEX);
	  s+="h Laenge: ";
    s+=String(this->canLength,HEX);
    s+="h Inhalt: ";
    for(byte i=0;i<this->canLength;i++)
    {
        AddZerosToStringHex(s, this->canBytes[i], 2);
        s+=String(this->canBytes[i],HEX);
        if(i<canLength-1)
        {
            s+=".";
        }   
    }
    s+=" h";
    return s;
}

/*
Liest eine im FIFO liegende Nachricht ein
Input: 	fifo 		- aus welchem FIFO soll gelesen werden 
					  moegliche Werte: 0,1
return:	void
*/
void Canmsg::Recieve(bool const fifo)
{
    if(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, fifo) != 0)
    {
        CAN_RxHeaderTypeDef header;
        uint8_t data[8];
        if(HAL_CAN_GetRxMessage(&CanUtility_hcan, fifo, &header, data) == HAL_OK)
        {
            this->stdIdentifier = header.StdId;
            this->extIdentifier = header.ExtId;
            this->isExtIdentifier = header.IDE;
            this->rtr = header.RTR;
            this->time = header.Timestamp;
            this->canLength = header.DLC;
            for(int i=0; i<canLength; i++)
            {
            this->canBytes[i] = data[i];
            }
        }
        else
        {
          Serial.println("CAN-Nachricht konnte nicht ausgelesen werden");
        }
    }
}

/*
Sendet eine CAN Nachricht
Input: 	void
return:	void
*/
void Canmsg::Send(void) const
{
	if(HAL_CAN_GetTxMailboxesFreeLevel(&CanUtility_hcan) != 0)
	{
		CAN_TxHeaderTypeDef header;
		if(!this->isExtIdentifier)
    {
      header.IDE = CAN_ID_STD;
    }
    else
    {
      header.IDE = CAN_ID_EXT;
    }
    header.StdId = this->stdIdentifier;
    header.ExtId = this->extIdentifier;
    if(!this->rtr)
    {
      header.RTR = CAN_RTR_DATA;
    }
    else
    {
      header.RTR = CAN_RTR_REMOTE;
    }
		header.DLC = this->canLength;
		header.TransmitGlobalTime = DISABLE;
		uint8_t data[8];
		for(uint8_t i=0; i<header.DLC; i++)
		{
			data[i] = this->canBytes[i];
		}
		uint32_t mailbox = 0;
		HAL_CAN_AddTxMessage(&CanUtility_hcan, &header, data, &mailbox);
	}
  else
  {
    Serial.println("CAN-Nachricht konnte nicht gesendet werden(Alle TX Mailboxen belegt).");
  }
}

char16_t Canmsg::GetStdIdentifier() const
{
  return this->stdIdentifier;
}

char32_t Canmsg::GetExtIdentifier() const
{
  return this->extIdentifier;  
}

bool Canmsg::GetIsExtIdentifier() const
{
  return this->isExtIdentifier;
}

bool Canmsg::GetRtr() const
{
  return this->rtr;
}

char16_t Canmsg::GetTime() const
{
  return this->time;
}

byte Canmsg::GetCanLength() const
{
  return this->canLength;
}

byte Canmsg::GetCanByte(int const idx) const
{
  if(idx<this->maxLength)
  {
    return this->canBytes[idx];
  }
  else
  {
    return 0;
  }
}

