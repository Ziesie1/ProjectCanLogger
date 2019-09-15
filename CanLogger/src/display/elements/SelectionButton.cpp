#include "display/elements/SelectionButton.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"


SelectionButton::SelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor,
                                unsigned long textColor, bool isSelected, bool ringBuff, byte buffSize)
                                :Button(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected), isPressed{false}, ringBuff{ringBuff}, textBuffSize{buffSize}
                                {
                                }
                                

SelectionButton::~SelectionButton()
{
      if(this->textBuff)
     {
         delete this->textBuff;
         this->textBuff = nullptr;
     } 
}

bool SelectionButton::getPressed()
{
    return this->isPressed;
}
void SelectionButton::pressButton()
{
    this->isPressed = true;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_GREEN_FRAME_PRESSED, this->FRAME_WIDTH);
}
void SelectionButton::unpressButton()
{
    this->isPressed = false;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_ORANGE_FRAME_SELECTED, this->FRAME_WIDTH);
}

void SelectionButton::setText()
{
    this->initText();
    this->display.drawFillRect(this->startX + FRAME_WIDTH, this->startY + FRAME_WIDTH, this->sizeX - 2 * FRAME_WIDTH, this->sizeY - 2 * FRAME_WIDTH, this->backColor);
    this->printText(); 
}

void SelectionButton::initText()
{
     if (this->textBuff)
    {
        this->text = this->textBuff[buffPos];
    } 
    this->setTextOffset();
}

bool SelectionButton::loop()
{
    if(this->getPressed())
    {
        if(hasEncoderPosChanged())
        {
            this->buffPos += getRelativeEncoderPos();

            if (this->ringBuff)
            {
                if(this->buffPos > (this->textBuffSize - 1))
                {
                    this->buffPos = 0;
                }else if(buffPos < 0)
                {
                    this->buffPos = this->textBuffSize - 1;
                }
            }else
            {
                if(this->buffPos > (this->textBuffSize - 1))
                {
                    this->buffPos = this->textBuffSize - 1;
                }else if(buffPos < 0)
                {
                    this->buffPos = 0;
                }

            }
            this->setText();
        }
        if(wasEncoderButtonPressed() || wasSingleTasterPressed())
        {
            this->unpressButton();
            return false;
        }
        return true;
    }
     return false;
}