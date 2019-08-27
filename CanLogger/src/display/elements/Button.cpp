#include "display/elements/Button.hpp"


Button::Button(ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
        uint16_t sizeY, unsigned long backColor, byte offsetX, byte offsetY, string text, unsigned long textColor,
        bool isSelected)
       :display{display}, startX{startX}, startY{startY}, sizeX{sizeX}, sizeY{sizeY}, backColor{backColor}, offsetX{offsetX}, offsetY{offsetY}, text{text}, textColor{textColor}, isSelected{false}
       {
           drawButton();
           if(isSelected)
           {
               selectButton();
           }
       }
       

void Button::drawButton()
{
    this->display.drawFillRect(this->startX,this->startY,this->sizeX,this->sizeY,this->backColor);
    this->display.printString(this->startX+this->offsetX,this->startY+this->offsetY,this->text.c_str(),this->textColor,this->backColor,1);
}

bool Button::getStatus()
{
    return this->isSelected;
}

void Button::selectButton()
{
    this->isSelected = true;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,this->COLOR_ORANGE_FRAME_SELECTED,this->FRAME_WIDTH);
    
    
    
}
void Button::unselectButton()
{
    this->isSelected = false;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,this->COLOR_BLUE_FRAME_UNSELECTED,this->FRAME_WIDTH);
    
    
}



uint16_t Button::getPosX()
{
    return this->startX;
}
uint16_t Button::getSizeX()
{
    return this->sizeX;
}
uint16_t Button::getPosY()
{
    return this->startY;
}
uint16_t Button::getSizeY()
{
    return this->sizeY;
}