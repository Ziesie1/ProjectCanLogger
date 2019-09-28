#include "display/elements/SelectionButton.hpp"
#include "buttons/Encoder.hpp"
#include "buttons/Taster.hpp"

/*
    Constructor of the virtual class SelectionButton
    input:  display     - reference of the display where the Button will be printed
            startx      - start position on x-axis
            starty      - start position on y-axis
            sizeX       - button width
            sizeY       - button hight
            backColor   - button collor
            textColor   - collor of the text
            isSelected  - Status of the button
            ringBuff    - tru or textbuffer for selectin is a ring buffer, false if not
            buffSize    - size of textbuffer for selection
*/
SelectionButton::SelectionButton (ILI9341& display, uint16_t startX, uint16_t startY, uint16_t sizeX, 
                                uint16_t sizeY, unsigned long backColor, unsigned long textColor, bool isSelected, 
                                bool ringBuff, byte buffSize)
                                :Button(display, startX,  startY, sizeX, sizeY, backColor, textColor,
                                    isSelected), isPressed{false}, ringBuff{ringBuff}, textBuffSize{buffSize}
                                {
                                }
                                
/*
    Dstructor of the class SelectionmButton
*/
SelectionButton::~SelectionButton()
{
      if(this->textBuff)
     {
         delete this->textBuff;
         this->textBuff = nullptr;
     } 
}

/*
    Returns the information if the button is pressed
*/
bool SelectionButton::getPressed()
{
    return this->isPressed;
}

/*
    sets the button status to pressd, changes the frame collor to green 
*/
void SelectionButton::pressButton()
{
    this->isPressed = true;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_GREEN_FRAME_PRESSED, this->FRAME_WIDTH);
}

/*
    sets the button status to unpressd, changes the frame collor to orange 
*/
void SelectionButton::unpressButton()
{
    this->isPressed = false;
    this->display.drawEmptyRect(this->startX, this->startY, this->sizeX, this->sizeY, this->COLOR_ORANGE_FRAME_SELECTED, this->FRAME_WIDTH);
}

/*
    setting the button text dsepending on the buffPos
*/
void SelectionButton::setText()
{
    if (this->textBuff)
    {
        this->text = this->textBuff[buffPos];
    } 
}

/*
    setting the button text for the constuktor and init the Y-Offset 
*/
void SelectionButton::initText()
{
    this->setText();
    this->setTextOffsetY();
}

/*
    printing the button text and clear teh uesed display section. 
*/
void SelectionButton::printText()
{
    this->setTextOffsetX();
    this->display.drawFillRect(this->startX + FRAME_WIDTH, this->startY + FRAME_WIDTH, this->sizeX - 2 * FRAME_WIDTH, this->sizeY - 2 * FRAME_WIDTH, this->backColor);
    Button::printText(); 
}

/*
    The loop methode chenges the bufferPos if the button is pressed ant the encoderPos has changed. The hew text will be printed. 
    return: true    - button is pressd
            false   - button is not pressd   
*/
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
            this->printText();
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