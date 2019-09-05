#include <Arduino.h>
#include "display/pages/HomePage.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/LogPage.hpp"
#include "display/DisplayImages.hpp"
#include "buttons/Encoder.hpp"
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

/*
    Constructor of the class HomePage
    input: display - Reference of the display where the homescreen will be printed on
*/
HomePage::HomePage(ILI9341& display)
    :display{display}, statusSD{false}
{
    
}

/*
    The destructor of the class HomePage calls the destructors of all created objects.
*/
HomePage::~HomePage()
{
    if(this->buttonNichtSpeichern)
        delete this->buttonNichtSpeichern;
    this->buttonNichtSpeichern = nullptr;
    if(this->buttonSpeichern)
        delete this->buttonSpeichern;
    this->buttonSpeichern = nullptr;
    if(this->pfeilNichtSpeichern)
        delete this->pfeilNichtSpeichern;
    this->pfeilNichtSpeichern = nullptr;
    if(this->pfeilSpeichern)
        delete this->pfeilSpeichern;
    this->pfeilSpeichern = nullptr;
}

/*
    The loop methode recognice detect if a userinput was set and calls the according acttions.
*/
void HomePage::loop()
{
    if(wasEncoderButtonPressed() && (getEncoderPos() == 0 || getEncoderPos() == 1))
    {
        if(this->buttonSpeichern->getStatus())
        {
            pageManager.openNewPage(new LogPage{this->display, true, getFullLogFilePath().c_str()});
        }
        else
        {
           pageManager.openNewPage(new LogPage{this->display,false, "Ohne Speichern"});
        }   
    }
    if(hasEncoderPosChanged())
    {  
        if(getEncoderPos() == 0)
        {
            this->statusSD = false;
            this->buttonNichtSpeichern->selectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_SELECTED);
            this->buttonSpeichern->unselectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonEinstellungen->unselectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_UNSELECTED);
        }
        if(getEncoderPos() == 1)
        {
            this->statusSD = true;
            this->buttonNichtSpeichern->unselectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonSpeichern->selectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_SELECTED);
            this->buttonEinstellungen->unselectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_UNSELECTED);
        }
        if(getEncoderPos() == 2)
        {
            this->statusSD = false;
            this->buttonNichtSpeichern->unselectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonSpeichern->unselectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonEinstellungen->selectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_SELECTED);
        }

        if(getEncoderPos() > 2)
        {
            setEncoderPos(2);
        }
        if(getEncoderPos() < 0)
        {
            setEncoderPos(0);
        }
    }
}

/*
    prints the start screen
*/
void HomePage::startView()
{
    this->display.fillScreen(WHITE);

    uint16_t xmitte = ILI9341::HORIZONTAL_MAX / 2;

    this->display.drawBmp(xmitte-OSTFALIA_LOGO_SIZE_X / 2, IMAGE_DST_Y + 25, OSTFALIA_LOGO_SIZE_X, OSTFALIA_LOGO_SIZE_Y, OSTFALIA_LOGO, 1);

    this->buttonNichtSpeichern = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET),this->BUTTON_WIDTH,this->BUTTON_HIGH,this->COLOR_BUTTON_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - ohne Speichern", this->COLOR_BUTTON_TEXT, !this->statusSD};
    this->buttonSpeichern = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET+this->BUTTON_Y_DISTANCE+this->BUTTON_HIGH), this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, this->BUTTON_TEXT_DST_X, this->BUTTON_TEXT_DST_Y, "Logging - mit Speichern", this->COLOR_BUTTON_TEXT, this->statusSD};
    this->buttonEinstellungen = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET+2*(this->BUTTON_Y_DISTANCE+this->BUTTON_HIGH)), this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, static_cast<byte>((this->BUTTON_WIDTH-13*8)/2), this->BUTTON_TEXT_DST_Y, "Einstellungen", this->COLOR_BUTTON_TEXT, this->statusSD};

    this->pfeilNichtSpeichern = new Arrow{this->display, this->buttonNichtSpeichern, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED, !this->statusSD};
    this->pfeilSpeichern = new Arrow{this->display, this->buttonSpeichern, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED, this->statusSD};
    this->pfeilEinstellungen = new Arrow{this->display, this->buttonEinstellungen, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED, this->statusSD};
    
}

/*
    relode the already
*/
void HomePage::reloadView()
{
    this->startView();
}