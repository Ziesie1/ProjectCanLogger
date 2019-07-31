#include "display/Button.hpp"


Button::Button(ILI9341& display, unsigned long startX, unsigned long startY, unsigned long sizeX, 
        unsigned long sizeY, unsigned long backColor, byte offsetX, byte offsetY, string text, unsigned long textColor,
        bool isSelected)
       :display{display}, startX{startX}, startY{startY}, sizeX{sizeX}, sizeY{sizeY}, backColor{backColor}, offsetX{offsetX}, offsetY{offsetY}, text{text}, textColor{textColor}, isSelected{false}
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
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,BLACK,2);
    this->isSelected = true;
}

void Button::resetFrame()
{
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,BLUE100,2);
    this->isSelected = false;
}

bool Button::getStatus()
{
    return this->isSelected;
}