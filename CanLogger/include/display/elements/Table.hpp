#ifndef TABLE_HPP
#define TABLE_HPP

#include "display/elements/Textzeile.hpp"
#include "display/ILI9341.hpp"
#include "display/screenBuffer.hpp"
#include "can/CanUtility.hpp"
/*
    This Class creates an inteface where the "Textzeilen", the log file and other information can be printed in a Table.
*/
class Table
{
protected:
    ILI9341& display;
    const uint8_t OFFSETX_HEADLINE1 = 0;
    const uint8_t OFFSETX_SPALTE1 = 64; //vertikale Linie 1
    const uint8_t OFFSETX_HEADLINE2 = 66;
    const uint8_t OFFSETX_SPALTE2 = 104; //vertikale Linie 2
    const uint8_t OFFSETX_HEADLINE3 = 106;
    const uint8_t OFFSETX_HEADLINE_FREEZE = 180;
    const uint8_t OFFSETY_KOPFZEILE = 16;    //Ende Kopfzeilenbereich
    const uint8_t OFFSETY_SPALTENNAMEN = 32; //Horizontale Linie
    const uint8_t ZEILENHOEHE = 16; 
    const uint8_t DISPLAY_X = 240;
    const uint16_t DISPLAY_Y = 320;
    
    const String HEADLINE_SPALTE1 = "Idf.";
    const String HEADLINE_SPALTE2 = "Time";
    const String HEADLINE_SPALTE3 = "Data";
    const String HEADLINE_FREEZE = "Freeze";
    Textzeile** nachrichten = nullptr;
    int anzahlNachrichten;
    bool pausing;


public:
    static constexpr unsigned long COLOR_TABLE_LINES = BLACK;
    static constexpr unsigned long COLOR_WRITING_HEADER = WHITE;
    static constexpr unsigned long COLOR_WRITING_BODY_DEFAULT = BLACK;
    static constexpr unsigned long COLOR_BACKGROUND_BODY = WHITE;
    static constexpr unsigned long COLOR_BLUE_BACKGROUND_HEADER = 0x007200; //R,G,B = 1,64,118
    static constexpr unsigned long COLOR_WRITING_BODY_NEXT_BYTE = 0x007BEF; // R,G,B = 127,127,127
    static constexpr unsigned long COLOR_ORANGE_WRITING_BODY_ISRTR = 0x0033FD; //R,G,B = 237,125,49
    static constexpr unsigned long COLOR_GREY_BACKGROUND_FREEZE = 0x00BDF7; //R,G,B = 191,191,191;


    Table(ILI9341& display, int anzahl, bool status = false);
    ~Table();
    void drawTableLines();
    void printTable();
    void printMessages();
    void setPausingStatus(bool status);
    bool getPausingStatus();
    void loop();
    void updateHeadlineBackground();
    void updateMessages();
    void printSingleMessage(int pos);
  
};



#endif