#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/elements/Button.hpp"
#include "buttons/Taster.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

LogPage::LogPage(ILI9341& display, bool statusSD)
 :display{display}, statusSD{statusSD}
{
    
}

LogPage::~LogPage()
{

}

void LogPage::loop()
{
    if(wasSingleTasterPressed())
    {
        pageManager.deleteOpenPage();
    }
}

void LogPage::startView()
{
    if(statusSD)
    {
        this->display.fillScreen(WHITE);
        display.printString(50,50,"Logging mit Speichern",BLUE100,WHITE,1);
    }
    else
    {
        this->display.fillScreen(WHITE);
        display.printString(50,50,"Logging ohne Speichern",BLUE100,WHITE,1);
        
    }
}