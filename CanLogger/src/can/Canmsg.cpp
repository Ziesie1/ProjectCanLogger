#include <Arduino.h>
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"

Canmsg Canmsg_bufferCanRecMessages[CAN_MSG_CAN_BUFFER_REC_SIZE];
int Canmsg_bufferCanRecPointer;

/*
    Canmsg::Canmsg()
    Zu Testzwecken, Standartwerte
*/
Canmsg::Canmsg()
    :stdIdentifier{0x100},extIdentifier{0},isExtIdentifier{false},
        rtr{false},time{0x1000},canLength{maxLength}
{
    for(byte i=0;i<this->maxLength;i++)
    {
        canBytes[i]=static_cast<char>(0x01+(0x22*i));
    }
}

Canmsg::operator String() const
{
    String s="Identifier: ";
    if(this->isExtIdentifier)
    {
        s+=String(this->stdIdentifier<<18|this->extIdentifier,HEX);
    }
    else
    {
        s+=String(this->stdIdentifier,HEX);
    }
    s+="h RTR: ";
    s+=String(this->rtr);
    s+=" Time: ";
    s+=String(this->time,HEX);
	s+="h Laenge: ";
    s+=String(this->canLength,HEX);
    s+="h Inhalt: ";
    for(byte i=0;i<this->canLength;i++)
    {
        if(this->canBytes[i]<0x10)
        {
            s+="0";
        }
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

