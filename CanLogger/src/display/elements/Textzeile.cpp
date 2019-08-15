#include "display/elements/Textzeile.hpp"

Textzeile::Textzeile(ILI9341& display,Canmsg &msg, bool isSelected, uint8_t offsetXSpalte1,
                     uint8_t offsetXSpalte2, uint8_t offsetHeadline1,uint8_t offsetHeadline2,uint8_t offsetHeadline3,uint8_t zeilenhoehe)
        :display{display},message{&msg},isSelected{isSelected},offsetXSpalte1{offsetXSpalte1},offsetXSpalte2{offsetXSpalte2},offsetXHeadline1{offsetHeadline1},offsetXHeadline2{offsetHeadline2},offsetXHeadline3{offsetHeadline3},zeilenhoehe{zeilenhoehe}
{

}

void Textzeile::printImportantContent(uint8_t posY,unsigned long Farbe)
{
    if(this->message)
    {

        //erste Spalte mit Begrenzung
        String s1 = String(this->message->GetStdIdentifier(),HEX);
        this->display.printString(this->offsetXHeadline1,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
        this->display.drawVerticalLine(this->offsetXSpalte1,posY,posY+this->zeilenhoehe,this->COLOR_TABLE_LINE);

        //zweite Spalte mit Begrenzung
        s1 = String(this->message->GetTime(),HEX);
        this->display.printString(this->offsetXHeadline2,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
        this->display.drawVerticalLine(this->offsetXSpalte2,posY,posY+this->zeilenhoehe,this->COLOR_TABLE_LINE);

        //dritte Spalte 
        s1 = "";
        for(int i=0;i<this->message->GetCanLength();i++)
        {
            s1+=String(this->message->GetCanByte(i),HEX);
        }
        this->display.printString(this->offsetXSpalte2+4,posY,s1.c_str(),Farbe,this->COLOR_BACKGROUND_UNSELECTED);
        
    }
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