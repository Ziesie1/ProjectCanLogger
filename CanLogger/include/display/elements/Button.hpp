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
    Color backColor;
    byte offsetX;
    byte offsetY;
    string text;
    Color textColor;
    bool isSelected;
    const Color COLOR_FRAME_SELECTED = BLACK;
    const Color COLOR_FRAME_UNSELECTED = BLUE100;
    const Color COLOR_ARROW_SELECTED = BLACK;
    const Color COLOR_ARROW_UNSELECTED = WHITE;
    const int FRAME_WIDTH = 2;



public:
    Button (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, Color backColor,byte offsetX, byte offsetY, string text, Color textColor, bool isSelected);
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