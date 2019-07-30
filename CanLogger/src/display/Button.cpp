#include "display/Button.hpp"


Button::Button(unsigned long startX, unsigned long startY, unsigned long sizeX, 
       unsigned long sizeY, unsigned long backColor, byte offsetX, byte offsetY, string text, unsigned long textColor,
       ILI9341& display, bool isSelected)
       :startX{startX}, startY{startY}, sizeX{sizeX}, sizeY{sizeY}, backColor{backColor}, offsetX{offsetX}, offsetY{offsetY}, text{text}, textColor{textColor}, display{display}, isSelected{false}
       {
           drawButton();
       }
       

void Button::drawButton()
{
    this->display.drawFillRect(this->startX,this->startY,this->sizeX,this->sizeY,this->backColor);
    this->display.printString(this->startX+this->offsetX,this->startY+this->offsetY,this->text.c_str(),this->textColor,this->backColor,1);
}

void Button::setFrame()
{
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,BLACK,10);
}

void Button::resetFrame()
{
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,BLUE100,10);
}

bool Button::getStatus()
{
    return this->isSelected;
}