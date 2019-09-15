#ifndef SPEED_SELECTION_BUTTON_HPP
#define SPEED_SELECTION_BUTTON_HPP

#include "display/elements/SelectionButton.hpp"
#include "can/CanUtility.hpp"

/*
    This Class creates the Button seleczt the transmission speed 
*/
class SpeedSelectionButton : public SelectionButton
{
protected:

    static const byte speeBuffSize = 6;
    static const bool SPEED_RING_BUFF_VALUE = true;
    const string speedTextBuff[speeBuffSize] = {"100 kBit","125 kBit","200 kBit","250 kBit","400 kBit","500 kBit"};
    

    virtual void readValue();

public:
    SpeedSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor, unsigned long textColor, bool isSelected);
    virtual ~SpeedSelectionButton();

    virtual void saveValue();  
};


#endif
