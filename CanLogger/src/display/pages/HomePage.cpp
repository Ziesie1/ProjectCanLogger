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
    
}

HomePage::~HomePage()
{
    if(this->buttonNichtSpeichern)
        delete this->buttonNichtSpeichern;
    this->buttonNichtSpeichern = nullptr;
    if(this->buttonSpeichern)
        delete this->buttonSpeichern;
    this->buttonSpeichern = nullptr;
}

void HomePage::loop()
{
    if(wasEncoderButtonPressed() && (getEncoderPos() == 1 || getEncoderPos() == 2))
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
            this->buttonNichtSpeichern->resetFrame();
            this->buttonSpeichern->resetFrame();
        }
        if(getEncoderPos() == 1)
        {
            this->buttonNichtSpeichern->setFrame();
            this->buttonSpeichern->resetFrame();
        }
        if(getEncoderPos() == 2)
        {
            this->buttonNichtSpeichern->resetFrame();
            this->buttonSpeichern->setFrame();
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

void HomePage::startView()
{

    this->display.fillScreen(WHITE);

    uint16_t ymitte = ILI9341::VERTICAL_MAX/2;
    uint16_t xmitte = ILI9341::HORIZONTAL_MAX/2;

    this->display.drawBmp(xmitte-OSTFALIA_LOGO_SIZE_X/2, IMAGE_DST_Y, OSTFALIA_LOGO_SIZE_X, OSTFALIA_LOGO_SIZE_Y, OSTFALIA_LOGO, 1);

    buttonNichtSpeichern = new Button{this->display, xmitte-this->BUTTON_WIDTH/2,ymitte-this->BUTTON_MID_DIST-this->BUTTON_HIGH,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->BUTTON_COLOR_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - ohne Speichern",WHITE,false};
    buttonSpeichern = new Button{this->display, xmitte-this->BUTTON_WIDTH/2,ymitte+this->BUTTON_MID_DIST,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->BUTTON_COLOR_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - mit Speichern",WHITE,false};
    
    
    //this->display.fillScreen(WHITE);

    //uint16_t ymitte = ILI9341::VERTICAL_MAX/2;
    //uint16_t xmitte = ILI9341::HORIZONTAL_MAX/2;

    //this->display.drawBmp(xmitte-OSTFALIA_LOGO_SIZE_X/2, IMAGE_DST_Y, OSTFALIA_LOGO_SIZE_X, OSTFALIA_LOGO_SIZE_Y, OSTFALIA_LOGO, 1);

    //Button buttonNichtSpeichern{xmitte-this->BUTTON_WIDTH/2,ymitte-this->BUTTON_MID_DIST-this->BUTTON_HIGH,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->BUTTON_COLOR_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - ohne Speichern",WHITE, display,false};
    //Button buttonSpeichern{xmitte-this->BUTTON_WIDTH/2,ymitte+this->BUTTON_MID_DIST,this->BUTTON_WIDTH,this->BUTTON_HIGH,this->BUTTON_COLOR_DEFAULT,this->BUTTON_TEXT_DST_X,this->BUTTON_TEXT_DST_Y,"Logging - mit Speichern",WHITE,display,false};




    //this->display.drawFillRect(xmitte-this->BUTTON_WIDTH/2,ymitte-this->BUTTON_MID_DIST-this->BUTTON_HIGH,this->BUTTON_WIDTH, this->BUTTON_HIGH, this->BUTTON_COLOR_DEFAULT);
    //this->display.drawFillRect(xmitte-this->BUTTON_WIDTH/2,ymitte+this->BUTTON_MID_DIST,this->BUTTON_WIDTH, this->BUTTON_HIGH, this->BUTTON_COLOR_DEFAULT);

    //this->display.printString(xmitte-this->BUTTON_WIDTH/2+this->BUTTON_TEXT_DST_X, ymitte-this->BUTTON_MID_DIST-this->BUTTON_HIGH+this->BUTTON_TEXT_DST_Y, "Logging - ohne Speichern",WHITE, this->BUTTON_COLOR_DEFAULT,1);
    //this->display.printString(xmitte-this->BUTTON_WIDTH/2+this->BUTTON_TEXT_DST_X, ymitte+this->BUTTON_MID_DIST+this->BUTTON_TEXT_DST_Y, "Logging - mit Speichern",WHITE, this->BUTTON_COLOR_DEFAULT,1);

}
