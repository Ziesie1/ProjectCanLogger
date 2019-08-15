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
    this->display.printString(posX1,posY1,">",Farbe,WHITE,this->ARROWWIDTH);
}

void Arrow::setPositions()
{
    this->posX1 = this->button->getPosX()-9; //-9 Damit nicht in den Button geschrieben wird
    this->posY1 = this->button->getPosY()+(this->button->getSizeY()*1/4);
}