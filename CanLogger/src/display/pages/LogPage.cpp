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

/*
    Constructor of the class LogPage
    input: display          - Reference of the display where the LogPage will be printed on
           status           - pausing status, if it´s true "Table" will go in freze mode and the printet messages staing constant
*/
LogPage::LogPage(ILI9341& display, bool statusSD, String headline)
 :display{display}, statusSD{statusSD}, headline{headline}
{
    
}
/*
    The destructor of the class Table "LogPage" calls the destructors of all created objects and stops the creenbuffer updates.
*/
LogPage::~LogPage()
{
  if(this->logTable)
  {
    delete this->logTable;
    this->logTable = nullptr;
  }

  if(this->kopfzeile)
  {
      delete this->kopfzeile;
      this->kopfzeile = nullptr;
  }
    
    screenBuffer_disableUpdate();
    screenBuffer_clearScreenBuffer();
}

/*
    The loop methode recognice detect if a userinput was set and calls the according acttions. Futhermore it calls the Table loop.
*/
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

/*
    prints the start screen for runnung mode, generats the Table
*/
void LogPage::startView()
{
    if(CanUtility_EnableRecieve() != HAL_OK)
    {
        scom.printError("Konnte das Empfangen von Cannachrichten nicht aktivieren.\nDas Programm wird angehalten!");
        while(1){}
    }
    
    screenBuffer_enableUpdate();
    this->display.fillScreen(WHITE);
    this->kopfzeile = new Kopfzeile{this->display, headline};
    this->logTable = new Table(this->display, screenBuffer_getFillLevel());

    if(this->statusSD)
    {
        init_SD();
        createNewCanLogFile();
        startSD();

       
    }
    
}
/*
    relode the start screen.
*/
void LogPage::reloadView()
{
    this->startView();
}
/*
    stop logging, go back to homescreen
*/
void LogPage::closeView()
{
    CanUtility_DissableRecieve();
    screenBuffer_disableUpdate();
    screenBuffer_clearScreenBuffer();

    if(this->statusSD)
        closeSD();
}