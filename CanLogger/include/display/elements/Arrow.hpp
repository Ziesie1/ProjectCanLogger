#ifndef ARROW_HPP
#define ARROW_HPP

#include "display/ILI9341.hpp"
#include "display/elements/Button.hpp"
/*
    This class prints an arrow left to the selected button on the homescreen
*/
class Arrow
{
    protected:
    ILI9341& display;
    Button* button;
    long posX1; //Anfang 1
    long posY1; //Anfang 1
    long posX2; //Anfang 2
    long posY2; //Anfang 2
    long posX3; //Spitze
    long posY3; //Spitze
    unsigned long colorArrowSelected;
    unsigned long colorArrowUnselected;
    bool isSelected;
    const uint8_t ARROWWIDTH = 1;
    


    public:
    Arrow(ILI9341& display, Button* button, unsigned long colorSelected, unsigned long colorUnselected, bool isSelected);
    void draw(unsigned long Farbe);
    void setPositions();
};


#endif