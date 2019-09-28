#include "display/elements/Textzeile.hpp"
#include "utilities/utilities.hpp"
#include "display/elements/Table.hpp"

/*
    Constructor of the class Textzeile
    input: display          - Reference of the display where the "Textzeile" will be printed on,
           isSelected       - selection status, referring to the functionality of scrollingthis attribut becomes important
           offsetXSpalte1   - X offset of the tables first vertical gridline
           offsetXSpalte2   - X offset of the tables second vertical gridline
           offsetHeadline1  - X offset of the Headline "Idf."  
           offsetHeadline2  - X offset of the Headline "Time" 
           offsetHeadline3  - X offset of the Headline "Data"
           zeilenhoehe      - height of each "Textzeile" when printed 

*/
Textzeile::Textzeile(ILI9341& display, bool isSelected, uint8_t offsetXSpalte1,
                     uint8_t offsetXSpalte2, uint8_t offsetHeadline1, uint8_t offsetHeadline2, 
                     uint8_t offsetHeadline3, uint8_t zeilenhoehe)
        :display{display},isSelected{isSelected},offsetXSpalte1{offsetXSpalte1},offsetXSpalte2{offsetXSpalte2},offsetXHeadline1{offsetHeadline1},offsetXHeadline2{offsetHeadline2},offsetXHeadline3{offsetHeadline3},zeilenhoehe{zeilenhoehe}
{
    
}

/*
    This methode prints the most important content of the CAN-messages of the frontend buffer into the logtable shown on the display
    input: posY     - Y coordinate of the printing start 
*/
void Textzeile::printImportantContent(uint16_t posY)
{
    unsigned long farbe = Table::COLOR_WRITING_BODY_DEFAULT;
    if(this->message.GetRtr())
    {
        farbe = Table::COLOR_ORANGE_WRITING_BODY_ISRTR;
    }

    //erste Spalte mit Begrenzung
    String s1 = String(this->message.GetFullId(),HEX);
    AddBlanksToString(s1, 8);
    s1.toUpperCase();
    this->display.printString(this->offsetXHeadline1, posY, s1.c_str(), farbe, this->COLOR_BACKGROUND_UNSELECTED);
    this->display.drawVerticalLine(this->offsetXSpalte1, posY, posY + this->zeilenhoehe, this->COLOR_TABLE_LINE);

    //zweite Spalte mit Begrenzung
    s1 = String(this->message.GetTime(),HEX);
    AddBlanksToString(s1, 4);
    s1.toUpperCase();
    this->display.printString(this->offsetXHeadline2, posY, s1.c_str(), farbe,this->COLOR_BACKGROUND_UNSELECTED);
    this->display.drawVerticalLine(this->offsetXSpalte2, posY, posY + this->zeilenhoehe, this->COLOR_TABLE_LINE);

    //dritte Spalte 
    s1 = "";
    if(!this->message.GetRtr())
    {
        for(int i = 0; i<this->message.GetCanLength(); i++)
        {
            AddZerosToString(s1, this->message[i], this->message.maxDataVal,HEX);
            s1 += String(this->message.GetCanByte(i),HEX);
        }
        s1.toUpperCase();
        AddBlanksToString(s1,16);

        for(unsigned int i = 0; i < s1.length(); i+=2)
        {
            String twoChars = "";
            twoChars += s1[i];
            twoChars += s1[i+1];

            if(i % 4 == 0)
            {
                farbe = Table::COLOR_WRITING_BODY_DEFAULT;
            }else{
                farbe = Table::COLOR_WRITING_BODY_NEXT_BYTE;
            }

            this->display.printString(this->offsetXSpalte2 + 4 + i*8, posY, twoChars.c_str(), farbe, this->COLOR_BACKGROUND_UNSELECTED);
        }
    }
    else
    {
        s1 += "Laenge: ";
        s1 += String(this->message.GetCanLength());
        AddBlanksToString(s1,16);
        this->display.printString(this->offsetXSpalte2 + 4, posY, s1.c_str(), farbe, this->COLOR_BACKGROUND_UNSELECTED);
    }
    
}

/*
    This methode changes the selection status to true
*/
void Textzeile::selectZeile()
{
    this->isSelected = true;
}

/*
    This methode changes the selection status to false
*/
void Textzeile::unselectZeile()
{
    this->isSelected = false;
}

/*
    This methode returns the RTR status of the illustrazed CAN-message
    return: true    - CAN-message is a RTR-message
            false   - CAN-message isn't a RTR-message
*/
bool Textzeile::isRtr()
{
    return this->message.GetRtr();
}

/*
    This methode sets CAN-message of the "Textzeile"
    input: msg  - reference to a CAN-message of the frontend buffer
*/
void Textzeile::setCanMsg(Canmsg& msg)
{
    this->message = msg;
}