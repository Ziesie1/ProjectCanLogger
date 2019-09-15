#include "display\elements\SpeedSelectionButton.hpp"
#include "can/CanUtility.hpp"

/*
    Constructor of the class SpeedSelectionButton
    input:  display     - reference of the display where the Button will be printed
            startx      - start position on x-axis
            starty      - start position on y-axis
            sizeX       - button width
            sizeY       - button hight
            backColor   - button collor
            textColor   - collor of the text
            isSelected  - Status of the button
*/
SpeedSelectionButton::SpeedSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor,
                                unsigned long textColor, bool isSelected)
                                :SelectionButton(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected, SPEED_RING_BUFF_VALUE, speeBuffSize)
                                {
                                    this->textBuff = new string[this->textBuffSize];

                                    for(int idx = 0; idx < this->textBuffSize; idx++)
                                    {
                                        this->textBuff[idx] = speedTextBuff[idx];
                                    }
                                    this->readValue();
                                    this->initText();
                                    drawButton();
                                }
/*
    destructor of the class SpeedSelectionButton
*/
SpeedSelectionButton::~SpeedSelectionButton()
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
void SpeedSelectionButton::saveValue()
{
    switch (this->buffPos)
    {
    case 0: CanUtility_setTransmissionSpeed(CAN_100_KBIT); break;
    case 1: CanUtility_setTransmissionSpeed(CAN_125_KBIT); break;
    case 2: CanUtility_setTransmissionSpeed(CAN_200_KBIT); break;
    case 3: CanUtility_setTransmissionSpeed(CAN_250_KBIT); break;
    case 4: CanUtility_setTransmissionSpeed(CAN_400_KBIT); break;
    case 5: CanUtility_setTransmissionSpeed(CAN_500_KBIT); break;
    }
    
}

/*
    reeding the actual value 
*/
void SpeedSelectionButton::readValue()
{
    switch (CanUtility_getTransmissionSpeed())
    {
    case CAN_100_KBIT: this->buffPos= 0; break;
    case CAN_125_KBIT: this->buffPos= 1; break;
    case CAN_200_KBIT: this->buffPos= 2; break;
    case CAN_250_KBIT: this->buffPos= 3; break;
    case CAN_400_KBIT: this->buffPos= 4; break;
    case CAN_500_KBIT: this->buffPos= 5; break;
    default: break;
    }

}
