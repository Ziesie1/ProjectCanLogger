#ifndef KOPFZEILE_HPP
#define KOPFZEILE_HPP

#include "display/elements/Table.hpp"

class Kopfzeile
{
    protected:
    ILI9341& display;
    const uint8_t OFFSET_Y_HEADLINE = 16;
    const uint8_t DISPLAY_X = 240;
    const long COLOR_BLUE_BACKGROUND = 0x007200;
    const long COLOR_WHITE_WRITING_HEADER = WHITE;
    const long COLOR_DISCARD_MASSAGE_COUNTER = RED100;
    uint8_t offsetXHeadline;
    String headline;

    void setDiscardMessagesCounter();

    public:
    Kopfzeile(ILI9341& display, String headline);
    void setHeadlinePosition();
    void setBackroundHeader();
    void loop();
    
};


#endif