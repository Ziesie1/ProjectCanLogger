#ifndef ARROW_HPP
#define ARROW_HPP

#include "display/ILI9341.hpp"
#include "display/elements/Button.hpp"

class Arrow
{
    protected:
    ILI9341& display;
    Button& button;
    long posX1; //Anfang 1
    long posY1; //Anfang 1
    long posX2; //Anfang 2
    long posY2; //Anfang 2
    long posX3; //Spitze
    long posY3; //Spitze
    


    public:
    Arrow(ILI9341& display,Button button, long posX1, long posY1, long posX2, long , long posX3, long posY3);
    void draw(Color Farbe);

};


#endif