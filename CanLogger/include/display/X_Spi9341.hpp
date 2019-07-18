/** 
*	@file ili9341.h
*	@brief Definition der externen Schnittstellen zum Modul
*	@author Karl-Dieter Tieste
*	@date 2018-05-04
*/

/*===================================================================================================================
 *	Defines
 *=================================================================================================================*/
#ifndef SPI_9341_H_
#define SPI_9341_H_

#define ROT100 		0x0000001FUL
#define GRUEN100 	0x000007E0UL
#define GRUEN50 	0x000003E0UL
#define GRUEN25 	0x000001E0UL
#define BLAU100 	0x0000F800UL
#define BLAU50 	  0x00007800UL
#define WEISS 		0x0000FFFFUL
#define SCHWARZ 	0x00000000UL
#define GELB100 	0x000007FFUL
#define GRAU 			0x00002A69UL   	// Nachtr�glich eingef�gt
#define GELB50  	0x000003eFUL
#define GELB25  	0x000001CFUL
#define GRAU50      0x00007BEFUL
#define GRAU25      0x00003AE7UL
#define MAGENTA100  0x0000F81FUL
#define CYAN100     0x0000FFE0UL
#define GRAUBRAUN   0x00001965UL
#define GRUENBLAU   0x00003BE0UL

#define ILI9341_DEVICE_CODE_READ_REG	0x00
#define ILI9341_SOFT_RESET_REG  			0x01
#define ILI9341_IDENTINFO_R_REG 			0x04
#define ILI9341_STATUS_R_REG    			0x09
#define ILI9341_POWERMODE_R_REG 			0x0A
#define ILI9341_MADCTL_R_REG    			0x0B
#define ILI9341_PIXFORMAT_R_REG 			0x0C
#define ILI9341_IMGFORMAT_R_REG 			0x0D
#define ILI9341_SIGMODE_R_REG   			0x0E	
#define ILI9341_SD_RESULT_R_REG 			0x0F
#define ILI9341_SLEEP_ENTER_REG 			0x10
#define ILI9341_SLEEP_OUT_REG   			0x11
#define ILI9341_PARTIALMODE_REG 			0x12
#define ILI9341_NORDISPMODE_REG 			0x13
#define ILI9341_INVERSIONOFF_REG      0x20
#define ILI9341_INVERSIONON_REG 			0x21
#define ILI9341_GAMMASET_REG    			0x26
#define ILI9341_DISPLAYOFF_REG  			0x28
#define ILI9341_DISPLAYON_REG   			0x29
#define ILI9341_COLADDRSET_REG  			0x2A
#define ILI9341_PAGEADDRSET_REG 			0x2B
#define ILI9341_MEMORYWRITE_REG 			0x2C
#define ILI9341_COLORSET_REG    			0x2D
#define ILI9341_MEMORYREAD_REG  			0x2E
#define ILI9341_PARTIALAREA_REG 			0x30
#define ILI9341_VERTSCROLL_REG  			0x33
#define ILI9341_TEAREFFECTLINEOFF_REG 0x34
#define ILI9341_TEAREFFECTLINEON_REG  0x35
#define ILI9341_MEMACCESS_REG   			0x36
#define ILI9341_VERSCRSRART_REG 			0x37
#define ILI9341_IDLEMODEOFF_REG 			0x38
#define ILI9341_IDLEMODEON_REG  			0x39							
#define ILI9341_PIXFORMATSET_REG      0x3A
#define ILI9341_WRITEMEMCONTINUE_REG  0x3C
#define ILI9341_READMEMCONTINUE_REG   0x3E
#define ILI9341_SETTEATSCAN_REG 			0x44	
#define ILI9341_GETSCANLINE_REG 			0x45
#define ILI9341_WRITEBRIGHT_REG 			0x51
#define ILI9341_READBRIGHT_REG  			0x52
#define ILI9341_WRITECTRL_REG   			0x53
#define ILI9341_READCTRL_REG    			0x54  
#define ILI9341_WRITECABC_REG   			0x55
#define ILI9341_READCABC_REG    			0x56                                         
#define ILI9341_WRITECABCMB_REG 			0x5E
#define ILI9341_READCABCMB_REG  			0x5F
#define ILI9341_RGB_ISCTL_REG   			0xB0
#define ILI9341_FRAMECTL_NOR_REG     	0xB1
#define ILI9341_FRAMECTL_IDLE_REG    	0xB2
#define ILI9341_FRAMECTL_PARTIAL_REG  0xB3
#define ILI9341_INVERCTL_REG    			0xB4
#define ILI9341_BLANKPORCTL_REG 			0xB5
#define ILI9341_FUNCTONCTL_REG  			0xB6
#define ILI9341_ENTRYMODE_REG   			0xB7
#define ILI9341_BLIGHTCTL1_REG  			0xB8
#define ILI9341_BLIGHTCTL2_REG  			0xB9
#define ILI9341_BLIGHTCTL3_REG  			0xBA
#define ILI9341_BLIGHTCTL4_REG  			0xBB
#define ILI9341_BLIGHTCTL5_REG  			0xBC
#define ILI9341_BLIGHTCTL7_REG  			0xBE
#define ILI9341_BLIGHTCTL8_REG  			0xBF
#define ILI9341_POWERCTL1_REG   			0xC0
#define ILI9341_POWERCTL2_REG   			0xC1
#define ILI9341_VCOMCTL1_REG    			0xC5
#define ILI9341_VCOMCTL2_REG    			0xC7
#define ILI9341_POWERCTLA_REG   			0xCB
#define ILI9341_POWERCTLB_REG   			0xCF
#define ILI9341_NVMEMWRITE_REG  			0xD0
#define ILI9341_NVMEMPROTECTKEY_REG   0xD1
#define ILI9341_NVMEMSTATUS_REG 			0xD2
#define ILI9341_READID4_REG     			0xD3
#define ILI9341_READID1_REG     			0xDA
#define ILI9341_READID2_REG     			0xDB
#define ILI9341_READID3_REG     			0xDC
#define ILI9341_POSGAMMACORRECTION_REG 0xE0
#define ILI9341_NEGGAMMACORRECTION_REG 0xE1
#define ILI9341_DIGGAMCTL1_REG  			0xE2
#define ILI9341_DIGGAMCTL2_REG  			0xE3
#define ILI9341_DIVTIMCTL_A_REG 			0xE8
#define ILI9341_DIVTIMCTL_B_REG 			0xEA
#define ILI9341_POWONSEQCTL_REG 			0xED
#define ILI9341_ENABLE_3G_REG   			0xF2
#define ILI9341_INTERFCTL_REG   			0xF6
#define ILI9341_PUMPRATIOCTL_REG    	0xF7

#define LCD_HORIZONTAL_MAX 						240
#define LCD_VERTICAL_MAX 							320

/*===================================================================================================================
 *	Prototypen
 *=================================================================================================================*/
void Init_GPIO_SPI	(void);
void WriteWord (unsigned short data);
void ILI9341Init(void);
void ClearColor(int color);
void ILI9341DisplayClear(unsigned long ulColor);
void ILI9341DisplayRectFill(unsigned long usStartX, unsigned long usStartY,  //FALSCH! startx, endex, starty, endey
                                   unsigned long usEndX, unsigned long usEndY, 
                                   unsigned long ulColor);
void ILI9341DisplayRect(unsigned long usStartX, unsigned long usStartY, 
                        unsigned long SizeX, unsigned long SizeY, unsigned long ulColor);
void ILI9341DisplayInversionOn(void);
void ILI9341DisplayInversionOff(void);
void ILI9341DrawOnePixel(unsigned long usX, unsigned long usY, 
                                  unsigned long ulValue);
void ILI9341DrawHorizontalLine(unsigned short usStartX, unsigned short usEndX,
                                      unsigned short usY, unsigned long ulColor);
void ILI9341DrawVerticalLine(unsigned short usX, unsigned short usStartY,
                                   unsigned short usEndY, unsigned long ulColor);
void ILI9341DrawDirectLine(unsigned short usStartX, unsigned short usEndX,
																unsigned short usStartY, unsigned short usEndY, unsigned long ulColor);
void ILI9341PutChar8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
void ILI9341PutChar32_8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
void ILI9341PutChar32_24_8x16(unsigned short usX, unsigned short usY, char c, unsigned long fColor, unsigned long bColor);
void ILI9341PutChar24_8x16(unsigned short usX, unsigned short usY, char c, 
                                unsigned long fColor, unsigned long bColor);
void ILI9341DisplayString(unsigned short usX, unsigned short usY, const char* pcString,
                                  unsigned long fColor, unsigned long bColor);
void ILI9341DisplayStringVertical(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor);
void ILI9341DisplayString32(unsigned short usX, unsigned short usY, const char* pcString,
                                  unsigned long fColor, unsigned long bColor);extern void ILI9341DisplayNum(unsigned short usX, unsigned short usY, unsigned long ulNum,
                               unsigned long fColor, unsigned long bColor);
void ILI9341DisplayString32_24(unsigned short usX, unsigned short usY, const char* pcString,
                                  unsigned long fColor, unsigned long bColor);extern void ILI9341DisplayNum(unsigned short usX, unsigned short usY, unsigned long ulNum,
                               unsigned long fColor, unsigned long bColor);
void ILI9341DisplayString24(unsigned short usX, unsigned short usY, const char* pcString,
                                  unsigned long fColor, unsigned long bColor);extern void ILI9341DisplayNum(unsigned short usX, unsigned short usY, unsigned long ulNum,
                               unsigned long fColor, unsigned long bColor);
void ILI9341DisplayString15(unsigned short usX, unsigned short usY, const char* pcString,
                                  unsigned long fColor, unsigned long bColor);extern void ILI9341DisplayNum(unsigned short usX, unsigned short usY, unsigned long ulNum,
                               unsigned long fColor, unsigned long bColor);
void ILI9341PutGB1616(unsigned short usX, unsigned short usY, unsigned char ucChinese[2], 
                       unsigned long fColor,unsigned long bColor);
void ILI9341SetCurPos(unsigned long ulStartX, unsigned long ulEndX, 
                  unsigned long ulStartY, unsigned long ulEndY);


#endif /* SPI_9341_H_ */
