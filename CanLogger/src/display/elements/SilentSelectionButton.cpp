#include "display\elements\SilentSelectionButton.hpp"
#include "can/CanUtility.hpp"


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
    /*
    if(this->buffPos = 1)
    {
        CanUtility_setTransmissionMode(true);
    }else
    {
        CanUtility_setTransmissionMode(false);
    }
    */
}


void SilentSelectionButton::readValue()
{
    /*
    if(CanUtility_getSilentMode())
    {
        this->buffPos = 0;
    }else
    {
        this->buffPos = 1;
    }
    */
    this->buffPos = 1;
}
