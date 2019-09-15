#ifndef SILENT_SELECTION_BUTTON_HPP
#define SILENT_SELECTION_BUTTON_HPP

#include "display/elements/SelectionButton.hpp"


/*
    This Class is the virtual parent class vor the selektion class in the settings screen 
*/
class SilentSelectionButton : public SelectionButton
{

protected:
    static const byte silentBuffSize = 2;
    const string silentTextBuff[2] = {"on", "off"};
    const bool RING_BUFF_VALUE = false;

    virtual void readValue();

public:
    SilentSelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor, unsigned long textColor, bool isSelected);
    virtual ~SilentSelectionButton();
    
    virtual void saveValue();
    
};


#endif
