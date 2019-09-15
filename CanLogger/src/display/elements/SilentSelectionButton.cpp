#include "display\elements\SilentSelectionButton.hpp"
#include "can/CanUtility.hpp"


SilentSelectionButton::SilentSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor,
                                unsigned long textColor, bool isSelected)
                                :SelectionButton(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected, RING_BUFF_VALUE, silentBuffSize)
                                {   
                                    this->textBuff = new string[this->textBuffSize];

                                    for(int idx = 0; idx < this->textBuffSize; idx++)
                                    {
                                        this->textBuff[idx] = silentTextBuff[idx];
                                    }
                                    this->readValue();
                                    this->initText();
                                    drawButton();
                                    if(isSelected)
                                    {
                                    selectButton();
                                    }
                                }

/*
SilentSelectionButton::SilentSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor,
                                unsigned long textColor, bool isSelected)
                                :SelectionButton(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected, false, 2)
                                {   
                                    
                                    this->textBuff = new string{2};

                                    textBuff[0] = "ön";
                                    textBuff[1] = "öff";

                                    this->readValue();
                                    this->initText(); 
                                    drawButton();
                                    if(isSelected)
                                    {
                                    selectButton();
                                    }
                                    
                                }
*/
SilentSelectionButton::~SilentSelectionButton()
{
    if(this->textBuff)
     {
         delete this->textBuff;
         this->textBuff = nullptr;
     } 
}

void SilentSelectionButton::saveValue()
{
    
    if(this->buffPos == 0)
    {
        if((CanUtility_getTransmissionMode() == CAN_TransmissionMode_Normal) || (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent))
        {
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Silent);
        }else if ((CanUtility_getTransmissionMode() == CAN_TransmissionMode_Loopback) || (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent_Loopback))
        {
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Silent_Loopback);
        }  
    }else if(this->buffPos == 1)
    {
        if((CanUtility_getTransmissionMode() == CAN_TransmissionMode_Normal) || (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent))
        {
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Normal);
        }else if ((CanUtility_getTransmissionMode() == CAN_TransmissionMode_Loopback) || (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent_Loopback))
        {
            CanUtility_setTransmissionMode(CAN_TransmissionMode_Loopback);
        }  
    }
    
}


void SilentSelectionButton::readValue()
{
    if((CanUtility_getTransmissionMode() == CAN_TransmissionMode_Normal) || (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Loopback))
    {
        this->buffPos = 1;
    }else if (CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent || CanUtility_getTransmissionMode() == CAN_TransmissionMode_Silent_Loopback)
    {
        this->buffPos = 0;
    }
    
}
