#include "display/elements/Arrow.hpp"

Arrow::Arrow(ILI9341& display, Button button, long posX1, long posY1, long posX2, long , long posX3, long posY3)
    :display{display},button{button},posX1{posX1},posY1{posY1},posX2{posX2},posY2{posY3},posX3{posX3},posY3{posY3}
{

}

void Arrow::draw(Color Farbe)
{
    this->display.drawLine(this->posX1,this->posY1,this->posX3,this->posY3,Farbe);
    this->display.drawLine(this->posX2,this->posY2,this->posX3,this->posY3,Farbe);
}