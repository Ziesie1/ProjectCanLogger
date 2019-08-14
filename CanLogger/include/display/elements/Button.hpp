#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include "display/ILI9341.hpp"
using namespace std;
class Button
{
protected:
    ILI9341& display;
    uint16_t startX;
    uint16_t startY;
    uint16_t sizeX;
    uint16_t sizeY;
    unsigned long backColor;
    byte offsetX;
    byte offsetY;
    string text;
    unsigned long textColor;
    bool isSelected;
    const byte COLOR_FRAME_SELECTED[3] = {237,125,49};
    const byte COLOR_FRAME_UNSELECTED[3] = {1,64,118};
    const byte COLOR_ARROW_SELECTED[3] = {237,125,49};
    const unsigned long COLOR_ARROW_UNSELECTED = WHITE;
    const int FRAME_WIDTH = 3;



public:
    Button (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor,byte offsetX, byte offsetY, string text, unsigned long textColor, bool isSelected);
    bool getStatus();
    void drawButton();
    void selectButton();
    void unselectButton();
    void drawArrow();
    void deleteArrow();
    uint16_t getPosX();
    uint16_t getSizeX();
    uint16_t getPosY();
    uint16_t getSizeY();
    

};



#endif