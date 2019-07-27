#ifndef ILI9341_HPP
#define ILI9341_HPP

// Die Farbe wird im 16 Bit Mode übertragen
// (MSB) 4 Zero Bits, Blau (5 Bit) , Grün (6 Bit), Rot (5 Bit) (LSB)
#define ROT100 		0x0000001FUL
#define GRUEN100 	0x000007E0UL
#define GRUEN50 	0x000003E0UL
#define GRUEN25 	0x000001E0UL
#define BLAU100 	0x0000F800UL
#define BLAU50 	    0x00007800UL
#define WEISS 		0x0000FFFFUL
#define SCHWARZ 	0x00000000UL
#define GELB100 	0x000007FFUL
#define GRAU 		0x00002A69UL
#define GELB50  	0x000003eFUL
#define GELB25  	0x000001CFUL
#define GRAU50      0x00007BEFUL
#define GRAU25      0x00003AE7UL
#define MAGENTA100  0x0000F81FUL
#define CYAN100     0x0000FFE0UL
#define GRAUBRAUN   0x00001965UL
#define GRUENBLAU   0x00003BE0UL
#define ORANGE      0x0000013DUL


#define LCD_HORIZONTAL_MAX 					240
#define LCD_VERTICAL_MAX 					320

class ILI9341
{
    const uint32_t PIN_CS;
    const uint32_t PIN_RESET;
    const uint32_t PIN_MOSI; // MOSI or SDI
    const uint32_t PIN_DC; // DC or RS
    const uint32_t PIN_SCK;

    uint32_t GPIO_PIN_CS;
    GPIO_TypeDef * GPIO_PORT_CS;
    uint32_t GPIO_PIN_RESET;
    GPIO_TypeDef * GPIO_PORT_RESET;
    uint32_t GPIO_PIN_MOSI;
    GPIO_TypeDef * GPIO_PORT_MOSI;
    uint32_t GPIO_PIN_DC;
    GPIO_TypeDef * GPIO_PORT_DC;
    uint32_t GPIO_PIN_SCK;
    GPIO_TypeDef * GPIO_PORT_SCK;

    uint16_t displayFunctionControlREG = 0x0AC2; // Keine Rotation und Schwarz bei ausgeschaltetem Display.

    inline void setPinCS();
    inline void resetPinCS();
    inline void setPinReset();
    inline void resetPinReset();
    inline void setPinMosi();
    inline void resetPinMosi();
    inline void setPinDC();
    inline void resetPinDC();
    inline void setPinSCK();
    inline void resetPinSCK();

    // Methoden zur Initialisierung
    void gpioInit();
    void displayInit();

    // Methoden zum beschreiben des Displays
    void SpiXfer(uint8_t Outwert);
    void WriteWord (uint16_t Outwert);
    void LCD_ILI9341_CMD(uint8_t Command);
    void LCD_ILI9341_Parameter(uint8_t Parameter);
    void setCurPos(unsigned long ulStartX, unsigned long ulEndX, unsigned long ulStartY, unsigned long ulEndY);

public:
    ILI9341(const uint32_t& CS, const uint32_t& RESET,const uint32_t& MOSI, const uint32_t& DC, const uint32_t& SCK, bool rotateDisp = false);
    void init();
    void setDisplayOn(bool statusOn);
    void enterSleepMode();
    void exitSleepMode();
    void invertDisplay(bool inverse);
    void rotateDisplay(bool rotateDisp);

    void fillScreen(int color);
    void drawOnePixel(unsigned long usX, unsigned long usY, unsigned long ulColor);
    void drawHorizontalLine(unsigned short usStartX, unsigned short usEndX, unsigned short usY, unsigned long ulColor);
    void drawVerticalLine(unsigned short usX, unsigned short usStartY, unsigned short usEndY, unsigned long ulColor);
    void drawEmptyRect(unsigned long usStartX, unsigned long usStartY, unsigned long sizeX, unsigned long sizeY, unsigned long ulColor, byte frameSize = 1);
    void drawEmptyRect2(unsigned long usStartX, unsigned long usEndX, unsigned long usStartY, unsigned long usEndY, unsigned long ulColor, byte frameSize = 1);
    void drawFillRect(unsigned long usStartX, unsigned long usStartY, unsigned long SizeX, unsigned long SizeY, unsigned long ulColor);
    void drawFillRect2(unsigned long usStartX, unsigned long usEndX, unsigned long usStartY, unsigned long usEndY, unsigned long ulColor);
    void drawBmp(unsigned short usX, unsigned short usY, unsigned short usSizeX, unsigned short usSizeY, uint16_t const *Bmp, byte size = 1);    
    
    void printChar(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor, byte size = 1);
    void printChar8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
    void printChar16x32(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
    void printChar16x24(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
    void printChar12x24(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
    void printString(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor, byte size = 1);
    void printString8x16(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor);
    void printString16x32(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor);
    void printString16x24(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor);
    void printString12x24(unsigned short usX, unsigned short usY, const char* pcString, unsigned long fColor, unsigned long bColor);

    static unsigned long makeColor(byte R, byte G, byte B);
};



#endif