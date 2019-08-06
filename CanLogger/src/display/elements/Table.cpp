#include "display/elements/Table.hpp"

Table::Table(ILI9341& display, String kopfzeile, Canmsg canMessages[], int anzahl)
    :display{display},kopfzeile{kopfzeile},anzahlNachrichten{anzahl}
{
    this->nachrichten = new Textzeile*[this->anzahlNachrichten];

    //Ausgabenachrichten-Array wird erzeugt
    for(int idx = 0;idx<this->anzahlNachrichten;idx++)
    {
        nachrichten[idx] = new Textzeile(this->display,canMessages[idx],false,this->OFFSETX_SPALTE1,this->OFFSETX_SPALTE2,this->OFFSETX_HEADLINE1,this->OFFSETX_HEADLINE2,this->OFFSETX_HEADLINE3,this->ZEILENHOEHE);
    }

    drawTableLines();
    printHeadlines(this->kopfzeile);
    printMessages();
}

Table::~Table()
{
    if(this->nachrichten)
    {
        for(int idx=0;idx<this->anzahlNachrichten;idx++)
        {
            if(this->nachrichten[idx])
            {
                delete this->nachrichten[idx];
                this->nachrichten[idx] = nullptr;
            }
            
        }
        delete[] this->nachrichten;
        this->nachrichten = nullptr;
    }
    
}

void Table::drawTableLines()
{
    this->display.drawVerticalLine(this->OFFSETX_SPALTE1,this->OFFSETY_KOPFZEILE,this->DISPLAY_Y,this->COLOR_TABLE_LINES);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE2,this->OFFSETY_KOPFZEILE,this->DISPLAY_Y,this->COLOR_TABLE_LINES);
    this->display.drawHorizontalLine(0,240,this->OFFSETY_SPALTENNAMEN,this->COLOR_TABLE_LINES);
    
}

void Table::printHeadlines(String speicherPfad)
{
    this->display.printString(this->OFFSETX_HEADLINE_STORAGE,0,speicherPfad.c_str(),this->COLOR_WRITING,this->COLOR_BACKGROUND,1);
    this->display.printString(this->OFFSETX_HEADLINE1,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE1.c_str(),this->COLOR_WRITING,this->COLOR_BACKGROUND,1);
    this->display.printString(this->OFFSETX_HEADLINE2,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE2.c_str(),this->COLOR_WRITING,this->COLOR_BACKGROUND,1);
    this->display.printString(this->OFFSETX_HEADLINE3,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE3.c_str(),this->COLOR_WRITING,this->COLOR_BACKGROUND,1);
}

void Table::printMessages()
{
    uint8_t posY = this->OFFSETY_SPALTENNAMEN+2;
    for(int idx = 0;idx<this->anzahlNachrichten;idx++)
    {
        this->nachrichten[idx]->printImportantContent(posY);
        posY = posY+this->ZEILENHOEHE;
    }
    
}