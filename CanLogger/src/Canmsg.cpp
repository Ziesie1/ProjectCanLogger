#include <Arduino.h>
#include "Canmsg.h"    

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
        s+=this->stdIdentifier;
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


