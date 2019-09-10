#include <Arduino.h>
#include "display/pages/HomePage.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/LogPage.hpp"
#include "display/DisplayImages.hpp"
#include "buttons/Encoder.hpp"
#include "sd/SD.hpp"
#include "can/CanUtility.hpp"
#include "display/pages/SettingPage.hpp"

extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main

/*
    Constructor of the class HomePage
    input: display - Reference of the display where the homescreen will be printed on
*/
HomePage::HomePage(ILI9341& display)
    :display{display}, statusSD{false},statusSettings{false}
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
    if(this->buttonEinstellungen)
        delete this->buttonEinstellungen;
    this->buttonEinstellungen = nullptr;
    if(this->pfeilNichtSpeichern)
        delete this->pfeilNichtSpeichern;
    this->pfeilNichtSpeichern = nullptr;
    if(this->pfeilSpeichern)
        delete this->pfeilSpeichern;
    this->pfeilSpeichern = nullptr;
    if(this->pfeilEinstellungen)
        delete this->pfeilEinstellungen;
    this->pfeilEinstellungen = nullptr;
}

/*
    The loop methode recognice detect if a userinput was set and calls the according acttions.
*/
void HomePage::loop()
{
    if(wasEncoderButtonPressed() && (this->lastEncoderPos == 0 || this->lastEncoderPos == 1 || this->lastEncoderPos == 2))
    {
        if(this->buttonSpeichern->getStatus())
        {
            pageManager.openNewPage(new LogPage{this->display,true});
        }
        else if(this->buttonNichtSpeichern->getStatus())
        {
           pageManager.openNewPage(new LogPage{this->display,false});
        }
        else
        {
            pageManager.openNewPage(new SettingPage{this->display});
        }
           
    }

    if(hasEncoderPosChanged())
    {  
        this->lastEncoderPos += getRelativeEncoderPos(); // Encoderpositon updaten

        if(this->lastEncoderPos == 0)
        {
            this->statusSD = false;
            statusSettings = false;
            this->buttonNichtSpeichern->selectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_SELECTED);
            this->buttonSpeichern->unselectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonEinstellungen->unselectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_UNSELECTED);
        }

        if(this->lastEncoderPos == 1)
        {
            this->statusSD = true;
            statusSettings = false;
            this->buttonNichtSpeichern->unselectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonSpeichern->selectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_SELECTED);
            this->buttonEinstellungen->unselectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_UNSELECTED);
        }

        if(this->lastEncoderPos == 2)
        {
            this->statusSD = false;
            statusSettings = true;
            this->buttonNichtSpeichern->unselectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonSpeichern->unselectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonEinstellungen->selectButton();
            this->pfeilEinstellungen->draw(this->COLOR_ARROW_SELECTED);
        }

        if(this->lastEncoderPos > 2)
        {
            this->lastEncoderPos = 2;
        }
        if(this->lastEncoderPos < 0)
        {
            this->lastEncoderPos = 0;
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

    this->buttonNichtSpeichern = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET),this->BUTTON_WIDTH,this->BUTTON_HIGH,this->COLOR_BUTTON_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - ohne Speichern", this->COLOR_BUTTON_TEXT, !this->statusSD && !this->statusSettings};
    this->buttonSpeichern = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET+this->BUTTON_Y_DISTANCE+this->BUTTON_HIGH), this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, this->BUTTON_TEXT_DST_X, this->BUTTON_TEXT_DST_Y, "Logging - mit Speichern", this->COLOR_BUTTON_TEXT, this->statusSD};
    this->buttonEinstellungen = new Button{this->display, static_cast<uint16_t>(xmitte - this->BUTTON_WIDTH / 2), static_cast<uint16_t>(this->FIRST_BUTTON_Y_OFFSET+2*(this->BUTTON_Y_DISTANCE+this->BUTTON_HIGH)), this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, static_cast<byte>((this->BUTTON_WIDTH-13*8)/2), this->BUTTON_TEXT_DST_Y, "Einstellungen", this->COLOR_BUTTON_TEXT, this->statusSettings};

    this->pfeilNichtSpeichern = new Arrow{this->display, this->buttonNichtSpeichern, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED, !this->statusSD && !statusSettings};
    this->pfeilSpeichern = new Arrow{this->display, this->buttonSpeichern, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED, this->statusSD};
    this->pfeilEinstellungen = new Arrow{this->display, this->buttonEinstellungen, this->COLOR_ARROW_SELECTED, this->COLOR_ARROW_UNSELECTED,  statusSettings};
    
}

/*
    reloade the already
*/
void HomePage::reloadView()
{
    this->startView();
}