#ifndef TABLE_HPP
#define TABLE_HPP

#include"display/elements/Textzeile.hpp"
#include"display/ILI9341.hpp"

class Table
{
    protected:
    ILI9341& display;
    const uint8_t OFFSETX_HEADLINE_STORAGE = 60;
    const uint8_t OFFSETX_HEADLINE1 = 10;
    const uint8_t OFFSETX_SPALTE1 = 26;
    const uint8_t OFFSETX_HEADLINE2 = 36;
    const uint8_t OFFSETX_SPALTE2 = 76;
    const uint8_t OFFSETX_HEADLINE3 = 86;
    const uint8_t OFFSETY_KOPFZEILE = 40;    //Ende Kopfzeilenbereich
    const uint8_t OFFSETY_SPALTENNAMEN = 60; //Horizontale Linie
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