#include <Arduino.h>
#include "display/ILI9341.hpp"
#include "display/ILI9341_Definitions.hpp"
#include "serial/SerialCommunication.hpp"
#include "serial/FunctionLifeTime.hpp"


using namespace utilities;

ILI9341::ILI9341(const uint32_t& CS, const uint32_t& RESET,const uint32_t& MOSI, const uint32_t& DC, const uint32_t& SCK, bool rotateDisp)
    : PIN_CS{CS}, PIN_RESET{RESET}, PIN_MOSI{MOSI}, PIN_DC{DC}, PIN_SCK{SCK}
{
    if(rotateDisp)
	{
		this->displayFunctionControlREG |= 0x20; // SS-Bit setzen
		this->displayFunctionControlREG &= ~0x40; // GS-Bit rücksetzen
	}
}

/*
	Initialisiert das Display.
	GPIO-Pins und Konfigurierung des Displays.
*/
void ILI9341::init()
{
    PinName p = NC;
    p = digitalPinToPinName(this->PIN_CS);
    if(p != NC)
    {
        this->GPIO_PIN_CS = STM_LL_GPIO_PIN(p);
        this->GPIO_PORT_CS = get_GPIO_Port(STM_PORT(p));
    }

    p = digitalPinToPinName(this->PIN_RESET);
    if(p != NC)
    {
        this->GPIO_PIN_RESET = STM_LL_GPIO_PIN(p);
        this->GPIO_PORT_RESET = get_GPIO_Port(STM_PORT(p));
    }

    p = digitalPinToPinName(this->PIN_MOSI);
    if(p != NC)
    {
        this->GPIO_PIN_MOSI = STM_LL_GPIO_PIN(p);
        this->GPIO_PORT_MOSI = get_GPIO_Port(STM_PORT(p));
    }

    p = digitalPinToPinName(this->PIN_DC);
    if(p != NC)
    {
        this->GPIO_PIN_DC = STM_LL_GPIO_PIN(p);
        this->GPIO_PORT_DC = get_GPIO_Port(STM_PORT(p));
    }

    p = digitalPinToPinName(this->PIN_SCK);
    if(p != NC)
    {
        this->GPIO_PIN_SCK = STM_LL_GPIO_PIN(p);
        this->GPIO_PORT_SCK = get_GPIO_Port(STM_PORT(p));
    }

    this->gpioInit();
    this->displayInit();

	scom.printDebug("Display ist Initialisiert");
}

/*
	Initialisert die GPIO-Pins.
*/
void ILI9341::gpioInit()
{
    GPIO_InitTypeDef displayPins;
  
    displayPins.Mode = GPIO_MODE_OUTPUT_PP;
    displayPins.Pull = GPIO_NOPULL;
    displayPins.Speed = GPIO_SPEED_FREQ_HIGH;

    displayPins.Pin = this->GPIO_PIN_CS;
    HAL_GPIO_Init(this->GPIO_PORT_CS, &displayPins);

    displayPins.Pin = this->GPIO_PIN_RESET;
    HAL_GPIO_Init(this->GPIO_PORT_RESET, &displayPins);

    displayPins.Pin = this->GPIO_PIN_MOSI;
    HAL_GPIO_Init(this->GPIO_PORT_MOSI, &displayPins);

    displayPins.Pin = this->GPIO_PIN_DC;
    HAL_GPIO_Init(this->GPIO_PORT_DC, &displayPins);

    displayPins.Pin = this->GPIO_PIN_SCK;
    HAL_GPIO_Init(this->GPIO_PORT_SCK, &displayPins);

}

/*
	Konfiguriert das Display.
*/
void ILI9341::displayInit()
{
	// Version f�r TFT_PROTO auf dem Laboraufbau F303. 
	//    unsigned long ulCount;
	// VCI=2.8V
	//=== Reset LCD Driver ==============================================================================================
	setPinReset();
	delay(1); // Delay 1ms
	resetPinReset();
	delay(10); // Delay 10ms // This delay time is necessary
	setPinReset();
	delay(120); // Delay 120 ms

	//=== Start Initial Sequence ========================================================================================
	LCD_ILI9341_CMD(ILI9341_POWERCTLB_REG);      // 0xCF
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x81);
	LCD_ILI9341_Parameter (0X30);
 
	LCD_ILI9341_CMD(ILI9341_POWONSEQCTL_REG);    // 0xED 
		LCD_ILI9341_Parameter (0X64);
		LCD_ILI9341_Parameter (0X03);
		LCD_ILI9341_Parameter (0X12);
		LCD_ILI9341_Parameter (0X81);
 
	LCD_ILI9341_CMD(ILI9341_DIVTIMCTL_A_REG);    // 0xE8
		LCD_ILI9341_Parameter (0X85);
		LCD_ILI9341_Parameter (0X10);
		LCD_ILI9341_Parameter (0X7A);

	LCD_ILI9341_CMD(ILI9341_POWERCTLA_REG);      // 0xC8 
	WriteWord(0x392c);
	WriteWord(0x34);	
	WriteWord(0x02);
	
	LCD_ILI9341_CMD(0xCB);
	LCD_ILI9341_Parameter (0x39);
	LCD_ILI9341_Parameter (0x2C);
	LCD_ILI9341_Parameter (0x00);
	LCD_ILI9341_Parameter (0x34);
	LCD_ILI9341_Parameter (0x02);	
 
	LCD_ILI9341_CMD(ILI9341_PUMPRATIOCTL_REG);   // 0xF7 
	LCD_ILI9341_Parameter(0x20);

	LCD_ILI9341_CMD(ILI9341_DIVTIMCTL_B_REG);    // 0xEA set VCL,VGH,VGL,AVDD 
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_Parameter(0x00);
	
	LCD_ILI9341_CMD(ILI9341_FRAMECTL_NOR_REG);   // 0xb1 set VCL,VGH,VGL,AVDD 
	//	WriteWord(0x1b);
 
	LCD_ILI9341_CMD(ILI9341_POWERCTL1_REG);      // 0xC0 
	LCD_ILI9341_Parameter(0x2e);//26
	//	LCD_ILI9341_Parameter(0x21);//26 Einstellung f�r TFT_PROTO Display
 
	LCD_ILI9341_CMD(ILI9341_POWERCTL2_REG);      // 0xC1
	//	LCD_ILI9341_Parameter(0x12);	
	LCD_ILI9341_Parameter(0x11);	
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL1_REG);       // 0xC5 
	WriteWord(0x5019); //2b
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL2_REG);       // 0xC7 
	LCD_ILI9341_Parameter(0x90); //c4

	LCD_ILI9341_CMD(ILI9341_MEMACCESS_REG);      // 0x36
	//	LCD_ILI9341_Parameter(0xa8); // Original-Scanrichtung
	LCD_ILI9341_Parameter(0xa0); // Original-Scanrichtung

	LCD_ILI9341_CMD(ILI9341_FUNCTONCTL_REG);     // 0xB6
	WriteWord(this->displayFunctionControlREG);

	LCD_ILI9341_CMD(ILI9341_ENABLE_3G_REG);      // 0xF2
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(ILI9341_GAMMASET_REG);       // 0x26
	LCD_ILI9341_Parameter(0x01);

	LCD_ILI9341_CMD(ILI9341_POSGAMMACORRECTION_REG);  // 0xE0
	WriteWord(0x0F27);
	WriteWord(0x230B);
	WriteWord(0x0F05);
	WriteWord(0x5474);
	WriteWord(0x450A);
	WriteWord(0x170A);
	WriteWord(0x1C0E);
	WriteWord(0x08);//00

	//=== gamma"-"polarity correction characteristic setting ========================================================================================
	LCD_ILI9341_CMD(ILI9341_NEGGAMMACORRECTION_REG);   //0xE1
	WriteWord(0x081A);
	WriteWord(0x1E03);
	WriteWord(0x0F05);
	WriteWord(0x2E25);
	WriteWord(0x3B01);
	WriteWord(0x0605);
	WriteWord(0x2533);
	WriteWord(0x0F);
	
	LCD_ILI9341_CMD(ILI9341_PIXFORMATSET_REG);   // 0x3A
	LCD_ILI9341_Parameter(0x55);                 // 16-bit
	LCD_ILI9341_CMD(ILI9341_MEMACCESS_REG);      // 0x36
	//	LCD_ILI9341_Parameter(0x08);				// kdt: RGB -> BGR Notwendig f�r neues TFT_Proto Display
	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(ILI9341_INTERFCTL_REG);      // 0xF6
	WriteWord(0x130);
	
	//	LCD_ILI9341_CMD(ILI9341_INVERSIONON_REG);    // bei den neuen Displays erforderlich
	

	LCD_ILI9341_CMD(ILI9341_DISPLAYON_REG);      // 0x29 Exit Sleep   
	LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);	     // 0x11
	delay(120);		
		
    //
    // Set the display size and ensure that the GRAM window is set to allow
    // access to the full display buffer.
    //
    LCD_ILI9341_CMD(ILI9341_COLADDRSET_REG);     // 0x2A
    WriteWord(0);
    WriteWord(ILI9341::HORIZONTAL_MAX - 1);
    LCD_ILI9341_CMD(ILI9341_PAGEADDRSET_REG);    // 0x2B
    WriteWord(0);
    WriteWord(ILI9341::VERTICAL_MAX - 1);
}



void ILI9341::SpiXfer(uint8_t Outwert)
{
    resetPinCS();
	if (Outwert & 0x80) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x40) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x20) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x10) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x08) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x04) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x02) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();	
	if (Outwert & 0x01) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	resetPinSCK();
    setPinCS();
}

void ILI9341::WriteWord (uint16_t Outwert)
{
    setPinDC();
	resetPinCS();
	if (Outwert & 0x8000) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x4000) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x2000) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x1000) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x0800) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x0400) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x0200) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();	
	if (Outwert & 0x0100) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	resetPinSCK();
	setPinCS();
	resetPinCS();
	if (Outwert & 0x80) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x40) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x20) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x10) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x08) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x04) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	if (Outwert & 0x02) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();	
	if (Outwert & 0x01) {resetPinSCK(); setPinMosi();} else {resetPinSCK(); resetPinMosi();}
	setPinSCK();
	resetPinSCK();
	setPinCS();
}

void ILI9341::LCD_ILI9341_CMD(uint8_t Command)
{
    this->resetPinDC();
	this->SpiXfer(Command);
}

void ILI9341::LCD_ILI9341_Parameter(uint8_t Parameter)
{
    this->setPinDC();
	this->SpiXfer(Parameter);
}

/*
	Schaltet das Display ein oder aus.
	Im ausgeschaltetem Zustand wird ein leeres Display gezeigt.
	Das Bild im Speicher wird nicht verändert.
*/
void ILI9341::setDisplayOn(bool statusOn)
{
	if(statusOn)
	{
		this->LCD_ILI9341_CMD(ILI9341_DISPLAYON_REG);
	}else{
		this->LCD_ILI9341_CMD(ILI9341_DISPLAYOFF_REG); 
	}
}
/*
	Das Display wird in einen minimalen Powermodus versetzt. 
	DC/DC Wandler, intener oszillator und das Panel scanning werden gestoppt.
*/
void ILI9341::enterSleepMode()
{
	this->LCD_ILI9341_CMD(ILI9341_SLEEP_ENTER_REG);
}

/*
	Das Display ist wieder voll Einsatzbereit.
	DC/DC Wandler, intener oszillator und das Panel scanning werden gestartet.
	Danach muss 500ms gewartet werden.
*/
void ILI9341::exitSleepMode()
{
	this->LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);		
    delay(500); 
}

/*
	Invertiert das Display oder stellt die normalen Farben wider her.
*/
void ILI9341::invertDisplay(bool inverse)
{
	if(inverse)
	{
		this->LCD_ILI9341_CMD(ILI9341_INVERSIONON_REG);  
	}else{
		this->LCD_ILI9341_CMD(ILI9341_INVERSIONOFF_REG);
	}
}

/*
	Setzt die Rotation des Display.
	Diese kann um 180° gedreht werden.
*/
void ILI9341::rotateDisplay(bool rotateDisp)
{
	if(rotateDisp)
	{
		this->displayFunctionControlREG |= 0x0020; // SS-Bit setzen
		this->displayFunctionControlREG &= ~0x0040; // GS-Bit rücksetzen
	}else{
		this->displayFunctionControlREG &= ~0x0020; // SS-Bit rücksetzen
		this->displayFunctionControlREG |= 0x0040; // GS-Bit setzen
	}
	this->LCD_ILI9341_CMD(ILI9341_FUNCTONCTL_REG);     // 0xB6
	this->WriteWord(this->displayFunctionControlREG);
}

/*
	Setzt einen rechteckigen Bereich auf dem Display fest, welcher als nächstes beschrieben wird.
*/
void ILI9341::setCurPos(unsigned long ulStartX, unsigned long ulEndX, unsigned long ulStartY, unsigned long ulEndY)
{
    //
	// Check Arguments.
	//
	//  xASSERT((ulStartX < ulEndX) && (ulStartY < ulEndY));
	//  xASSERT((ulEndX < LCD_HORIZONTAL_MAX) && (ulEndY < LCD_VERTICAL_MAX));
	//
	// 	Tell the controller we are about to write data into its RAM.
	//
	this->LCD_ILI9341_CMD(ILI9341_COLADDRSET_REG);
	this->WriteWord(ulStartX);
	this->WriteWord(ulEndX);
	this->LCD_ILI9341_CMD(ILI9341_PAGEADDRSET_REG);
	this->WriteWord(ulStartY);
	this->WriteWord(ulEndY);
	this->LCD_ILI9341_CMD(ILI9341_MEMORYWRITE_REG);
}

/*
	Füllt das ganze Display mit einer Farbe.
*/
void ILI9341::fillScreen(int color)
{
    unsigned long ulCount;
    LCD_ILI9341_CMD(ILI9341_MEMORYWRITE_REG);    // 0x2C
    for(ulCount = 0; ulCount < (ILI9341::HORIZONTAL_MAX * ILI9341::VERTICAL_MAX); ulCount++)
    {
        WriteWord(color);
    }
}

/*
	Zeichnet einen einzelnen Pixel auf dem Display.
*/
void ILI9341::drawOnePixel(unsigned long usX, unsigned long usY, unsigned long ulColor)
{
	this->setCurPos(usX, usX, usY, usY);
	this->WriteWord(ulColor);
}

/*
	Zeichnet eine horizontale Line auf dem Display.
*/
void ILI9341::drawHorizontalLine(unsigned short usStartX, unsigned short usEndX, unsigned short usY, unsigned long ulColor)
{
	this->setCurPos(usStartX, usEndX, usY, usY);
	for(int i = 0; i <= usEndX-usStartX; i++)
	{
		this->WriteWord(ulColor);
	}
}

/*
	Zeichnet eine vertikale Line auf dem Display.
*/
void ILI9341::drawVerticalLine(unsigned short usX, unsigned short usStartY, unsigned short usEndY, unsigned long ulColor)
{
	this->setCurPos(usX, usX, usStartY, usEndY);
	for(int i = 0; i <= usEndY-usStartY; i++)
	{
		this->WriteWord(ulColor);
	}
}

/*
	Zeichnet einen rechteckigen Rahmen.
	Die Rahmendicke ist variable und wächst mit größerer Dicke nach innen an.
*/
void ILI9341::drawEmptyRect(unsigned long usStartX, unsigned long usStartY, unsigned long sizeX, unsigned long sizeY, unsigned long ulColor, byte frameSize)
{
	// oberen Rahmen
	this->drawFillRect(usStartX, usStartY, sizeX, frameSize, ulColor);
	// linken Rahmen
	this->drawFillRect(usStartX, usStartY, frameSize, sizeY, ulColor);
	// rechten Rahmen
	this->drawFillRect(usStartX+sizeX-frameSize, usStartY, frameSize, sizeY, ulColor);
	// unteren Rahmen
	this->drawFillRect(usStartX, usStartY+sizeY-frameSize, sizeX, frameSize, ulColor);
}

/*
	Zeichnet einen rechteckigen Rahmen.
	Die Rahmendicke ist variable und wächst mit größerer Dicke nach innen an.
*/
void ILI9341::drawEmptyRect2(unsigned long usStartX, unsigned long usEndX, unsigned long usStartY, unsigned long usEndY, unsigned long ulColor, byte frameSize)
{
	// oberen Rahmen
	this->drawFillRect2(usStartX, usEndX, usStartY, usStartY+frameSize, ulColor);
	// linken Rahmen
	this->drawFillRect2(usStartX, usStartX+frameSize, usStartY, usEndY, ulColor);
	// rechten Rahmen
	this->drawFillRect2(usEndX-frameSize, usEndX, usStartY, usEndY, ulColor);
	// unteren Rahmen
	this->drawFillRect2(usStartX, usEndX, usEndY-frameSize, usEndY, ulColor);
}

/*
	Zeichnet ein gefülltes Rechteck auf das Display.
*/
void ILI9341::drawFillRect2(unsigned long usStartX, unsigned long usEndX, unsigned long usStartY, unsigned long usEndY, unsigned long ulColor)
{
    unsigned long ulTemp;

	//
	// Check Arguments.
	//
	//    xASSERT((usStartX <= ulEndX)                &&
	//            (usStartY <= ulEndY)                &&
	//            ((usStartX >= 0) || (usEndX < LCD_HORIZONTAL_MAX)) &&
	//            ((usStartY >= 0) || (usEndY < LCD_VERTICAL_MAX)));            

	//
	// Clear LCD
	//
	this->setCurPos(usStartX, usEndX, usStartY, usEndY); 

	for(ulTemp = 0; ulTemp <= (usEndX - usStartX + 1) * (usEndY - usStartY + 1); ulTemp++)
	{
		this->WriteWord(ulColor);
	}
}

/*
	Zeichnet ein gefülltes Rechteck auf das Display.
*/
void ILI9341::drawFillRect(unsigned long usStartX, unsigned long usStartY, 
                   unsigned long SizeX, unsigned long SizeY, unsigned long ulColor)
{
	unsigned long ulTemp;

	//
	// Check Arguments.
	//
	//    xASSERT((usStartX <= ulEndX)                &&
	//            (usStartY <= ulEndY)                &&
	//            ((usStartX >= 0) || (usEndX < LCD_HORIZONTAL_MAX)) &&
	//            ((usStartY >= 0) || (usEndY < LCD_VERTICAL_MAX)));            

	//
	// Clear LCD
	//
	this->setCurPos(usStartX, usStartX+SizeX-1, usStartY, usStartY+SizeY-1);

	for(ulTemp = 0; ulTemp <= SizeX * SizeY; ulTemp++)
	{
		this->WriteWord(ulColor);
	}
}


/*
	Zeichnet ein Bild an die angegeben Position.
	Das Bild ist ganzahlig Skallierbar (default: size = 1).
*/
void ILI9341::drawBmp(unsigned short usX, unsigned short usY, unsigned short usSizeX, unsigned short usSizeY, uint16_t const *Bmp, byte size)
{
	unsigned short i,j;

    this->setCurPos(usX, usX + usSizeX*size-1, usY, usY + usSizeY*size-1); 
	
    for( i = 0; i < usSizeY*size; i++) 
    {
        for( j = 0 ; j < usSizeX*size; j++)
        {
            this->WriteWord(Bmp[i/size*usSizeX + j/size]);
        }
    }
}


#include "display/X_Tft_Zeichensatz_8X16_96.hpp"
/*
	Schreibt ein Zeichen auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel Pro Zeichen. 
	Die Zeichen können ganzahlig dynamisch skalliert werden.
	Die kleinste größe ist mit size = 1. 
*/
void ILI9341::printChar(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor, byte size)
{
	unsigned int i, j;
    
	if ((c >= 0x20) && (c <= 0x7F)) // Zeichen nach ASCII zwischen "Space" und "DEL"
	{
		this->setCurPos(usX, usX + 8*size - 1, usY, usY + 16*size - 1);

		for(i = 0; i < 16*size; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i/size]; // Ein Zeichen besteht aus 16 Zeilen mit jeweils 8 Bildpunkten
			for(j = 0; j < 8; j++) 
			{
				for(unsigned int k=0; k < size; k++)
				{
					if((m & 0x80) == 0x80) 
					{
						this->WriteWord(fColor);
					}
					else 
					{
						this->WriteWord(bColor);
					}
				}
				
				m <<= 1;
			}
		}
	}
}

/*
	Schreibt ein Zeichen auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen.
*/
void ILI9341::printChar8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
{
	unsigned int i, j;
    
	if ((c >= 0x20) && (c <= 0x7F)) // Zeichen nach ASCII zwischen "Space" und "DEL"
	{
		this->setCurPos(usX, usX + 8 - 1, usY, usY + 16 - 1);

		for(i = 0; i < 16; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i]; // Ein Zeichen besteht aus 16 Zeilen mit jeweils 8 Bildpunkten
			for(j = 0; j < 8; j++) 
			{
				if((m & 0x80) == 0x80) 
				{
					this->WriteWord(fColor);
				}
				else 
				{
					this->WriteWord(bColor);
				}
				m <<= 1;
			}
		}
	}
}

/*
	Schreibt ein Zeichen auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen.
	Die Zeichen werden skalliert auf 16x32 Pixel pro Zeichen.
*/
void ILI9341::printChar16x32(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
{
	unsigned int i, j;

  	if ((c >= 0x20) && (c <= 0x7F))
	{		
		this->setCurPos(usX, usX + 2*8 - 1, usY, usY + 2*16 - 1);
		
		for(i = 0; i < 32; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i/2];
			for(j = 0; j < 8; j++) 
			{
				if((m & 0x80) == 0x80) 
				{
					WriteWord(fColor);
					WriteWord(fColor);
				}
				else 
				{
					WriteWord(bColor);
					WriteWord(bColor);
				}
				m <<= 1;
			}
		}
	}
}

/*
	Schreibt ein Zeichen auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen.
	Die Zeichen werden skalliert auf 16x24 Pixel pro Zeichen.
*/
void ILI9341::printChar16x24(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
{
	unsigned int i, j;
    
  	if ((c >= 0x20) && (c <= 0x7F))
	{		
    	this->setCurPos(usX, usX + 2*8 - 1, usY, usY + 2*16 - 1);
	
		for(i = 2; i < 26; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i/2];
			for(j = 0; j < 8; j++) 
			{
				if((m & 0x80) == 0x80) 
				{
					WriteWord(fColor);
				}
				else 
				{
					WriteWord(bColor);
				}
				if((m & 0x80) == 0x80) 
				{
					WriteWord(fColor);
				}
				else 
				{
					WriteWord(bColor);
				}
				m <<= 1;
			}
		}
	}	
}

/*
	Schreibt ein Zeichen auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen.
	Die Zeichen werden skalliert auf 12x24 Pixel pro Zeichen.
*/
void ILI9341::printChar12x24(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
{
	unsigned int i;

  	if ((c >= 0x20) && (c <= 0x7F))
	{		
		this->setCurPos(usX, usX + 12 - 1, usY, usY + 2*12 - 1);
		
		for(i = 0; i < 24; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + 2*i/3];
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
			if((m & 0x80) == 0x80){WriteWord(fColor);}else {WriteWord(bColor);}
			m <<= 1;
		}
	}
}

/*
	Schreibt eine Zeichenkette auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen. 
	Die Zeichen können ganzahlig dynamisch skalliert werden.
	Die kleinste größe ist mit size = 1. 
*/
void ILI9341::printString(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor, byte size)
{
	unsigned char ucl = 0;
	
	while(*pcString) 
	{
		if( *pcString < 0xC0) 
		{
			this->printChar(usX + ucl*8*size, usY, *pcString, fColor, bColor, size);
			pcString++; ucl++;
		}
		else
		{
			pcString++; ucl++;
		}
	}
}

/*
	Schreibt eine Zeichenkette auf das Display.
	Verwendet wird der Zeichensatz, mit 8x16 Pixel pro Zeichen. 
*/
void ILI9341::printString8x16(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor)
{
	unsigned char ucl = 0;
	
	while(*pcString) 
	{
		if( *pcString < 0xC0) 
		{
			this->printChar8x16(usX + ucl * 8, usY, *pcString, fColor, bColor);
			pcString++; ucl++;
		}
		else
		{
			pcString++; ucl++;
		}
	}
}

/*
	Schreibt eine Zeichenkette auf das Display.
	Verwendet wird der Zeichensatz, mit 16x32 Pixel pro Zeichen. 
*/
void ILI9341::printString16x32(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor)
{
	unsigned char ucl = 0;
    while(*pcString) 
    {
        if( *pcString < 0xC0)
        {
            this->printChar16x32(usX + ucl * 2 * 8, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
            pcString++; ucl++;		
        }	
    }
}

/*
	Schreibt eine Zeichenkette auf das Display.
	Verwendet wird der Zeichensatz, mit 16x24 Pixel Pro Zeichen. 
*/
void ILI9341::printString16x24(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor)
{
	unsigned char ucl = 0;
    while(*pcString) 
    {
        if( *pcString < 0xC0)
        {
            this->printChar16x24(usX + ucl * 2 * 8, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
            pcString++; ucl++;		
        }	
    }
}

/*
	Schreibt eine Zeichenkette auf das Display.
	Verwendet wird der Zeichensatz, mit 12x24 Pixel Pro Zeichen. 
*/
void ILI9341::printString12x24(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor)
{
	unsigned char ucl = 0;
    while(*pcString) 
    {
        if( *pcString < 0xC0)
        {
            this->printChar12x24(usX + ucl * 12, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
            pcString++; ucl++;		
        }	
    }
}

// ########################################################################
/*
    Es folgen alle inline definition von Methoden zum setzen und rücksetzen der Display Pins.
    Aufgrund von einbußen der Geschwindigkeit bekommt jeder pin eine eigene Methode.
	Das Ermittelns des GPIO Pins und Register aus einer Pinbezeichnung wie z.B. PB8 daurte zu lange.
	Die Folge: Das Display baute sich sehr langsam auf.
*/

inline void ILI9341::setPinCS()
{
    this->GPIO_PORT_CS->BSRR = this->GPIO_PIN_CS;
}
inline void ILI9341::resetPinCS()
{
    this->GPIO_PORT_CS->BRR = this->GPIO_PIN_CS;
}

inline void ILI9341::setPinReset()
{
    this->GPIO_PORT_RESET->BSRR = this->GPIO_PIN_RESET;
}
inline void ILI9341::resetPinReset()
{
    this->GPIO_PORT_RESET->BRR = this->GPIO_PIN_RESET;
}

inline void ILI9341::setPinMosi()
{
    this->GPIO_PORT_MOSI->BSRR = this->GPIO_PIN_MOSI;
}
inline void ILI9341::resetPinMosi()
{
    this->GPIO_PORT_MOSI->BRR = this->GPIO_PIN_MOSI;
}

inline void ILI9341::setPinDC()
{
    this->GPIO_PORT_DC->BSRR = this->GPIO_PIN_DC;
}
inline void ILI9341::resetPinDC()
{
    this->GPIO_PORT_DC->BRR = this->GPIO_PIN_DC;
}

inline void ILI9341::setPinSCK()
{
    this->GPIO_PORT_SCK->BSRR = this->GPIO_PIN_SCK;
}
inline void ILI9341::resetPinSCK()
{
    this->GPIO_PORT_SCK->BRR = this->GPIO_PIN_SCK;
}

// ########################################################################

/*
	Generiert aus den RGB Farben (0-255), den 16-Bit Farbcode fürs Display.
*/
unsigned long ILI9341::makeColor(byte R, byte G, byte B)
{
	// (MSB) 4 Zero Bits, Blau (5 Bit) , Grün (6 Bit), Rot (5 Bit) (LSB)
	unsigned long color = 0;
	R = round(R/255.0*31.0); // roter Anteil
	G = round(G/255.0*63.0); // grüner Anteil
	B = round(B/255.0*31.0); // blauer Anteil
	color = R + (G << 5) + (B << 11);
	return color;
}