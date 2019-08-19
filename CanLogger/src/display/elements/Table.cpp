#include "display/elements/Table.hpp"

Table::Table(ILI9341& display, String kopfzeile, Canmsg* canMessages, int anzahl, bool status)
    :display{display},kopfzeile{kopfzeile},anzahlNachrichten{anzahl},pausing{status}
{
    this->nachrichten = new Textzeile*[this->anzahlNachrichten];

    //Ausgabenachrichten-Array wird erzeugt
    for(int idx = 0;idx<this->anzahlNachrichten;idx++)
    {
        nachrichten[idx] = new Textzeile(this->display,&canMessages[idx],false,this->OFFSETX_SPALTE1,this->OFFSETX_SPALTE2,this->OFFSETX_HEADLINE1,this->OFFSETX_HEADLINE2,this->OFFSETX_HEADLINE3,this->ZEILENHOEHE);
    }

    this->colorBackgroundFreeze = this->display.makeColor(this->COLOR_BACKGROUND_FREEZE[0],this->COLOR_BACKGROUND_FREEZE[1],this->COLOR_BACKGROUND_FREEZE[2]);
    this->colorWritingBodyIsRtr = this->display.makeColor(this->COLOR_WRITING_BODY_ISRTR[0],this->COLOR_WRITING_BODY_ISRTR[1],this->COLOR_WRITING_BODY_ISRTR[2]);
    this->colorBackgroundHeader = this->display.makeColor(this->COLOR_BACKGROUND_HEADER[0],this->COLOR_BACKGROUND_HEADER[1],this->COLOR_BACKGROUND_HEADER[2]);

    setHeadlinePosition();
    printTable(this->kopfzeile);
    drawTableLines();
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
    this->display.drawHorizontalLine(0,this->DISPLAY_X,this->OFFSETY_SPALTENNAMEN,this->COLOR_TABLE_LINES);
    this->display.drawHorizontalLine(0,this->DISPLAY_X,this->OFFSETY_KOPFZEILE,this->COLOR_TABLE_LINES);
    
}

void Table::printTable(String speicherPfad)
{
    setBackroundHeader();

    this->display.printString(this->offsetXHeadlineStorage,0,speicherPfad.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
    this->display.printString(this->OFFSETX_HEADLINE1,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE1.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE1,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
    this->display.printString(this->OFFSETX_HEADLINE2,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE2.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE2,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
    this->display.printString(this->OFFSETX_HEADLINE3,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE3.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);

    drawTableLines();


}

void Table::printMessages()
{
    uint8_t posY = this->OFFSETY_SPALTENNAMEN+2;
    for(int idx = 0;idx<this->anzahlNachrichten;idx++)
    {
        this->nachrichten[idx]->printImportantContent(posY,this->COLOR_WRITING_BODY_DEFAULT);
        posY = posY+this->ZEILENHOEHE;
    }
    
}

void Table::setHeadlinePosition()
{
    this->offsetXHeadlineStorage = (this->DISPLAY_X-(this->kopfzeile.length()*8))/2;
    
    
}

void Table::setBackroundHeader()
{
    this->display.drawFillRect(0,0,this->DISPLAY_X,this->OFFSETY_SPALTENNAMEN,this->colorBackgroundHeader);
}

void Table::setPausingStatus(bool status)
{
    this->pausing = status;
}

bool Table::getPausingStatus()
{
    return this->pausing;
}

void Table::loop()
{
    if(pausing)
    {
        screenBuffer_disableUpdate();
    }
    else
    {
        screenBuffer_enableUpdate();
    }
    
}

void Table::updateHeadlineBackground()
{
    if(pausing)
    {
        this->display.drawFillRect(0,this->OFFSETY_KOPFZEILE,this->DISPLAY_X,16,this->colorBackgroundFreeze);
        this->display.printString(this->OFFSETX_HEADLINE_FREEZE,this->OFFSETY_KOPFZEILE,this->HEADLINE_FREEZE.c_str(),this->colorWritingBodyIsRtr,this->colorBackgroundFreeze,1);
        this->display.printString(this->OFFSETX_HEADLINE1,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE1.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundFreeze,1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE1,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE2,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE2.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundFreeze,1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE2,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE3,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE3.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundFreeze,1);
        this->display.drawHorizontalLine(0,this->DISPLAY_X,this->OFFSETY_KOPFZEILE,this->COLOR_TABLE_LINES);
    }
    else
    {
        this->display.drawFillRect(0,this->OFFSETY_KOPFZEILE,this->DISPLAY_X,16,this->colorBackgroundHeader);
        this->display.printString(this->OFFSETX_HEADLINE1,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE1.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE1,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE2,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE2.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE2,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE3,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE3.c_str(),this->COLOR_WRITING_HEADER,this->colorBackgroundHeader,1);
        this->display.drawHorizontalLine(0,this->DISPLAY_X,this->OFFSETY_KOPFZEILE,this->COLOR_TABLE_LINES);
    }
    
 
}
