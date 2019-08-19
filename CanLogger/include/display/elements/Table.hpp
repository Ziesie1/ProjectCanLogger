#ifndef TABLE_HPP
#define TABLE_HPP

#include "display/elements/Textzeile.hpp"
#include "display/ILI9341.hpp"
#include "display/screenBuffer.hpp"

class Table
{
    protected:
    ILI9341& display;
    uint8_t offsetXHeadlineStorage;
    const uint8_t OFFSETX_HEADLINE1 = 0;
    const uint8_t OFFSETX_SPALTE1 = 32; //vertikale Linie 1
    const uint8_t OFFSETX_HEADLINE2 = 34;
    const uint8_t OFFSETX_SPALTE2 = 80; //vertikale Linie 2
    const uint8_t OFFSETX_HEADLINE3 = 82;
    const uint8_t OFFSETX_HEADLINE_FREEZE = 180;
    const uint8_t OFFSETY_KOPFZEILE = 16;    //Ende Kopfzeilenbereich
    const uint8_t OFFSETY_SPALTENNAMEN = 32; //Horizontale Linie
    const uint8_t ZEILENHOEHE = 16; 
    const uint8_t DISPLAY_X = 240;
    const uint16_t DISPLAY_Y = 320;
    const unsigned long COLOR_TABLE_LINES = BLACK;
    const unsigned long COLOR_WRITING_HEADER = WHITE;
    const unsigned long COLOR_WRITING_BODY_DEFAULT = BLACK;
    const unsigned long COLOR_BACKGROUND_BODY = WHITE;
    const byte COLOR_BACKGROUND_HEADER[3] = {1,64,118};
    unsigned long colorBackgroundHeader;
    const byte COLOR_WRITING_BODY_ISRTR[3] = {237,125,49};
    unsigned long colorWritingBodyIsRtr;
    const byte COLOR_BACKGROUND_FREEZE[3] = {191,191,191};
    unsigned long colorBackgroundFreeze;
    const String HEADLINE_SPALTE1 = "Idf.";
    const String HEADLINE_SPALTE2 = "TimeS.";
    const String HEADLINE_SPALTE3 = "Data";
    const String HEADLINE_FREEZE = "Freeze";

    String kopfzeile;
    Textzeile** nachrichten = nullptr;
    int anzahlNachrichten;

    bool pausing;


    public:
    Table(ILI9341& display, String kopfzeile, Canmsg* canMessages, int anzahl, bool status = false);
    ~Table();
    void drawTableLines();
    void printTable(String speicherPfad);
    void printMessages();
    void setHeadlinePosition();
    void setBackroundHeader();
    void setPausingStatus(bool status);
    bool getPausingStatus();
    void loop();
    void updateHeadlineBackground();
    

};



#endif