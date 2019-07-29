#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"


extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

LogPage::LogPage(ILI9341& display)
 :display{display}
{ }

void LogPage::loop()
{
    if(wasEncoderButtonPressed())
    {
        pageManager.deleteOpenPage();
    }
}

void LogPage::startView()
{
    this->display.fillScreen(RED100);
}