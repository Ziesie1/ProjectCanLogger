#ifndef TEXTZEILE_HPP
#define TEXTZEILE_HPP

#include "display/ILI9341.hpp"
#include "can/Canmsg.hpp"
#include "display/screenBuffer.hpp"

/*  
    Die Klasse Textzeile wird verwendet, um CAN-Nachrichten auf dem Display darzustellen.
    Dementsprechend sind die Instanzen der Klasse Textzeile wichtige Bestandteile der Klasse Table.
*/
class Textzeile
{
    protected:
        ILI9341& display;
        Canmsg message;
        bool isSelected;
        const unsigned long COLOR_BACKGROUND_SELECTED = RED100;
        const unsigned long COLOR_BACKGROUND_UNSELECTED = WHITE;
        const unsigned long COLOR_TABLE_LINE = BLACK;
        uint8_t offsetXSpalte1; 
        uint8_t offsetXSpalte2;   
        uint8_t offsetXHeadline1;
        uint8_t offsetXHeadline2;
        uint8_t offsetXHeadline3;
        uint8_t zeilenhoehe;



    public:
        Textzeile(ILI9341& display, bool isSelected, uint8_t offsetXSpalte1, 
                  uint8_t offsetXSpalte2, uint8_t offsetHeadline1, uint8_t offsetHeadline2, 
                  uint8_t offsetHeadline3, uint8_t zeilenhoehe);
        void printImportantContent(uint8_t posY, unsigned long farbe);
        void selectZeile();
        void unselectZeile();
        bool isRtr();
        void setCanMsg(Canmsg& msg);
};



#endif