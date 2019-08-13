#include <Arduino.h>
#include "display/pages/HomePage.hpp"
#include "display/DisplayPageManager.hpp"
#include "display/pages/LogPage.hpp"
#include "display/DisplayImages.hpp"
#include "buttons/Encoder.hpp"


extern DisplayPageManager pageManager; // verweißt auf das Objekt in Main



HomePage::HomePage(ILI9341& display)
    :display{display}
{
    this->buttonColorDefault = this->display.makeColor(1,64,118);
    this->buttonColorSelected = this->display.makeColor(237,125,49);
    this->colorArrowSelected = this->display.makeColor(237,125,49);
}

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

void HomePage::loop()
{
    if(wasEncoderButtonPressed() && (getEncoderPos() == 0 || getEncoderPos() == 1))
    {
        if(this->buttonSpeichern->getStatus())
        {
            pageManager.openNewPage(new LogPage{display,true});
        }
        else
        {
           pageManager.openNewPage(new LogPage{display,false});
        }
        
        
    }
    if(hasEncoderPosChanged())
    {
        
        if(getEncoderPos() == 0)
        {
            this->buttonNichtSpeichern->selectButton();
            this->pfeilNichtSpeichern->draw(this->colorArrowSelected);
            this->buttonSpeichern->unselectButton();
            this->pfeilSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
        }
        if(getEncoderPos() == 1)
        {
            this->buttonNichtSpeichern->unselectButton();
            this->pfeilNichtSpeichern->draw(this->COLOR_ARROW_UNSELECTED);
            this->buttonSpeichern->selectButton();
            this->pfeilSpeichern->draw(this->colorArrowSelected);
        }
        if(getEncoderPos() > 1)
        {
            setEncoderPos(1);
        }
        if(getEncoderPos() < 0)
        {
            setEncoderPos(0);
        }
        
        
    }
}

void HomePage::startView()
{

    this->display.fillScreen(WHITE);

    uint16_t ymitte = ILI9341::VERTICAL_MAX/2;
    uint16_t xmitte = ILI9341::HORIZONTAL_MAX/2;

    this->display.drawBmp(xmitte-OSTFALIA_LOGO_SIZE_X/2, IMAGE_DST_Y+25, OSTFALIA_LOGO_SIZE_X, OSTFALIA_LOGO_SIZE_Y, OSTFALIA_LOGO, 1);

    this->buttonNichtSpeichern = new Button{this->display, xmitte-this->BUTTON_WIDTH/2,ymitte-this->BUTTON_MID_DIST-this->BUTTON_HIGH+this->BUTTON_Y_AXE_OFFSET,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->buttonColorDefault,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - ohne Speichern",this->BUTTON_COLOR_TEXT,true};
    this->buttonSpeichern = new Button{this->display, xmitte-this->BUTTON_WIDTH/2,ymitte+this->BUTTON_MID_DIST+this->BUTTON_Y_AXE_OFFSET,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->buttonColorDefault,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - mit Speichern",this->BUTTON_COLOR_TEXT,false};
    this->pfeilNichtSpeichern = new Arrow{this->display,this->buttonNichtSpeichern,this->colorArrowSelected,this->COLOR_ARROW_UNSELECTED,true};
    this->pfeilSpeichern = new Arrow{this->display,this->buttonSpeichern,this->colorArrowSelected,this->COLOR_ARROW_UNSELECTED,false};

}
