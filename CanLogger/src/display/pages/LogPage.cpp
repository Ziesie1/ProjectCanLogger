#include <Arduino.h>
#include "display/pages/LogPage.hpp"
#include "buttons/Encoder.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/elements/Button.hpp"
#include "buttons/Taster.hpp"
#include "can/CanUtility.hpp"
#include "display/screenBuffer.hpp"
#include "sd/SD.hpp"
#include "utilities/SerialCommunication.hpp"

using namespace utilities;

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
    this->logTable->loop();

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

void LogPage::startView()
{
    if(CanUtility_EnableRecieve() != HAL_OK)
    {
        scom.printError("Konnte das Empfangen von Cannachrichten nicht aktivieren.\nDas Programm wird angehalten!");
        while(1){}
    }
    screenBuffer_enableUpdate();

    if(this->statusSD)
    {
        init_SD();
        createNewCanLogFile();
        startSD();

        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,getFullLogFilePath().c_str(),screenBuffer_getFillLevel());
    }
    else
    {
        this->display.fillScreen(WHITE);
        this->logTable = new Table(this->display,"Ohne Speichern",screenBuffer_getFillLevel());
        
    }
}

void LogPage::reloadView()
{
    this->startView();
}

void LogPage::closeView()
{
    CanUtility_DissableRecieve();
    screenBuffer_disableUpdate();
    screenBuffer_clearScreenBuffer();
    CanUtility_resetDiscardcounter();
    
    if(this->statusSD)
        closeSD();
}