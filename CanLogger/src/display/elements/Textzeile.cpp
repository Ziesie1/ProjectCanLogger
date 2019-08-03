#include "display/elements/Textzeile.hpp"

Textzeile::Textzeile(ILI9341& display, Canmsg msg, bool isSelected)
        :display{display},message{msg},isSelected{isSelected}
{

}
void Textzeile::printImportantContent(uint8_t posY)
{
    //erste Spalte
    String s1 = String(this->message.GetStdIdentifier(),HEX);
    this->display.printString(this->OFFSETX1,posY,s1.c_str(),this->COLOR_FONT,this->COLOR_BACKGROUND_UNSELECTED);

    //zweite Spalte
    s1 = String(this->message.GetTime(),HEX);
    this->display.printString(this->OFFSETX2,posY,s1.c_str(),this->COLOR_FONT,this->COLOR_BACKGROUND_UNSELECTED);

    //dritte Spalte
    s1 = "";
    for(int i=0;i<this->message.GetCanLength();i++)
    {
        s1+=String(this->message.GetCanByte(i),HEX);
    }
    this->display.printString(this->OFFSETX3,posY,s1.c_str(),this->COLOR_FONT,this->COLOR_BACKGROUND_UNSELECTED);

}
void Textzeile::printWholeContent()
{
    
}
void Textzeile::selectZeile()
{
    this->isSelected = true;
}
void Textzeile::unselectZeile()
{
    this->isSelected = false;
}