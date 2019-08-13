#include "display/elements/Arrow.hpp"

Arrow::Arrow(ILI9341& display, Button* button, unsigned long colorSelected, unsigned long colorUnselected,bool isSelecetd)
    :display{display},button{button},colorArrowSelected{colorSelected},colorArrowUnselected{colorUnselected},isSelected{isSelected}
{
    setPositions();
    if(isSelecetd)
    {
        draw(this->colorArrowSelected);
    }
}

void Arrow::draw(unsigned long Farbe)
{
    //this->display.drawLine(this->posX1,this->posY1,this->posX3,this->posY3,Farbe);
    //this->display.drawLine(this->posX2,this->posY2,this->posX3,this->posY3,Farbe);
    this->display.printString(posX1,posX2,">",Farbe,WHITE,1);
}

void Arrow::setPositions()
{
    this->posX1 = this->button->getPosX()-15;
    this->posY1 = this->button->getPosY()+(this->button->getSizeY()*1/4);
    this->posX2 = this->posX1;
    this->posY2 = this->button->getPosY()+(this->button->getSizeY()*3/4);
    this->posX3 = this->button->getPosX()-1;//-1, damit kein weißer Punkt im Button entsteht
    this->posY3 = this->button->getPosY()+(this->button->getSizeY()*1/2);

}