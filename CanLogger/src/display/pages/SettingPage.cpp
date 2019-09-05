#include "display/pages/SettingPage.hpp"
#include "display/DisplayPageManager.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

extern DisplayPageManager pageManager; 

SettingPage::SettingPage(ILI9341& display,  String headline)
    :display{display}, kopfzeile{headline}
{

}

SettingPage::~SettingPage()
{
     if(this->header)
     {
         delete this->header;
         this->header = nullptr;
     }
}

void SettingPage::loop()
{
    if(wasSingleTasterPressed())
    {
        pageManager.deleteOpenPage();
    }
}

void SettingPage::startView() 
{
    this->display.fillScreen(WHITE);
    this->header = new Kopfzeile{this->display, this->kopfzeile};
}

void SettingPage::reloadView()
{
    this->startView();
}

void SettingPage::closeView() 
{

}
