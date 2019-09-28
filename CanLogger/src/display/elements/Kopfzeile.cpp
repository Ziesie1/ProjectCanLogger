#include "display/elements/Kopfzeile.hpp"

/*
    Constructor of the class Kopfzeile
    input: display - reference to the used display-object
           headline - text, which is supposed to be shown as the headline
    output: -
*/
Kopfzeile::Kopfzeile(ILI9341& display, String headline)
    :display{display}, headline{headline}
{
    setHeadlinePosition();
    setBackroundHeader();
    this->display.printString(this->offsetXHeadline, 0, headline.c_str(), this->COLOR_WHITE_WRITING_HEADER, this->COLOR_BLUE_BACKGROUND, 1);

}

/*
    Setting the startpsition for the headline input. The text has to be printed in the middel of the line. 
*/
void Kopfzeile::setHeadlinePosition()
{
    this->offsetXHeadline = (this->DISPLAY_X - (this->headline.length() * 8)) / 2;
}

/*
    Drawing the Header Background.
*/
void Kopfzeile::setBackroundHeader()
{
    this->display.drawFillRect(0, 0, this->DISPLAY_X, this->OFFSET_Y_HEADLINE, this->COLOR_BLUE_BACKGROUND);
    this->display.drawHorizontalLine(0,this->DISPLAY_X,this->OFFSET_Y_HEADLINE,BLACK);
}

/* 
    updates Kopfzeile on the screen.
*/
void Kopfzeile::loop()
{
    if(CanUtility_whereNewMessagesDiscarded())
    {
        this->setDiscardMessagesCounter();
    }
}

/* 
    Prints in the upper right-hand corner an counter of the discard messanges.
*/
void Kopfzeile::setDiscardMessagesCounter()
{
    String counter = "( " + String(CanUtility_howManyMessagesWhereDiscarded(), DEC) + " )";
    this->display.printString(0, 0, counter.c_str(), this->COLOR_DISCARD_MASSAGE_COUNTER, this->COLOR_BLUE_BACKGROUND, 1);
}

