#ifndef TFT_HPP
#define TFT_HPP

#include "Arduino.h"


#define TFT_DC_Pin GPIO_PIN_8
#define TFT_DC_GPIO_Port GPIOA
#define INIT_TFT_DC	 TFT_DC_GPIO_Port->BRR = TFT_DC_Pin
#define SET_TFT_DC	 TFT_DC_GPIO_Port->BSRR = TFT_DC_Pin
#define RES_TFT_DC	 TFT_DC_GPIO_Port->BRR = TFT_DC_Pin

#define TFT_MOSI_Pin GPIO_PIN_10
#define TFT_MOSI_GPIO_Port GPIOA
#define INIT_TFT_MOSI	 TFT_MOSI_GPIO_Port->BRR = TFT_MOSI_Pin
#define SET_TFT_MOSI	 TFT_MOSI_GPIO_Port->BSRR = TFT_MOSI_Pin
#define RES_TFT_MOSI	 TFT_MOSI_GPIO_Port->BRR = TFT_MOSI_Pin

#define TFT_SCK_Pin GPIO_PIN_5
#define TFT_SCK_GPIO_Port GPIOB
#define INIT_TFT_SCK	 TFT_SCK_GPIO_Port->BRR = TFT_SCK_Pin
#define SET_TFT_SCK	 TFT_SCK_GPIO_Port->BSRR = TFT_SCK_Pin 
#define RES_TFT_SCK	 TFT_SCK_GPIO_Port->BRR = TFT_SCK_Pin

#define TFT_RESET_N_Pin GPIO_PIN_8
#define TFT_RESET_N_GPIO_Port GPIOC
#define INIT_TFT_RESET_N	 TFT_RESET_N_GPIO_Port->BRR = TFT_RESET_N_Pin
#define SET_TFT_RESET_N	 TFT_RESET_N_GPIO_Port->BSRR = TFT_RESET_N_Pin
#define RES_TFT_RESET_N	 TFT_RESET_N_GPIO_Port->BRR = TFT_RESET_N_Pin

#define TFT_CS_N_Pin GPIO_PIN_9
#define TFT_CS_N_GPIO_Port GPIOC
#define INIT_TFT_CS_N	 TFT_CS_N_GPIO_Port->BSRR = TFT_CS_N_Pin
#define SET_TFT_CS_N	 TFT_CS_N_GPIO_Port->BSRR = TFT_CS_N_Pin
#define RES_TFT_CS_N	 TFT_CS_N_GPIO_Port->BRR = TFT_CS_N_Pin

void TFT_gpioInit(void);
void TFT_init(void);

#endif