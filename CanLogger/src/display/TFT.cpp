#include "display/TFT.hpp"
#include "display/X_Spi9341.hpp"
#include <Arduino.h>

void initTFT(void)
{
    TFT_gpioInit();
    ILI9341Init();
	ClearColor(BLAU50);
	ILI9341DrawHorizontalLine(0,240,25,SCHWARZ);
	ILI9341DrawVerticalLine(60,0,320,SCHWARZ);
	ILI9341DrawVerticalLine(120,0,320,SCHWARZ);
	ILI9341DrawVerticalLine(180,0,320,SCHWARZ);

    ILI9341DisplayString32(20,100,"TEST", ROT100, BLAU50);
}

void TFT_gpioInit(void)
{
    GPIO_InitTypeDef displayPins;
  
    displayPins.Mode = GPIO_MODE_OUTPUT_PP;
    displayPins.Pull = GPIO_NOPULL;
    displayPins.Speed = GPIO_SPEED_FREQ_HIGH;
  
    displayPins.Pin = TFT_DC_Pin;
    HAL_GPIO_Init(TFT_DC_GPIO_Port, &displayPins);
  
    displayPins.Pin = TFT_MOSI_Pin;
    HAL_GPIO_Init(TFT_MOSI_GPIO_Port, &displayPins);
  
    displayPins.Pin = TFT_SCK_Pin;
    HAL_GPIO_Init(TFT_SCK_GPIO_Port, &displayPins);
  
    displayPins.Pin = TFT_RESET_N_Pin;
    HAL_GPIO_Init(TFT_RESET_N_GPIO_Port,&displayPins);
    
    displayPins.Pin = TFT_CS_N_Pin;
    HAL_GPIO_Init(TFT_CS_N_GPIO_Port, &displayPins);
}


