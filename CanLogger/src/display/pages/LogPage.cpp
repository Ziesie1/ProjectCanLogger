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
  if(this->logTable)
  {
    //delete this->logTable;
    //this->logTable = nullptr;
    /*
    Der Destruktor Aufruf muss noch angepasst werden, da dieser im Moment noch dazu führt, dass sich das Programm
    aufhängt
     */
  }
    

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
    Canmsg* nachrichten = new Canmsg[5];
    if(statusSD)
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,"Speicherpfad SD",nachrichten,5);
    }
    else
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,"Ohne Speichern",nachrichten,5);
        
    }
}