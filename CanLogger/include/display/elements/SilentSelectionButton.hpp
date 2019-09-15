#ifndef SILENT_SELECTION_BUTTON_HPP
#define SILENT_SELECTION_BUTTON_HPP

#include "display/elements/SelectionButton.hpp"


/*
    This Class creates the Button to enabel and disable teh silent mode 
*/
class SilentSelectionButton : public SelectionButton
{

protected:
    static const byte silentBuffSize = 2;
    static const bool RING_BUFF_VALUE = false;
    const string silentTextBuff[2] = {"on", "off"};
    

    virtual void readValue();

public:
    SilentSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor, unsigned long textColor, bool isSelected);
    virtual ~SilentSelectionButton();
    
    virtual void saveValue();
    
};


#endif
