#ifndef TABLE_HPP
#define TABLE_HPP

#include"display/elements/Textzeile.hpp"
#include"display/ILI9341.hpp"

class Table
{
    protected:
    ILI9341& display;
    const uint8_t OFFSETX_HEADLINE_STORAGE = 60;
    const uint8_t OFFSETX_HEADLINE1 = 2;
    const uint8_t OFFSETX_SPALTE1 = 28; //vertikale Linie 1
    const uint8_t OFFSETX_HEADLINE2 = 32;
    const uint8_t OFFSETX_SPALTE2 = 76; //vertikale Linie 2
    const uint8_t OFFSETX_HEADLINE3 = 140;
    const uint8_t OFFSETY_KOPFZEILE = 40;    //Ende Kopfzeilenbereich
    const uint8_t OFFSETY_SPALTENNAMEN = 58;
    const uint8_t ZEILENHOEHE = 20; //Horizontale Linie
    const uint8_t DISPLAY_X = 240;
    const uint16_t DISPLAY_Y = 320;
    const Color COLOR_TABLE_LINES = BLACK;
    const Color COLOR_WRITING = BLACK;
    const Color COLOR_BACKGROUND = WHITE;
    const String HEADLINE_SPALTE1 = "Idf";
    const String HEADLINE_SPALTE2 = "TimeS";
    const String HEADLINE_SPALTE3 = "Data";

    String kopfzeile;
    Textzeile** nachrichten;
    int anzahlNachrichten;


    public:
    Table(ILI9341& display, String kopfzeile, Canmsg canMessages[], int anzahl);
    ~Table();
    void drawTableLines();
    void printHeadlines(String speicherPfad);
    void printMessages();
    

};



#endif