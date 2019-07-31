#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/Button.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

LogPage::LogPage(ILI9341& display, bool statusSD)
 :display{display}, statusSD{statusSD}
{
    if(statusSD)
    {
        this->display.fillScreen(RED100);
    }
    else
    {
        this->display.fillScreen(BLUE50);
    }
}

void LogPage::loop()
{
    if(wasEncoderButtonPressed())
    {
        pageManager.deleteOpenPage();
    }
}

void LogPage::startView()
{
    
}