#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include "display/ILI9341.hpp"
#include "display/elements/Table.hpp"
using namespace std;
/*
    This class creates a button for selection in the homescreen menu.
*/
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
    string text = "";
    unsigned long textColor;
    bool isSelected;
    const unsigned long COLOR_ORANGE_FRAME_SELECTED = Table::COLOR_ORANGE_WRITING_BODY_ISRTR;
    const unsigned long COLOR_BLUE_FRAME_UNSELECTED = Table::COLOR_BLUE_BACKGROUND_HEADER;
    const unsigned long COLOR_ORANGE_ARROW_SELECTED = Table::COLOR_ORANGE_WRITING_BODY_ISRTR;
    const unsigned long COLOR_ARROW_UNSELECTED = WHITE;
    const int FRAME_WIDTH = 3;

    Button (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor, unsigned long textColor, bool isSelected);
    void setTextOffset();
    void setTextOffsetX();
    void setTextOffsetY();
    void drawButton();
    virtual void printText();

public:
    Button (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor,byte offsetX, byte offsetY, string text, unsigned long textColor, bool isSelected);
    Button (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
            uint16_t sizeY, unsigned long backColor, string text, unsigned long textColor, bool isSelected);
    virtual ~Button();
    bool getStatus();
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