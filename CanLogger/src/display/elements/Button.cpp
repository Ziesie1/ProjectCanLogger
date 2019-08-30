#include "display/elements/Button.hpp"

/*
    Constructor of the class Button
    input:  display - reference of the display where the Button will be printed
            startx  - start position on x-axis
            starty  - start position on y-axis
            sizeX   - button width
*/
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
       
/*
    This methode prints the Button.
*/
void Button::drawButton()
{
    this->display.drawFillRect(this->startX, this->startY, this->sizeX, this->sizeY, this->backColor);
    this->display.printString(this->startX + this->offsetX, this->startY + this->offsetY, this->text.c_str(), this->textColor, this->backColor, 1);
}
/*
    Request of the button status 
    return: true    - buttin is selected
            false   - bunnton is not selected
*/
bool Button::getStatus()
{
    return this->isSelected;
}
/*
    Set the button status to selected
*/
void Button::selectButton()
{
    this->isSelected = true;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_ORANGE_FRAME_SELECTED, this->FRAME_WIDTH);  
}
/*
    Set the button status to unselected
*/
void Button::unselectButton()
{
    this->isSelected = false;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_BLUE_FRAME_UNSELECTED, this->FRAME_WIDTH); 
}


/*
    request for the positin on x-axis
    return: startX   - start positin on x-axis
*/
uint16_t Button::getPosX()
{
    return this->startX;
}
/*
    request for the buttun width
    return: sizeX   - buttun width
*/
uint16_t Button::getSizeX()
{
    return this->sizeX;
}
/*
    request for the positin on y-axis
    return: startY   - start positin on y-axis
*/
uint16_t Button::getPosY()
{
    return this->startY;
}
/*
    request for the button hight
    return: sizeY   - buttun hight
*/
uint16_t Button::getSizeY()
{
    return this->sizeY;
}