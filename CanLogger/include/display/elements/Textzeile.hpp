#ifndef TEXTZEILE_HPP
#define TEXTZEILE_HPP

#include "display/ILI9341.hpp"
#include "can/Canmsg.hpp"

class Textzeile
{
    protected:
    ILI9341& display;
    Canmsg message;
    bool isSelected;
    const Color COLOR_BACKGROUND_SELECTED = RED100;
    const Color COLOR_BACKGROUND_UNSELECTED = WHITE;
    const Color COLOR_FONT = BLACK;
    const uint8_t ZEILENHOEHE = 20;
    const uint8_t OFFSETX1 = 5;
    const uint8_t OFFSETX2 = 31; //OFFSET1+2Zeichen(16Pixel)+Abstand von 10 Pixeln
    const uint8_t OFFSETX3 = 73; //OFFSEZ2+4Zeichen(32 Pixel)+Abstand von 10 Pixeln


    public:
    Textzeile(ILI9341& display, Canmsg msg, bool isSelected);
    void printImportantContent(uint8_t posY);
    void printWholeContent();
    void selectZeile();
    void unselectZeile();

};



#endif