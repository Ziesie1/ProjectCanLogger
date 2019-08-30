#include "display/elements/Arrow.hpp"
/*
    Constructor of the Class Arrow
    input:  display         - reference of the display were the Arrow is shown
            button          - pointer of the highlighted button
            colorSelected   - collor of the arrow, if the button is selected
            colorSelected   - collor of the arrow, if the button is unselected
            isSelected      - selection status of the button, if the button is selectet isSelecetd = true, if not isSelecetd = false

*/
Arrow::Arrow(ILI9341& display, Button* button, unsigned long colorSelected, unsigned long colorUnselected,bool isSelecetd)
    :display{display}, button{button}, colorArrowSelected{colorSelected}, colorArrowUnselected{colorUnselected}, isSelected{isSelected}
{
    setPositions();
    if(isSelecetd)
    {
        draw(this->colorArrowSelected);
    }
}
/*
    This metode prints or deletes the arrow on the screen.
    input:  Farbe   - collor of the arrow, if its this->colorArrowUnselected the arrow will be disapper, else it will be printed in the selectet collor
*/
void Arrow::draw(unsigned long Farbe)
{
    this->display.printString(posX1, posY1, ">", Farbe, WHITE, this->ARROWWIDTH);
}
/*
    This metode sets the positon of the arrow 9 pixels left from the highlighted button
*/
void Arrow::setPositions()
{
    this->posX1 = this->button->getPosX() - 9; //-9 Damit nicht in den Button geschrieben wird
    this->posY1 = this->button->getPosY() + (this->button->getSizeY() * (1 / 4));
}