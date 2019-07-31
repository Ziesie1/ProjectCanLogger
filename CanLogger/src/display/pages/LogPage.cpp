#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/Button.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

LogPage::LogPage(ILI9341& display, bool statusSD)
 :display{display}, statusSD{statusSD}
{
    
}
LogPage::~LogPage()
{
    delete[] this->buttonZurueck;
    this->buttonZurueck = nullptr;
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
    if(statusSD)
    {
        this->display.fillScreen(RED100);
        this->buttonZurueck = new Button{160,10,150,30,BLUE100,5,8,"Zurück",WHITE,this->display,false};
    }
    else
    {
        this->display.fillScreen(BLUE50);
        this->buttonZurueck = new Button{160,10,150,30,BLUE100,5,8,"Zurück",WHITE,this->display,false};
    }
}