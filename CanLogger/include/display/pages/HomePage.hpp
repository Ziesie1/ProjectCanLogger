#ifndef HOME_PAGE_HPP
#define HOME_PAGE_HPP

#include "display/ILI9341.hpp"
#include "display/pages/DisplayPage.hpp"
#include "display/Button.hpp"
class HomePage : public DisplayPage
{

protected:
    const uint16_t BUTTON_WIDTH = 205;
    const uint16_t BUTTON_HIGH = 30;
    const uint16_t BUTTON_MID_DIST = 20;
    const uint16_t BUTTON_TEXT_DST_X = 5;
    const uint16_t BUTTON_TEXT_DST_Y = 8;
    const Color BUTTON_COLOR_DEFAULT = BLUE100;

    const uint16_t IMAGE_DST_Y = 15;

    ILI9341& display;
   
   Button* buttonNichtSpeichern;
   Button* buttonSpeichern;
   
public:
    HomePage(ILI9341& display);
    void loop() override;
    void startView() override;
    ~HomePage();
};


#endif