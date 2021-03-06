#include "can/Canmsg.hpp"
#include "utilities/utilities.hpp"
#include "utilities/SerialCommunication.hpp"

int messagesWithNew = 0; //counter of the ammount of currently existing objects of Canmsg (for debug purposes)

/*
    creates a default CAN-message used e.g. in tests 
*/
Canmsg::Canmsg()
    : Canmsg(0x100, 0x0, false, false, 0x1000, 8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef)  
{

}

/* 
    creates a CAN-message, where the databits are passed via a array
    Input:  stdId     - specifies the standard identifier of the message 
                        possible values: 0 - 0x7ff
            extId     - specifies the extended Identifier of the message (may only of impact when isExtId is true) 
                        possible values: 0 - 0x3ffff
            isExtId   - specifies if the message is using the extended Identifier
                        possible values: true, false
            rtr       - specifies if the remote transmission request feature of CAN should be used
                        possible values: true, false
            time      - specifies the timestamp of the message, 
                        note that the timestamp specified here 
                        will not be the same as the timestamp sended by the CAN hardware
                        this argument is a placeholder for further extensions
                        possible values: 0 - 0xffff
            canLength - specifies the length of the message in Bytes
                        possible values: 0 - 8      
            data      - array containing the databits of the message, 
                        note that even if the arry contains more content, 
                        the constructor will only copy the ammount of values specifies in canLength
                        possible values of the single databits: 0 - 0xff
*/
Canmsg::Canmsg(uint16_t stdId, uint32_t extId, bool isExtId, bool rtr, uint16_t time, 
               uint8_t canLength, uint8_t const * const data)
    :Canmsg{stdId, extId, isExtId, rtr, time, canLength}
{
    if(data != nullptr)
    {
        for(uint8_t i=0; i<this->canLength; i++)
        {
            this->data[i] = data[i];
        }
    }
    else
    {
        this->canLength = 0;
    }
    for(uint8_t i=this->canLength; i<maxLength; i++)
    {
        this->data[i] = 0x00;
    }
}

/* 
    creates a CAN-message, where the databits are passed one by one
    Input:  stdId     - specifies the standard identifier of the message 
                        possible values: 0 - 0x7ff
            extId     - specifies the extended Identifier of the message (may only of impact when isExtId is true) 
                        possible values: 0 - 0x3ffff
            isExtId   - specifies if the message is using the extended Identifier
                        possible values: true, false
            rtr       - specifies if the remote transmission request feature of CAN should be used
                        possible values: true, false
            time      - specifies the timestamp of the message, 
                        note that the timestamp specified here 
                        will not be the same as the timestamp sended by the CAN hardware
                        this argument is a placeholder for further extensions
                        possible values: 0 - 0xffff
            canLength - specifies the length of the message in Bytes
                        possible values: 0 - 8      
            databit0  - specifies the first byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit1  - specifies the second byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit2  - specifies the third byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit3  - specifies the fourth byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit4  - specifies the fifth byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit5  - specifies the sixth byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit6  - specifies the seventh byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
            databit7  - specifies the eighth byte of the message
                        possible values: 0 - 0xff
                        note that even if there are more databitx values surpassed, 
                        the constructor will only copy the ammount of values specifies in canLength
*/
Canmsg::Canmsg(uint16_t stdId, uint32_t extId, bool isExtId, bool rtr, uint16_t time, 
			   uint8_t canLength, uint8_t databit0, uint8_t databit1, 
			   uint8_t databit2, uint8_t databit3, uint8_t databit4, 
			   uint8_t databit5, uint8_t databit6, uint8_t databit7)
  :isExtIdentifier{isExtId}, rtr{rtr}, time{time}
{
    messagesWithNew ++;
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
    
    this->data = new uint8_t[this->maxLength];

    uint8_t data[8] = {databit0, databit1, databit2, databit3, 
                        databit4, databit5, databit6, databit7};
    for(uint8_t i=0; i<this->canLength; i++)
    {
        this->data[i] = data[i];
    }
}
	
/* 
    function which deletes dynamic resources of a CAN-message
*/
void Canmsg::destroy(void)
{
    if(this->data)
    {
        delete[] this->data;
    }
    this->moveDestroy();
}

/* 
    function which sets dynamic resources as deleted
*/
void Canmsg::moveDestroy(void)
{
    this->data = nullptr;
    this->canLength = 0;
}

/* 
    destructor for CAN-messages
*/
Canmsg::~Canmsg()
{
    messagesWithNew--;
    this->destroy();
}

/* 
    creates a CAN-message equal to the one passed to the constuctor
    Input:  other - areference to a instance of Canmsg, 
                    whose values will be copied into this instance
*/
Canmsg::Canmsg(Canmsg const& other)
{
    messagesWithNew++;
    *this = other;
}

/* 
    operator to copy a message
    Input:  other - a reference to a instance of Canmsg, 
                    whose values will be copied into this instance
*/
Canmsg& Canmsg::operator= (Canmsg const& other)
{
    this->destroy();
    this->stdIdentifier = other.stdIdentifier;
    this->extIdentifier = other.extIdentifier;
    this->isExtIdentifier = other.isExtIdentifier;
    this->rtr = other.rtr;
    this->time = other.time;
    this->canLength = other.canLength;
    this->data = new uint8_t[this->maxLength];
    for(byte i=0; i<maxLength; i++)
    {
        this->data[i] = other.data[i];
    }
    return *this;
}	

/* 
    creates a CAN-message equal to the one passed to the constuctor
    Input:  other - a R-value instance of Canmsg whose values will be copied into this instance
*/
Canmsg::Canmsg(Canmsg && other)
{
    messagesWithNew++;
    (*this) = std::move(other);
}

/*
    operator to move a message
    Input:  other - a R-value instance of Canmsg whose values will be copied into this instance
*/
Canmsg& Canmsg::operator= (Canmsg && other)
{
    this->destroy();
    this->stdIdentifier = other.stdIdentifier;
    this->extIdentifier = other.extIdentifier;
    this->isExtIdentifier = other.isExtIdentifier;
    this->rtr = other.rtr;
    this->time = other.time;
    this->canLength = other.canLength;
    this->data = other.data;
    other.moveDestroy();
    return (*this);
}

/* 
    operator to transform a CAN-message into a String
    return: String that contains all the important data of the CAN-message
*/
Canmsg::operator String() const
{
    String s="Identifier: ";
    if(this->isExtIdentifier)
    {
        char32_t fullId = (this->stdIdentifier<<18)|this->extIdentifier;
        AddZerosToString(s, fullId, Canmsg::maxStdId<<18|Canmsg::maxExtId, HEX);
        s+=String(fullId, HEX);
    }
    else
    {
        AddZerosToString(s, this->stdIdentifier, Canmsg::maxStdId, HEX);
        s+=String(this->stdIdentifier,HEX);
    }
    s+="h RTR: ";
    s+=String(this->rtr);
    s+=" Time: ";
    AddZerosToString(s, this->time, Canmsg::maxTime, HEX);
    s+=String(this->time,HEX);
	s+="h Laenge: ";
    s+=String(this->canLength,HEX);
    s+="h Inhalt: ";
    for(byte i=0;(i<this->canLength)&&(!this->rtr);i++)
    {
        AddZerosToString(s, this->data[i], Canmsg::maxDataVal, HEX);
        s+=String(this->data[i],HEX);
        if(i<canLength-1)
        {
            s+=".";
        }   
    }
    if((this->canLength > 0) && (!this->rtr))
    {
        s+=" h";
    }
    return s;
}

/* 
    operator to get the value of single data bits of the CAN-message
    Input: idx  - number of the data bit
                  note if idx is higher than canLength the information is possibly irrelevant
                  possible values: 0 - 7
    return: value of the data bit of the message
*/
uint8_t Canmsg::operator[](int idx) const
{
    if(idx<this->maxLength)
    {
        return this->data[idx];
    }
    else
    {
        return 0;
    }
}

/*
    operator which checks if to CAN-messages are congruent
    input:  other - a reference to a CAN-message that will be compared with this CAN-message
    return: true  - messages are congruent
            false - messages are not congruent
*/
bool Canmsg::operator==(Canmsg const& other) const
{
    if(this->isExtIdentifier!=other.isExtIdentifier)
    {
        return false;
    }
    if(this->stdIdentifier!=other.stdIdentifier)
    {
        return false;
    }
    if(this->isExtIdentifier && (this->extIdentifier!=other.extIdentifier))
    {
        return false;
    }
    if(this->rtr!=other.rtr)
    {
        return false;
    }
    if(this->time!=other.time)
    {
        return false;
    }
    if(this->canLength!=other.canLength)
    {
        return false;
    }
    for(int i=0; i<this->canLength; i++)
    {
        if(this->data[i]!=other.data[i])
        {
            return false;
        }
    }
    return true;
}

/*
    operator which checks if to CAN-messages are congruent
    Input:  other - a reference to a CAN-message that will be compared with this CAN-message
    return: false  - messages are congruent
            true   - messages are not congruent
*/
bool Canmsg::operator!=(Canmsg const& other) const
{
    return !(*this==other);
}

/* 
    function to get the standard Identifier of the CAN-message
    return: value of the standard Identifier of the message
*/
uint16_t Canmsg::GetStdIdentifier() const
{
    return this->stdIdentifier;
}

/* 
    function to get the extended Identifier of the CAN-message
    return: value of the extended Identifier of the message
*/
uint32_t Canmsg::GetExtIdentifier() const
{
    return this->extIdentifier;  
}

/* 
    function to get the full Identifier of the CAN-message,
    regardless whether it uses the extended identifier or not
    return: value of the Identifier of the message
*/
uint32_t Canmsg::GetFullId() const
{
    if(this->isExtIdentifier)
    {
        return ((this->stdIdentifier<<18)|this->extIdentifier);
    }
    else
    {
        return this->stdIdentifier;
    }
}

/* 
    function to get the information, if the extended Identifier is used in this CAN-message
    return: true  - extended Identifier is used
            false - extended identifier is not used
*/
bool Canmsg::GetIsExtIdentifier() const
{
    return this->isExtIdentifier;
}

/* 
    function to get the information if the CAN-message is a remote transmission request
    return: true  - CAN-message is a remote transmission request
            false - CAN-message is no remote transimisson request
*/
bool Canmsg::GetRtr() const
{
    return this->rtr;
}

/* 
    function to get the timestamp of the CAN-message
    return: value of the timestamp of the message
*/
uint16_t Canmsg::GetTime() const
{
    return this->time;
}

/* 
    function to get the ammount of relevant data bits of the CAN-message
    return: ammount of relevant data bits of the message
*/
byte Canmsg::GetCanLength() const
{
    return this->canLength;
}

/* 
    function to get the value of single data bits of the CAN-message
    Input: idx  - number of the data bit
                  note if idx is higher than canLength the information is possibly irrelevant
                  possible values: 0 - 7
    return: value of the data bit of the message
*/
byte Canmsg::GetCanByte(int const idx) const
{
    return (*this)[idx];
}

