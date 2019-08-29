#include "display/elements/Textzeile.hpp"
#include "utilities/utilities.hpp"

Textzeile::Textzeile(ILI9341& display, bool isSelected, uint8_t offsetXSpalte1,
                     uint8_t offsetXSpalte2, uint8_t offsetHeadline1,uint8_t offsetHeadline2,uint8_t offsetHeadline3,uint8_t zeilenhoehe)
        :display{display},isSelected{isSelected},offsetXSpalte1{offsetXSpalte1},offsetXSpalte2{offsetXSpalte2},offsetXHeadline1{offsetHeadline1},offsetXHeadline2{offsetHeadline2},offsetXHeadline3{offsetHeadline3},zeilenhoehe{zeilenhoehe}
{
    //screenBuffer_getMessageAtPosition(this->message,this->pos);
}

void Textzeile::printImportantContent(uint8_t posY,unsigned long Farbe)
{
    
    //erste Spalte mit Begrenzung
    String s1 = String(this->message.GetFullId(),HEX);
    AddBlanksToString(s1,8);
    s1.toUpperCase();
    this->display.printString(this->offsetXHeadline1,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
    this->display.drawVerticalLine(this->offsetXSpalte1,posY,posY+this->zeilenhoehe,this->COLOR_TABLE_LINE);

    //zweite Spalte mit Begrenzung
    s1 = String(this->message.GetTime(),HEX);
    AddBlanksToString(s1,4);
    s1.toUpperCase();
    this->display.printString(this->offsetXHeadline2,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
    this->display.drawVerticalLine(this->offsetXSpalte2,posY,posY+this->zeilenhoehe,this->COLOR_TABLE_LINE);

    //dritte Spalte 
    s1 = "";
    if(!this->message.GetRtr())
    {
        for(int i=0;i<this->message.GetCanLength();i++)
        {
            AddZerosToString(s1, this->message[i], this->message.maxDataVal, HEX);
            s1+=String(this->message.GetCanByte(i),HEX);
        }
        s1.toUpperCase();
    }
    else
    {
        s1 += "Laenge: ";
        s1 += String(this->message.GetCanLength());
    }
    AddBlanksToString(s1,16);
    this->display.printString(this->offsetXSpalte2+4,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
        
    
}
void Textzeile::printWholeContent()
{
    //hier wird detailview geöffnet
}
void Textzeile::selectZeile()
{
    this->isSelected = true;
}
void Textzeile::unselectZeile()
{
    this->isSelected = false;
}
bool Textzeile::isRtr()
{
    return this->message.GetRtr();
}

void Textzeile::setCanMsg(Canmsg& msg)
{
    this->message = msg;
}