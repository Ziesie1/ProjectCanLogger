#include "display\elements\SilentSelectionButton.hpp"
#include "can/CanUtility.hpp"

/*
    Constructor of the class SilentSelectionButton
    input:  display     - reference of the display where the Button will be printed
            startx      - start position on x-axis
            starty      - start position on y-axis
            sizeX       - button width
            sizeY       - button hight
            backColor   - button collor
            textColor   - collor of the text
            isSelected  - Status of the button
*/
SilentSelectionButton::SilentSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor,
                                unsigned long textColor, bool isSelected)
                                :SelectionButton(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected, false, silentBuffSize)
                                {   
                                    this->textBuff = new string[this->textBuffSize];

                                    for(int idx = 0; idx < this->textBuffSize; idx++)
                                    {
                                        this->textBuff[idx] = silentTextBuff[idx];
                                    }
                                    this->readValue();
                                    this->initText();
                                    drawButton();
                                }

/*
    destructor of the class SilentSelectionButton
*/
SilentSelectionButton::~SilentSelectionButton()
{
    if(this->textBuff)
     {
         delete this->textBuff;
         this->textBuff = nullptr;
     } 
}

/*
    Saving the placed changes
*/
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

/*
    reeding the actual value 
*/
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
