
#ifndef SELECTION_BUTTON_HPP
#define SELECTION_BUTTON_HPP

#include "display/elements/Button.hpp"



/*
    This Class is the virtual parent class vor the selektion class in the settings screen 
*/
class SelectionButton : public Button
{

protected:
    
    bool isPressed = false;
    bool ringBuff = false;
    byte buffPos = 0;
    byte textBuffSize = 0;
    string* textBuff = nullptr;
    
    const unsigned long COLOR_GREEN_FRAME_PRESSED = 0x004d6e; //R,G,B = 112, 173, 71
    

    SelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, uint16_t sizeY, 
                    unsigned long backColor, unsigned long textColor, bool isSelected, bool ringBuff, byte buffSize);


    void unpressButton();
    bool getPressed();
    virtual void setText();
    void initText();
    
    virtual void readValue() = 0;

public:
    virtual ~SelectionButton();
    void pressButton();
    bool loop();

    virtual void saveValue() = 0;  //saveValues

};


#endif
