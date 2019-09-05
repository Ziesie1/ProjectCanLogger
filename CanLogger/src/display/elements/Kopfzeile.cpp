#include "display/elements/Kopfzeile.hpp"


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
}