/** 
*	@file SPI_ILI9341.c 
*	@brief GPIO-SPI-Treiber f�r TFT-Display
*	@author Karl-Dieter Tieste
*	@date 2018-05-04
*/
/*===================================================================================================================
 *	Includes
 *=================================================================================================================*/
#include "display/X_Spi9341.hpp"
#include "display/TFT.hpp"
/*===================================================================================================================
 *	Defines
 *=================================================================================================================*/
#define ILI9341_CHARACTER_FONT_8X16

#define TFT_CHINA9341
#define TFT_ROTATION

/*===================================================================================================================
 *	Funktionen
 *=================================================================================================================*/
 void Init_GPIO_SPI(void)
{
	SET_TFT_CS_N;
	RES_TFT_SCK;
	RES_TFT_DC;
}

static void SpiXfer(uint8_t Outwert)
{
	RES_TFT_CS_N;
	if (Outwert & 0x80) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x40) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x20) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x10) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x08) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x04) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x02) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;	
	if (Outwert & 0x01) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	RES_TFT_SCK;
	SET_TFT_CS_N;
}

static void delayms(int delay)
{
	int i;
	while (delay > 0)
	{
		delay --;
		for (i = 0;i < 10000;i++)
		{
			__NOP();
			
		}
	}
}

static void LCD_ILI9341_Parameter(uint8_t Parameter)
{
	SET_TFT_DC;
	SpiXfer(Parameter);
}

static void LCD_ILI9341_CMD(uint8_t Command)
{
	RES_TFT_DC;
	SpiXfer(Command);
}


#ifdef TFT_PROTO
void ILI9341Init(void)
//void ILI9341Init_TFT_PROTO(void)
{
	// Version f�r TFT_PROTO auf dem Laboraufbau F303. 
	//    unsigned long ulCount;
	// VCI=2.8V
	//=== Reset LCD Driver ==============================================================================================
	SET_TFT_RESET_N;
	delayms(1); // Delay 1ms
	RES_TFT_RESET_N;
	delayms(10); // Delay 10ms // This delay time is necessary
	SET_TFT_RESET_N;
	delayms(120); // Delay 120 ms

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
//	LCD_ILI9341_Parameter(0x2e);//26
	LCD_ILI9341_Parameter(0x21);//26 Einstellung f�r TFT_PROTO Display
 
	LCD_ILI9341_CMD(ILI9341_POWERCTL2_REG);      // 0xC1
//	LCD_ILI9341_Parameter(0x12);	
	LCD_ILI9341_Parameter(0x11);	
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL1_REG);       // 0xC5 
	WriteWord(0x5019); //2b
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL2_REG);       // 0xC7 
	LCD_ILI9341_Parameter(0x90); //c4

	LCD_ILI9341_CMD(ILI9341_MEMACCESS_REG);      // 0x36
	LCD_ILI9341_Parameter(0xa8); // Original-Scanrichtung
//	LCD_ILI9341_Parameter(0xa0); // Original-Scanrichtung

	LCD_ILI9341_CMD(ILI9341_FUNCTONCTL_REG);     // 0xB6
	
		LCD_ILI9341_CMD(ILI9341_FUNCTONCTL_REG);     // 0xB6
	#ifdef TFT_ROTATION
	WriteWord(0x0AA2); // Normal, kleines Display
	#else
	WriteWord(0x0AC2); // Normal, kleines Display
	#endif

//	WriteWord(0x0AC2); // Kopf�ber f�r TFT_Proto 
//	WriteWord(0x0AA2); // Normal, kleines Display
	
	
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
	LCD_ILI9341_Parameter(0x08);				// kdt: RGB -> BGR Notwendig f�r neues TFT_Proto Display
//	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(ILI9341_INTERFCTL_REG);      // 0xF6
	WriteWord(0x130);
	
	LCD_ILI9341_CMD(ILI9341_INVERSIONON_REG);    // bei den neuen Displays erforderlich
	

	LCD_ILI9341_CMD(ILI9341_DISPLAYON_REG);      // 0x29 Exit Sleep   
	LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);	     // 0x11
	delayms(120);		
		
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
#endif // TFT_PROTO

#ifdef TFT_CHINA9341

void ILI9341Init(void)
//void ILI9341Init_CHINA(void)
{
	// Version f�r TFT_PROTO auf dem Laboraufbau F303. 
	//    unsigned long ulCount;
	// VCI=2.8V
	//=== Reset LCD Driver ==============================================================================================
	SET_TFT_RESET_N;
	delayms(1); // Delay 1ms
	RES_TFT_RESET_N;
	delayms(10); // Delay 10ms // This delay time is necessary
	SET_TFT_RESET_N;
	delayms(120); // Delay 120 ms

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
	delayms(120);		
		
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
#endif //TFT_CHINA9341


void ILI9341Init__(void)
{
	// Version f�r TFT_PROTO auf dem Laboraufbau F303. 
	//    unsigned long ulCount;
	// VCI=2.8V
	//=== Reset LCD Driver ==============================================================================================
	SET_TFT_RESET_N;
	delayms(1); // Delay 1ms
	RES_TFT_RESET_N;
	delayms(10); // Delay 10ms // This delay time is necessary
	SET_TFT_RESET_N;
	delayms(120); // Delay 120 ms

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
//	LCD_ILI9341_Parameter(0x2e);//26
	LCD_ILI9341_Parameter(0x21);//26 Einstellung f�r TFT_PROTO Display
 
	LCD_ILI9341_CMD(ILI9341_POWERCTL2_REG);      // 0xC1
//	LCD_ILI9341_Parameter(0x12);	
	LCD_ILI9341_Parameter(0x11);	
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL1_REG);       // 0xC5 
	WriteWord(0x5019); //2b
 
	LCD_ILI9341_CMD(ILI9341_VCOMCTL2_REG);       // 0xC7 
	LCD_ILI9341_Parameter(0x90); //c4

	LCD_ILI9341_CMD(ILI9341_MEMACCESS_REG);      // 0x36
	LCD_ILI9341_Parameter(0xa8); // Original-Scanrichtung
//	LCD_ILI9341_Parameter(0xa0); // Original-Scanrichtung

	LCD_ILI9341_CMD(ILI9341_FUNCTONCTL_REG);     // 0xB6
	WriteWord(0x0AC2); // Kopf�ber f�r TFT_Proto 
//	WriteWord(0x0AA2); // Normal, kleines Display
	
	
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
	LCD_ILI9341_Parameter(0x08);				// kdt: RGB -> BGR Notwendig f�r neues TFT_Proto Display
//	LCD_ILI9341_Parameter(0x00);
	LCD_ILI9341_CMD(ILI9341_INTERFCTL_REG);      // 0xF6
	WriteWord(0x130);
	
	LCD_ILI9341_CMD(ILI9341_INVERSIONON_REG);    // bei den neuen Displays erforderlich
	

	LCD_ILI9341_CMD(ILI9341_DISPLAYON_REG);      // 0x29 Exit Sleep   
	LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);	     // 0x11
	delayms(120);		
		
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

void ClearColor(int color)
{
    unsigned long ulCount;
    LCD_ILI9341_CMD(ILI9341_MEMORYWRITE_REG);    // 0x2C
    for(ulCount = 0; ulCount < (LCD_HORIZONTAL_MAX * LCD_VERTICAL_MAX); ulCount++)
    {
        WriteWord(color);
    } 
}

/*static*/ void WriteWord (uint16_t Outwert)
{
	SET_TFT_DC;
	RES_TFT_CS_N;
	if (Outwert & 0x8000) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x4000) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x2000) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x1000) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x0800) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x0400) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x0200) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;	
	if (Outwert & 0x0100) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	RES_TFT_SCK;
	SET_TFT_CS_N;
	RES_TFT_CS_N;
	if (Outwert & 0x80) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x40) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x20) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x10) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x08) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x04) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	if (Outwert & 0x02) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;	
	if (Outwert & 0x01) {RES_TFT_SCK; SET_TFT_MOSI;} else {RES_TFT_SCK; RES_TFT_MOSI;}
	SET_TFT_SCK;
	RES_TFT_SCK;
	SET_TFT_CS_N;
}

void SetAddress (unsigned x1, unsigned y1, unsigned x2, unsigned y2)
{
   LCD_ILI9341_CMD(0x2a);
	WriteWord(x1);
	WriteWord(x2);
   LCD_ILI9341_CMD(0x2b);
	WriteWord(y1);
	WriteWord(y2);
   LCD_ILI9341_CMD(0x2C);
}

//*****************************************************************************
//
//! \brief Set the cursor location.
//!
//! \param ulStartX is the character index.
//! \param ulEndX is the line number.
//!
//! The default location is (0,0). After set the location, the strings will
//! display at the (ulCol, ulRow).
//!
//! \return None.
//
//*****************************************************************************
void ILI9341SetCurPos(unsigned long ulStartX, unsigned long ulEndX, 
                  unsigned long ulStartY, unsigned long ulEndY)
{
	//
	// Check Arguments.
	//
	//  xASSERT((ulStartX < ulEndX) && (ulStartY < ulEndY));
	//  xASSERT((ulEndX < LCD_HORIZONTAL_MAX) && (ulEndY < LCD_VERTICAL_MAX));
	//
	// 	Tell the controller we are about to write data into its RAM.
	//
	LCD_ILI9341_CMD(ILI9341_COLADDRSET_REG);
	WriteWord(ulStartX);
	WriteWord(ulEndX);
	LCD_ILI9341_CMD(ILI9341_PAGEADDRSET_REG);
	WriteWord(ulStartY);
	WriteWord(ulEndY);
	LCD_ILI9341_CMD(ILI9341_MEMORYWRITE_REG);
}

/////////////////////////////////////////////////////////////////////////7
//*****************************************************************************
//
//! \brief Enter the the LCD sleep mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341EnterSleepMode(void)
{
    //
    // Sleep set
    //
    LCD_ILI9341_CMD(ILI9341_SLEEP_ENTER_REG);		
}

//*****************************************************************************
//
//! \brief Exit the the LCD sleep mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341ExitSleepMode(void)
{
    LCD_ILI9341_CMD(ILI9341_SLEEP_OUT_REG);		
    delayms(500); 
}

//*****************************************************************************
//
//! \brief Fills a rectangle (direct LCD access mode).
//!
//! \param ulStartX is the starting address of X.
//! \param ulStartY is the starting address of Y.
//! \param ulEndX is the ending address of X.
//! \param ulEndY is the ending address of Y.
//! \param ulColor is the color.
//!
//! This function fills a rectangle on the display.  The coordinates of the
//! rectangle are assumed to be within the extents of the display, and the
//! rectangle specification is fully inclusive (in other words, both ulStartX and
//! ulEndX are drawn, along with ulStartY and ulEndY).
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayRectFill(unsigned long usStartX, unsigned long usEndX, 
														unsigned long usStartY, unsigned long usEndY, unsigned long ulColor)
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
	ILI9341SetCurPos(usStartX, usEndX, usStartY, usEndY); 

	for(ulTemp = 0; ulTemp <= (usEndX - usStartX + 1) * (usEndY - usStartY + 1); ulTemp++)
	{
			WriteWord(ulColor);
	}
}

void 
ILI9341DisplayRect(unsigned long usStartX, unsigned long usStartY, 
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
	ILI9341SetCurPos(usStartX, usStartX+SizeX, usStartY, usStartY+SizeY); 

	for(ulTemp = 0; ulTemp <= (SizeX + 1) * (SizeY + 1); ulTemp++)
	{
			WriteWord(ulColor);
	}
}

//*****************************************************************************
//
//! \brief Clears all the LCD display with the specified colour.
//!
//! \param ulColor is the specified colour.
//!
//! It clears all the LCD display with the specified colour.
//!
//! \return None.
//
//*****************************************************************************
void 
ILI9341DisplayClear(unsigned long ulColor)
{
   ILI9341DisplayRectFill(0, LCD_HORIZONTAL_MAX - 1, 0, LCD_VERTICAL_MAX - 1, ulColor);
}

//*****************************************************************************
//
//! \brief Enter into display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayInversionOn(void)
{
  LCD_ILI9341_CMD(ILI9341_INVERSIONON_REG);   
}

//*****************************************************************************
//
//! \brief Recover from display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayInversionOff(void)
{
  LCD_ILI9341_CMD(ILI9341_INVERSIONOFF_REG);      
}

//*****************************************************************************
//
//! \brief Enter into display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayOn(void)
{
  LCD_ILI9341_CMD(ILI9341_DISPLAYON_REG);   
}

//*****************************************************************************
//
//! \brief Recover from display inversion mode.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayOff(void)
{
  LCD_ILI9341_CMD(ILI9341_DISPLAYOFF_REG);      
}


//*****************************************************************************
//
//! \brief Draws a pixel on the screen.
//!
//! \param ulX is the X coordinate of the pixel.
//! \param ulY is the Y coordinate of the pixel.
//! \param ulValue is the color of the pixel.
//!
//! This function sets the given pixel to a particular color.  The coordinates
//! of the pixel are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawOnePixel(unsigned long usX, unsigned long usY, unsigned long ulColor)                                 
{
	//
	// Write the pixel value.
	//
	ILI9341SetCurPos(usX, usX+1, usY, usY+1);
	WriteWord(ulColor);
}

//*****************************************************************************
//
//! \brief Draws a horizontal line.
//!
//! \param usStartX is the X coordinate of the start of the line.
//! \param usEndX is the X coordinate of the end of the line.
//! \param usY is the Y coordinate of the line.
//! \param usColor is the color of the line.
//!
//! This function draws a horizontal line on the display.  The coordinates of
//! the line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawHorizontalLine(unsigned short usStartX, unsigned short usEndX,
                          unsigned short usY, unsigned long ulColor)
{
	// xASSERT((usStartX <= usEndX) && (usEndX <= LCD_HORIZONTAL_MAX) && (usStartX >= 0));
    
	while(usStartX++ <= usEndX)
	{
			ILI9341DrawOnePixel(usStartX, usY, ulColor);
	}
}

//*****************************************************************************
//
//! \brief Draws a vertical line.
//!
//! \param ulX is the X coordinate of the line.
//! \param ulStartY is the Y coordinate of the start of the line.
//! \param ulEndY is the Y coordinate of the end of the line.
//! \param ulColor is the color of the line.
//!
//! This function draws a vertical line on the display.  The coordinates of the
//! line are assumed to be within the extents of the display.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DrawVerticalLine(unsigned short usX, unsigned short usStartY,
                        unsigned short usEndY, unsigned long ulColor)
{
	// xASSERT((usStartY <= usEndY) && (usEndY <= LCD_VERTICAL_MAX) && (usStartY >= 0));
    
	while(usStartY++ <= usEndY)
	{
			ILI9341DrawOnePixel(usX, usStartY, ulColor);
	}
}


//*****************************************************************************
//
//! \brief Display a char on the LCD.
//!
//! \param pcString is a non-terminated string that to display.
//!
//! Display a string on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
#ifdef ILI9341_CHARACTER_FONT_8X16
//#include "X_Tft_8X16.h"
#include "display/X_Tft_Zeichensatz_8X16_96.hpp"
void ILI9341PutChar8x15(unsigned short usX, unsigned short usY, char c, 
                        unsigned long fColor, unsigned long bColor)
{
   unsigned int i, j;
    
	// xASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));
  
  if ((c >= 0x20) && (c <= 0x7F))
	{		
	ILI9341SetCurPos(usX, usX + 8 - 1, usY, usY + 16 - 1);

	for(i = 0; i < 15; i++) 
	{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i];
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
					m <<= 1;
			}
	}
}
}

void ILI9341PutChar8x16(unsigned short usX, unsigned short usY, char c, 
                        unsigned long fColor, unsigned long bColor)
{
   unsigned int i, j;
    
	// xASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));
    
  if ((c >= 0x20) && (c <= 0x7F))
	{		
	ILI9341SetCurPos(usX, usX + 8 - 1, usY, usY + 16 - 1);

	for(i = 0; i < 16; i++) 
	{
			unsigned char m = Font8x16[(c - 0x20) * 16 + i];
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
					m <<= 1;
			}
	}
}
}

void ILI9341PutChar32_8x16(unsigned short usX, unsigned short usY, char c, 
                         unsigned long fColor, unsigned long bColor)
{
    unsigned int i, j;
    
//    xASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));
  if ((c >= 0x20) && (c <= 0x7F))
	{		
    
    ILI9341SetCurPos(usX, usX + 2*8 - 1, usY, usY + 2*16 - 1);
	
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
#endif

void ILI9341PutChar32_24_8x16(unsigned short usX, unsigned short usY, char c, 
                         unsigned long fColor, unsigned long bColor)
{
    unsigned int i, j;
    
//    xASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));

    
  if ((c >= 0x20) && (c <= 0x7F))
	{		
    ILI9341SetCurPos(usX, usX + 2*8 - 1, usY, usY + 2*16 - 1);
	
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

// auf Zeichenformat 16x24 Pixel gefaket
void ILI9341PutChar24_8x16(unsigned short usX, unsigned short usY, char c, 
                         unsigned long fColor, unsigned long bColor)
{
    unsigned int i;
    
//    xASSERT((usX <= LCD_HORIZONTAL_MAX) && (usY <= LCD_VERTICAL_MAX));
    
  if ((c >= 0x20) && (c <= 0x7F))
	{		
    ILI9341SetCurPos(usX, usX + 12 - 1, usY, usY + 2*12 - 1);
	
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

//*****************************************************************************
//
//! \brief Display a string on the LCD.
//!
//! \param pcString is a non-terminated string that to display.
//!
//! Display a string on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void 
ILI9341DisplayString(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0xC0) 
        {
            ILI9341PutChar8x16(usX + ucl * 8, usY, *pcString, fColor, bColor);
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
#endif

//*****************************************************************************
#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void 
ILI9341DisplayString15(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0xC0) 
        {
            ILI9341PutChar8x15(usX + ucl * 8, usY, *pcString, fColor, bColor);
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
#endif


#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void 
ILI9341DisplayStringVertical(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0x80) 
        {
            ILI9341PutChar8x16(usX , usY + ucl * 16, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
//            ILI9341PutGB1616(usX, usY + ucl * 16, (unsigned char*)pcString, fColor, bColor);
//            pcString += 2; ucl += 2;
					            pcString++; ucl++;

        }
    }
}
#endif

#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void ILI9341DisplayString32(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0xC0) 
        {
            ILI9341PutChar32_8x16(usX + ucl * 2*8, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
//            ILI9341PutGB1616(usX + ucl * 2*8, usY, (unsigned char*)pcString, fColor, bColor);
//            pcString += 2; ucl += 2;
            pcString++; ucl++;
					
        }
				
    }
}
#endif

#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void ILI9341DisplayString32_24(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0xC0) 
        {
            ILI9341PutChar32_8x16(usX + ucl * 2*8, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
//            ILI9341PutGB1616(usX + ucl * 2*8, usY, (unsigned char*)pcString, fColor, bColor);
//            pcString += 2; ucl += 2;
            pcString++; ucl++;
					
        }
    }
}
#endif


#if ((defined ILI9341_CHARACTER_FONT_8X16)||(defined ILI9341_CHINESE_FONT_16X16))
void ILI9341DisplayString24(unsigned short usX, unsigned short usY, const char* pcString,
                     unsigned long fColor, unsigned long bColor)
{
    unsigned char ucl = 0;
    
//    xASSERT(pcString != 0);
    
    while(*pcString) 
    {
        if( *pcString < 0xC0) 
        {
            ILI9341PutChar24_8x16(usX + ucl * 12, usY, *pcString, fColor, bColor);
            pcString++; ucl++;
        }
        else
        {
//            ILI9341PutGB1616(usX + ucl * 2*8, usY, (unsigned char*)pcString, fColor, bColor);
//            pcString += 2; ucl += 2;
					            pcString++; ucl++;

        }
    }
}
#endif
//*****************************************************************************
//
//! \brief Display a decimal number on the LCD.
//!
//! \param n is the number that to display.
//!
//! Display a decimal number on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayNum(unsigned short usX, unsigned short usY, unsigned long ulNum,
                   unsigned long fColor, unsigned long bColor)
{
    unsigned char ucFigures = 0;
    unsigned long ulTemp = ulNum;  
    
    if(ulNum)
    {
        while (ulTemp)
        {
            ulTemp /= 10;
            ucFigures++;
        }
    
        while (ulNum)
        {
            ILI9341PutChar8x16(usX + (ucFigures--) * 8 - 8, usY, ulNum%10 + '0', fColor, bColor);
            ulNum /= 10;
        }
    }
    else
    {
        ILI9341PutChar8x16(usX, usY, '0', fColor, bColor);
    }
}

//*****************************************************************************
//
//! \brief Display a bmp picture on the LCD.
//!
//! \param n is the number that to display.
//!
//! Display a decimal number on the cursor location.
//!
//! \return None.
//
//*****************************************************************************
void ILI9341DisplayBmp(unsigned short usX, unsigned short usY, unsigned short usSizeX, 
                   unsigned short usSizeY, unsigned char const *Bmp)
{
    unsigned short i,j;
    unsigned long ulBmpData;
    
//    xASSERT((usX < LCD_HORIZONTAL_MAX) && (usY < LCD_VERTICAL_MAX));
    
    ILI9341SetCurPos(usX, usX + usSizeX, usY, usY + usSizeY); 
	
    for( i = usY; i <= usY + usSizeY; i++ ) 
    {
        for( j = usX ; j <= usX + usSizeX; j++)
        {
            ulBmpData = *Bmp++;
            ulBmpData |= (*Bmp++) << 8;
            WriteWord(ulBmpData);  
        }
    }
}

