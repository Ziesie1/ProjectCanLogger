#include "display/elements/Button.hpp"


Button::Button(ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
        uint16_t sizeY, unsigned long backColor, byte offsetX, byte offsetY, string text, unsigned long textColor,
        bool isSelected)
       :display{display}, startX{startX}, startY{startY}, sizeX{sizeX}, sizeY{sizeY}, backColor{backColor}, offsetX{offsetX}, offsetY{offsetY}, text{text}, textColor{textColor}, isSelected{false}
       {
           this->colorFrameSelected = this->display.makeColor(this->COLOR_FRAME_SELECTED[0],this->COLOR_FRAME_SELECTED[1],this->COLOR_FRAME_SELECTED[2]);
           this->colorFrameUnselected = this->display.makeColor(this->COLOR_FRAME_UNSELECTED[0],this->COLOR_FRAME_UNSELECTED[1],this->COLOR_FRAME_UNSELECTED[2]);
           this->colorArrowSelected = this->display.makeColor(this->COLOR_ARROW_SELECTED[0],this->COLOR_ARROW_SELECTED[1],this->COLOR_ARROW_SELECTED[2]);

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
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,this->colorFrameSelected,this->FRAME_WIDTH);
    
    
    
}
void Button::unselectButton()
{
    this->isSelected = false;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX,this->sizeY,this->colorFrameUnselected,this->FRAME_WIDTH);
    
    //Löschen des Pfeils
    //deleteArrow();
}

/*void Button::deleteArrow()
{
    int y = this->startY+(this->sizeY/4);
    for(int x=2;x<this->startX;x++)
    {
        this->display.drawOnePixel(x,y,this->COLOR_ARROW_UNSELECTED);
        if(x>2 && x%2 == 0)
        {
            y++;
        }
        
    }
    y =this->startY+(this->sizeY)*3/4;
    for(int x=2;x<this->startX;x++)
    {
        this->display.drawOnePixel(x,y,this->COLOR_ARROW_UNSELECTED);
        if(x>2 && x%2 == 0)
        {
            y--;
        }
    }
}
*/
/*void Button::drawArrow()
{
    int y = this->startY+(this->sizeY/4);
    for(int x=2;x<this->startX;x++)
    {
        this->display.drawOnePixel(x,y,this->colorArrowSelected);
        if(x>2 && x%2 == 0)
        {
            y++;
        }
        
    }
    y =this->startY+(this->sizeY)*3/4;
    for(int x=2;x<this->startX;x++)
    {
        this->display.drawOnePixel(x,y,this->colorArrowSelected);
        if(x>2 && x%2 == 0)
        {
            y--;
        }
    }
}
*/

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