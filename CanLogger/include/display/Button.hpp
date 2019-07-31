#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include "display/ILI9341.hpp"
using namespace std;
class Button
{
protected:
    ILI9341& display;
    unsigned long startX;
    unsigned long startY;
    unsigned long sizeX;
    unsigned long sizeY;
    unsigned long backColor;
    byte offsetX;
    byte offsetY;
    string text;
    unsigned long textColor;
    bool isSelected;

public:
    Button (ILI9341& display, unsigned long startX, unsigned long startY, unsigned long sizeX, 
            unsigned long sizeY, unsigned long backColor,byte offsetX, byte offsetY, string text, unsigned long textColor, bool isSelected);
    bool getStatus();
    void drawButton();
    void setFrame();
    void resetFrame();
};



#endif