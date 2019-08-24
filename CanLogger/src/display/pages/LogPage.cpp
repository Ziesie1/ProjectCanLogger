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
    
    screenBuffer_disableUpdate();
    screenBuffer_clearScreenBuffer();
}

void LogPage::loop()
{
    //this->logTable->loop();

    if(wasSingleTasterPressed())
    {
        if(this->logTable->getPausingStatus())
        {
            this->logTable->setPausingStatus(false);
            this->logTable->updateHeadlineBackground();
            screenBuffer_enableUpdate();
            
        }
        else
        {
             pageManager.deleteOpenPage();
        }
    }

    if(wasEncoderButtonPressed())
    {
        this->logTable->setPausingStatus(true);
        this->logTable->updateHeadlineBackground();
        screenBuffer_disableUpdate();
    }

    if(hasEncoderPosChanged())
    {
        this->logTable->setPausingStatus(true);
        this->logTable->updateHeadlineBackground();
        screenBuffer_disableUpdate();
        
    }
    
    
}

void LogPage::loadStartView()
{
 
    // for(int idx = 0;idx<2;idx++)
    // {
    //     sortCanMessageIntoBuffer(new Canmsg{});
    // }
    

    if(statusSD)
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,getFullLogFilePath().c_str(),screenBuffer_getFillLevel());
    }
    else
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,"Ohne Speichern",screenBuffer_getFillLevel());
        
    }
}