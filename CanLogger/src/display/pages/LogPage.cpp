#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/elements/Button.hpp"
#include "buttons/Taster.hpp"
#include "can/CanUtility.hpp"
#include "display/screenBuffer.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

LogPage::LogPage(ILI9341& display, bool statusSD)
 :display{display}, statusSD{statusSD}
{
    
}

LogPage::~LogPage()
{
  if(this->logTable)
  {
    delete this->logTable;
    this->logTable = nullptr;

  }
    

}

void LogPage::loop()
{
    this->logTable->loop();

    if(wasSingleTasterPressed())
    {
        if(this->logTable->getPausingStatus())
        {
            this->logTable->setPausingStatus(false);
            this->logTable->updateHeadlineBackground();
        }
        else
        {
             pageManager.deleteOpenPage();
        }
    }

    if(hasEncoderPosChanged())
    {
        this->logTable->setPausingStatus(true);
        this->logTable->updateHeadlineBackground();
    }
    
    
}

void LogPage::loadStartView()
{
    

    Canmsg* nachrichten = new Canmsg[screenBuffer_getFillLevel()];
    for(int i=0;i<screenBuffer_getFillLevel();i++)
    {
        screenBuffer_getMessageAtPosition(nachrichten[i],i);
    }

    if(statusSD)
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,getFullLogFilePath().c_str(),nachrichten,screenBuffer_getFillLevel());
    }
    else
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,"Ohne Speichern",nachrichten,screenBuffer_getFillLevel());
        
    }
}