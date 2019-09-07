#ifndef HOME_PAGE_HPP
#define HOME_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/elements/Button.hpp"
#include "display/elements/Arrow.hpp"
#include "display/elements/Table.hpp"
/*
    This Class creates the homescreen page and prints it on the display.
*/
class HomePage : public DisplayPage
{

protected:
    const byte BUTTON_WIDTH = 204;
    const  uint16_t BUTTON_HIGH = 32;
    const uint16_t BUTTON_MID_DIST = 20;
    const uint16_t BUTTON_Y_AXE_OFFSET = 80;
    const uint16_t FIRST_BUTTON_Y_OFFSET = 160;
    const uint16_t BUTTON_Y_DISTANCE = 20;
    const byte BUTTON_TEXT_DST_X = 5;
    const byte BUTTON_TEXT_DST_Y = 8;
    const unsigned long COLOR_BUTTON_DEFAULT = Table::COLOR_BLUE_BACKGROUND_HEADER;
    const unsigned long COLOR_BUTTON_TEXT = WHITE;
    unsigned long COLOR_ARROW_SELECTED = Table::COLOR_ORANGE_WRITING_BODY_ISRTR;
    const unsigned long COLOR_ARROW_UNSELECTED = WHITE;
    const uint16_t IMAGE_DST_Y = 15;

   

    ILI9341& display;
    bool statusSD;
    bool statusSettings;
   
    Button* buttonNichtSpeichern;
    Arrow*  pfeilNichtSpeichern;
    Button* buttonSpeichern;
    Arrow* pfeilSpeichern;
    Button* buttonEinstellungen;
    Arrow* pfeilEinstellungen;
   
public:
    HomePage(ILI9341& display);
    void loop() override;
    void startView() override;
    void reloadView() override;
    ~HomePage();
};


#endif