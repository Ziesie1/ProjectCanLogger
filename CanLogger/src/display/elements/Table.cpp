#include "display/elements/Table.hpp"

/*
    Constructor of the class Table, nitialize an array of "Textzeile" and prints the whole gui on the display
    Input: display          - Reference of the display where the "Table" will be printed on,
           kopfzeile        - leter of the first line, file path if "speichern" is selected else "Ohne Speichern"
           anzahl           - number of printed messages
           status           - pausing status, if it´s true the display will go in freze mode and the printet messages stay constant

*/
Table::Table(ILI9341& display, int anzahl, bool status)
    :display{display}, anzahlNachrichten{anzahl}, pausing{status}
{
    this->nachrichten = new Textzeile * [SCREEN_BUFFER_SIZE];

    //Textzeilenarray maximaler Groeße 
    for(int idx = 0; idx < SCREEN_BUFFER_SIZE; idx++)
    {    
        nachrichten[idx] = new Textzeile(this->display, false, this->OFFSETX_SPALTE1, this->OFFSETX_SPALTE2, this->OFFSETX_HEADLINE1, this->OFFSETX_HEADLINE2, this->OFFSETX_HEADLINE3, this->ZEILENHOEHE);
    }

    //Canmsg der Textzeilen initialisieren
    Canmsg dummi;
    for(int idx = 0; idx<screenBuffer_getFillLevel(); idx++)
    {
        screenBuffer_getMessageAtPosition(dummi,idx);
        nachrichten[idx]->setCanMsg(dummi);
    }

    this->printTable();
    this->printMessages();   
}

/*
    The destructor of the class Table calls the destructors of all "Textzeilen" and the array "nachrichten".
*/
Table::~Table()
{
    if(this->nachrichten)
    {
        for(int idx = 0; idx<SCREEN_BUFFER_SIZE; idx++)
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

/*
    Drawing the horizontal and vertical lines of the tabel.
*/
void Table::drawTableLines()
{
    this->display.drawVerticalLine(this->OFFSETX_SPALTE1, this->OFFSETY_KOPFZEILE, this->DISPLAY_Y, this->COLOR_TABLE_LINES);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE2, this->OFFSETY_KOPFZEILE, this->DISPLAY_Y, this->COLOR_TABLE_LINES);
    this->display.drawHorizontalLine(0, this->DISPLAY_X, this->OFFSETY_SPALTENNAMEN, this->COLOR_TABLE_LINES);
    this->display.drawHorizontalLine(0, this->DISPLAY_X, this->OFFSETY_KOPFZEILE, this->COLOR_TABLE_LINES);  
}

/*
    This methode printsthe whol table, fillis the headllines and calls "drawTableLines".
    input: speicherPfad - leter of the first line, "file path" if "speichern" is selected else "Ohne Speichern"
*/
void Table::printTable()
{
    this->display.drawFillRect(0,this->OFFSETY_KOPFZEILE,this->DISPLAY_X,this->OFFSETY_KOPFZEILE,this->COLOR_BLUE_BACKGROUND_HEADER);
    this->display.printString(this->OFFSETX_HEADLINE1,this->OFFSETY_KOPFZEILE,this->HEADLINE_SPALTE1.c_str(),this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE1, this->OFFSETY_KOPFZEILE, this->OFFSETY_SPALTENNAMEN,BLACK);
    this->display.printString(this->OFFSETX_HEADLINE2, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE2.c_str(),this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);
    this->display.drawVerticalLine(this->OFFSETX_SPALTE2, this->OFFSETY_KOPFZEILE, this->OFFSETY_SPALTENNAMEN,BLACK);
    this->display.printString(this->OFFSETX_HEADLINE3, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE3.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);

    drawTableLines();
}

/*
    Printing the messages from "nachrichten" in to the right lines, by hightlighting the RTR-bits.
*/
void Table::printMessages()
{
    uint8_t posY = this->OFFSETY_SPALTENNAMEN+2;
    
    for(int idx = 0; idx<screenBuffer_getFillLevel(); idx++)
    {   
        this->nachrichten[idx]->printImportantContent(posY);
        posY = posY + this->ZEILENHOEHE;
    }
}



/*
    Setting the pausinng status for freze mode.
    input: status   - if it´s true the display will go in freze mode, if it´s false the display will be updated normal 
*/
void Table::setPausingStatus(bool status)
{
    this->pausing = status;
}

/*
    request for the PausingStatus
    return: pausing - if it´s true the display is infreze mode, if it´s false the display gets normal updates
*/
bool Table::getPausingStatus()
{
    return this->pausing;
}

/*
    Loop for updating the screen. Seperation between freze and normal mode. 
*/
void Table::loop()
{
    if(pausing)
    {
        screenBuffer_disableUpdate();
    }
    else
    {
        screenBuffer_enableUpdate();

        //check for Updates in screenBuffer, if updates are deteced the table will be updated
        if(screenBuffer_hasSomethingChanged())
        {
            updateMessages();
        }
    }
}
/*
    Updating the titelline background adiccted to the pausing status
*/
void Table::updateHeadlineBackground()
{
    if(pausing)
    {
        this->display.drawFillRect(0, this->OFFSETY_KOPFZEILE, this->DISPLAY_X, 16, this->COLOR_GREY_BACKGROUND_FREEZE);
        this->display.printString(this->OFFSETX_HEADLINE_FREEZE, this->OFFSETY_KOPFZEILE, this->HEADLINE_FREEZE.c_str(), this->COLOR_ORANGE_WRITING_BODY_ISRTR, this->COLOR_GREY_BACKGROUND_FREEZE,1);
        this->display.printString(this->OFFSETX_HEADLINE1, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE1.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_GREY_BACKGROUND_FREEZE, 1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE1, this->OFFSETY_KOPFZEILE, this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE2, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE2.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_GREY_BACKGROUND_FREEZE, 1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE2,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE3, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE3.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_GREY_BACKGROUND_FREEZE, 1);
        this->display.drawHorizontalLine(0, this->DISPLAY_X, this->OFFSETY_KOPFZEILE, this->COLOR_TABLE_LINES);
    }
    else
    {
        this->display.drawFillRect(0, this->OFFSETY_KOPFZEILE, this->DISPLAY_X, 16, this->COLOR_BLUE_BACKGROUND_HEADER);
        this->display.printString(this->OFFSETX_HEADLINE1, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE1.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE1,this->OFFSETY_KOPFZEILE,this->OFFSETY_SPALTENNAMEN,BLACK);
        this->display.printString(this->OFFSETX_HEADLINE2, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE2.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);
        this->display.drawVerticalLine(this->OFFSETX_SPALTE2, this->OFFSETY_KOPFZEILE, this->OFFSETY_SPALTENNAMEN, BLACK);
        this->display.printString(this->OFFSETX_HEADLINE3, this->OFFSETY_KOPFZEILE, this->HEADLINE_SPALTE3.c_str(), this->COLOR_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND_HEADER, 1);
        this->display.drawHorizontalLine(0, this->DISPLAY_X, this->OFFSETY_KOPFZEILE, this->COLOR_TABLE_LINES);
    }
}

/*
    Updating nachrichten and printin new messages by calling printSingleMessage.
*/
void Table::updateMessages()
{
    this->anzahlNachrichten = screenBuffer_getFillLevel();

    Canmsg dummi;
    //neue Nachrichten aufnehmen        
    for(int idx = 0; idx<this->anzahlNachrichten; idx++)
    {
        if(screenBuffer_hasThisMessageChanged(idx))
        {
            screenBuffer_getMessageAtPosition(dummi, idx);
            this->nachrichten[idx]->setCanMsg(dummi);
            printSingleMessage(idx);

        }
    } 
}

/*
    Printing singel "textzeile" of "nachrichten".
    input:  pos     - positin in "nachrichten"
            collor  - collor for printing
*/
void Table::printSingleMessage(int pos)
{
    uint8_t startOffsetY = this->OFFSETY_SPALTENNAMEN + 2;
    uint8_t posYMessage = (pos*this->ZEILENHOEHE) + startOffsetY;
    this->nachrichten[pos]->printImportantContent(posYMessage);
}
