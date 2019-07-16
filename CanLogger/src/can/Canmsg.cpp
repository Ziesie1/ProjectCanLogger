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
    this->stdIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_STID) >> CAN_RI0R_STID_Pos); //stdIdentifier aus dem CAN_RIxR Register
	this->extIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_EXID) >> CAN_RI0R_EXID_Pos);
	this->isExtIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_IDE) >> CAN_RI0R_IDE_Pos);
	this->rtr = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_RTR) >> CAN_RI0R_RTR_Pos); //Remote transmission Reguest aus dem CAN_RIxR Register
	this->time = ((CAN->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_TIME) >> CAN_RDT0R_TIME_Pos); //timestamp aus dem CAN_RDTxR Register
	this->canLength = ((CAN->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_DLC) >> CAN_RDT0R_DLC_Pos); //anzahl der bytes aus dem CAN_RDTxR Register
	this->canBytes[0] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA0) >> CAN_RDL0R_DATA0_Pos); //Daten bytes aus dem CAN_RDLxR Register
	this->canBytes[1] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA1) >> CAN_RDL0R_DATA1_Pos);
	this->canBytes[2] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA2) >> CAN_RDL0R_DATA2_Pos);
	this->canBytes[3] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA3) >> CAN_RDL0R_DATA3_Pos);
	this->canBytes[4] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA4) >> CAN_RDH0R_DATA4_Pos); //Daten bytes aus dem CAN_RDHxR Register
	this->canBytes[5] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA5) >> CAN_RDH0R_DATA5_Pos);
	this->canBytes[6] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA6) >> CAN_RDH0R_DATA6_Pos);
	this->canBytes[7] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA7) >> CAN_RDH0R_DATA7_Pos);
	if(!fifo)
	{
		CAN->RF0R |= CAN_RF0R_RFOM0; //Empfange Nachricht freigeben im CAN_RF0R Register
	}
	else
	{
		CAN->RF1R |= CAN_RF1R_RFOM1; //Empfange Nachricht freigeben im CAN_RF1R Register
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
		for(int i=0; i<header.DLC; i++)
		{
			data[i] = this->canBytes[i];
		}
		uint32_t mailbox = 0;
		HAL_CAN_AddTxMessage(&CanUtility_hcan, &header, data, &mailbox);
	}
  else
  {
    Serial.println("Alle TX Mailboxen belegt");
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

