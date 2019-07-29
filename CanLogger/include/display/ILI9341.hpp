#ifndef ILI9341_HPP
#define ILI9341_HPP

#include <Arduino.h>

// 16 Bit RGB Colors
// Blue (5 Bit) , Green (6 Bit), Red (5 Bit) (LSB)
enum Color {
    RED100 =        0x001FUL,
    GREEN100 =      0x07E0UL,
    GREEN50 =       0x03E0UL,
    GREEN25 =       0x01E0UL,
    BLUE100 =       0xF800UL,
    BLUE50 =        0x7800UL,
    WHITE =         0xFFFFUL,
    BLACK =         0x0000UL,
    YELLOW100 =     0x07FFUL,
    YELLOW50 =      0x01CFUL,
    YELLOW25 =      0x01CFUL,
    GREY100 =       0x2A69UL,
    GREY50 =        0x7BEFUL,
    GREY25 =        0x3AE7UL,
    MAGENTA100 =    0xF81FUL,
    CYAN100 =       0xFFE0UL,
    GREYBROWN =     0x1965UL,
    GREENBROWN =    0x3BE0UL,
    ORANGE =        0x013DUL
};

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

    uint16_t displayFunctionControlREG = 0x0AC2; // default: No rotation and black display when powered off.

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

    // Statische Deklarationen und Definitionen
    static constexpr uint16_t HORIZONTAL_MAX = 240;
    static constexpr uint16_t VERTICAL_MAX = 320;
    static unsigned long makeColor(byte R, byte G, byte B);
};



#endif