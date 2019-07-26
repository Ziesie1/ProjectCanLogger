#include <Arduino.h>
#include "display/ILI9341.hpp"
#include "display/ILI9341_Definitions.hpp"
#include "serial/SerialCommunication.hpp"
#include "serial/FunctionLifeTime.hpp"


#define TFT_ROTATION    //setzen der Rotation

using namespace utilities;

ILI9341::ILI9341(const uint32_t& CS, const uint32_t& RESET,const uint32_t& MOSI, const uint32_t& DC, const uint32_t& SCK)
    : PIN_CS{CS}, PIN_RESET{RESET}, PIN_MOSI{MOSI}, PIN_DC{DC}, PIN_SCK{SCK}
{
    
}

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

	scom.printDebug("Display ist Initialisiert.");
}

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
	#ifdef TFT_ROTATION
	WriteWord(0x0AA2); // Normal, kleines Display
	#else
	WriteWord(0x0AC2); // Normal, kleines Display
	#endif
	
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
    WriteWord(LCD_HORIZONTAL_MAX - 1);
    LCD_ILI9341_CMD(ILI9341_PAGEADDRSET_REG);    // 0x2B
    WriteWord(0);
    WriteWord(LCD_VERTICAL_MAX - 1);
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
	LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);		
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
    for(ulCount = 0; ulCount < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); ulCount++)
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
	Zeichnet ein Bild an der angegeben Position.
*/
void ILI9341::drawBmp(unsigned short usX, unsigned short usY, unsigned short usSizeX, unsigned short usSizeY, uint16_t const *Bmp)
{
	unsigned short i,j;
    
    this->setCurPos(usX, usX + usSizeX-1, usY, usY + usSizeY-1); 
	
    for( i = usY; i < usY + usSizeY; i++) 
    {
        for( j = usX ; j < usX + usSizeX; j++)
        {
            this->WriteWord(*Bmp++);  
        }
    }
}


#include "display/X_Tft_Zeichensatz_8X16_96.hpp"

/*

*/
void ILI9341::printChar8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
{
	unsigned int i, j;
    
	if ((c >= 0x20) && (c <= 0x7F))
	{
		this->setCurPos(usX, usX + 8 - 1, usY, usY + 16 - 1);

		for(i = 0; i < 16; i++) 
		{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i];
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

*/
void ILI9341::printChar32_8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
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

void ILI9341::printChar32_24_8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
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

void ILI9341::printChar24_8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor)
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


void ILI9341::printString(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor)
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
	//           ILI9341PutGB1616(usX + ucl * 8, usY, (unsigned char*)pcString, fColor, bColor);
	//           pcString += 2; ucl += 2;
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