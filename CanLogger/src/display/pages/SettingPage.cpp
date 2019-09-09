#include "display/pages/SettingPage.hpp"
#include "display/DisplayPageManager.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

extern DisplayPageManager pageManager; 

/*
    Constructor of the class SettingPage
    Input: display - reference to the used display-object
           headline - headine of the site
    return: -
*/

SettingPage::SettingPage(ILI9341& display,  String headline)
    :display{display}, kopfzeile{headline}
{

}

/*
    Dstructor of the class SettingPage
    Input: -
    return: -
*/
SettingPage::~SettingPage()
{
     if(this->header)
     {
         delete this->header;
         this->header = nullptr;
     }
     if(this->buttonDiscard)
     {
         delete this->buttonDiscard;
         this->buttonDiscard = nullptr;
     }
     if(this->buttonStore)
     {
         delete this->buttonStore;
         this->buttonStore = nullptr;
     }
     if(this->speedSelect)
     {
         delete this->speedSelect;
         this->speedSelect = nullptr;
     }
     if(this->silentSelect)
     {
         delete this->silentSelect;
         this->silentSelect = nullptr;
     }
}

/*
    Loop-methode of the class SettingPage which is calles cyclically. It checks the status of the Encoder and the single push-button
    in order to vary the objects of the SettingPage in the expected way.
    Input: -
    return: -
*/
void SettingPage::loop()
{
    if(speedSelect->loop() || silentSelect->loop())
    {

    }else
    {
        if(hasEncoderPosChanged())
        {
            this->lastEncoderPos += getRelativeEncoderPos();
            
            if(this->lastEncoderPos == 0)
            {
                
                this->speedSelect->selectButton();
                this->silentSelect->unselectButton();
                this->buttonDiscard->unselectButton();
                this->buttonStore->unselectButton();
            }
            else if(this->lastEncoderPos == 1)
            {
                this->speedSelect->unselectButton();
                this->silentSelect->selectButton();
                this->buttonDiscard->unselectButton();
                this->buttonStore->unselectButton();
            }
            else if(this->lastEncoderPos == 2)
            {
                this->speedSelect->unselectButton();
                this->silentSelect->unselectButton();
                this->buttonDiscard->selectButton();
                this->buttonStore->unselectButton();
                
            }
            else if(this->lastEncoderPos == 3)
            {
                this->speedSelect->unselectButton();
                this->silentSelect->unselectButton();
                this->buttonDiscard->unselectButton();
                this->buttonStore->selectButton();
            }else if(this->lastEncoderPos<0)
            {
                this->lastEncoderPos = 0;
            }else if(this->lastEncoderPos>3)
            {
                this->lastEncoderPos = 3;
            }
        }else if(wasSingleTasterPressed())
        {
            pageManager.deleteOpenPage();
        }else if(wasEncoderButtonPressed())
        {
            if(lastEncoderPos == 0)
            {
                speedSelect->pressButton();
            }else if(lastEncoderPos == 1)
            {
                silentSelect->pressButton();
            }else if(lastEncoderPos > 1)
            {
                if(lastEncoderPos == 3)
                {
                    speedSelect->saveValue();
                    silentSelect->saveValue();
                }
                pageManager.deleteOpenPage();
            }

            
        }
    }
}

/*
    This method builds the Page with all containing elements
    Input:  -
    return: -
*/
void SettingPage::startView() 
{
    this->display.fillScreen(WHITE);
    this->header = new Kopfzeile{this->display, this->kopfzeile};
    setElements();
    this->speedSelect->selectButton();
}

/*
    Once the site has been opened, this method is called in the reopening process.
    Input: -
    return: -
*/
void SettingPage::reloadView()
{
    this->startView();
}

/*
    This method is called, when the site is closed
    Input: -
    return: -
*/
void SettingPage::closeView() 
{
    
}

/*
    This methode prints the elements, such as buttons and their descriptions, of the site on the site.
    Input: -
    return: -
*/
void SettingPage::setElements()
{
    //Strings
    this->display.printString(this->TRANSFER_RATE_SILENT_MODE_POS_X, this->TRANSFER_RATE_POS_Y, this->TRANSFER_RATE.c_str(), this->COLOR_BLACK_SETTING_WRITING, this->COLOR_WHITE_BACKGROUND_SETTING, 1);
    this->display.printString(this->TRANSFER_RATE_SILENT_MODE_POS_X, this->SILENT_MODE_POS_Y, this->SILENT_MODE.c_str(), this->COLOR_BLACK_SETTING_WRITING, this->COLOR_WHITE_BACKGROUND_SETTING, 1);

    //Buttons
    this->buttonDiscard = new Button{this->display, static_cast<uint16_t>((this->DISPLAY_X / 2-this->BUTTON_WIDTH) / 2), this->BUTTON_POS_Y, this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, this->BUTTON_DISCARD_TEXT.c_str(), this->COLOR_BUTTON_TEXT, false};
    this->buttonStore = new Button{this->display, static_cast<uint16_t>((this->DISPLAY_X / 2-this->BUTTON_WIDTH) / 2 + this->DISPLAY_X / 2), this->BUTTON_POS_Y, this->BUTTON_WIDTH, this->BUTTON_HIGH, this->COLOR_BUTTON_DEFAULT, this->BUTTON_STORE_TEXT.c_str(), this->COLOR_BUTTON_TEXT, false};
    this->speedSelect = new SpeedSelectionButton{this->display, this->BUTTON_SPECIAL_POS_X, this->TRANSFER_RATE_POS_Y, this->BUTTON_SPECIAL_WIDTH, this->BUTTON_SPECIAL_HIGH, this->COLOR_BUTTON_DEFAULT, this->COLOR_BUTTON_TEXT, true};
    this->silentSelect = new SilentSelectionButton{this->display, this->BUTTON_SPECIAL_POS_X, this->SILENT_MODE_POS_Y, this->BUTTON_SPECIAL_WIDTH, this->BUTTON_SPECIAL_HIGH, this->COLOR_BUTTON_DEFAULT, this->COLOR_BUTTON_TEXT, false};
    
}
