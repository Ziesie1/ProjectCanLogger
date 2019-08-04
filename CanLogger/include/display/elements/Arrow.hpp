#ifndef ARROW_HPP
#define ARROW_HPP

#include "display/ILI9341.hpp"
#include "display/elements/Button.hpp"

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
    bool isSelected;
    const Color COLOR_ARROW_SELECTED = BLACK;
    const Color COLOR_ARROW_UNSELECTED = WHITE;
    


    public:
    Arrow(ILI9341& display,Button* button,bool isSelected);
    void draw(Color Farbe);
    void setPositions();
};


#endif