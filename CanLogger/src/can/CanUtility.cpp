#include <Arduino.h>
#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"

CAN_HandleTypeDef CanUtility_hcan;

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	/*
	From "stm32f3xx_hal_can.c":
	(#) Initialize the CAN low level resources by implementing the
          HAL_CAN_MspInit():
         (++) Enable the CAN interface clock using __HAL_RCC_CANx_CLK_ENABLE()
         (++) Configure CAN pins
             (+++) Enable the clock for the CAN GPIOs
             (+++) Configure CAN pins as alternate function open-drain
         (++) In case of using interrupts (e.g. HAL_CAN_ActivateNotification())
             (+++) Configure the CAN interrupt priority using
                   HAL_NVIC_SetPriority()
             (+++) Enable the CAN IRQ handler using HAL_NVIC_EnableIRQ()
             (+++) In CAN IRQ handler, call HAL_CAN_IRQHandler()
	*/
	__HAL_RCC_CAN1_CLK_ENABLE();
	while(__HAL_RCC_CAN1_IS_CLK_DISABLED())
	{}
	__HAL_RCC_GPIOB_CLK_ENABLE();
	while(__HAL_RCC_GPIOB_IS_CLK_DISABLED())
	{}
	
	GPIO_InitTypeDef canPin;
	// TX-Pin:
	canPin.Pin = GPIO_PIN_9;
	canPin.Mode = GPIO_MODE_AF_OD;
	canPin.Pull = GPIO_NOPULL;
	canPin.Speed = GPIO_SPEED_FREQ_HIGH;
	canPin.Alternate = GPIO_AF9_CAN;
	HAL_GPIO_Init(GPIOB, &canPin);
	
	// RX-Pin:
	canPin.Pin = GPIO_PIN_8;
	canPins.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &canPin);	
	
	//setup Interrupts:
}

void FillCAN_Handle(CAN_HandleTypeDef& hcan)
{
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 4; // entspricht 500 kBit/s
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_5TQ;
	hcan.Init.TimeTriggeredMode = ENABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
}

void FillCAN_Filter(CAN_FilterTypeDef& canFilter)
{
	canFilter.FilterActivation = CAN_FILTER_ENABLE;
	canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterBank = 0;
	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterIdLow = 0x0;
	canFilter.FilterIdHigh = 0x0;
	canFilter.FilterMaskIdLow = 0x0;
	canFilter.FilterMaskIdHigh = 0x0; 
}

void CanUtility_Init(void)
{	
	//initialize CAN-handler:
	FillCAN_Handle(CanUtility_hcan);
	if(HAL_CAN_Init(&CanUtility_hcan) != HAL_OK)
	{
		Serial.print("Fehler während der CAN initialisierung. Status: ");
		Serial.print(HAL_CAN_GetState(&CanUtility_hcan));
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&CanUtility_hcan),HEX));
		while(1){}
	}
	else
	{
		Serial.println("CAN MCR und BTR erfolgreich initialisiert");
	}
		
	//initialize CAN-Filter 0:
	CAN_FilterTypeDef canFilter;
	FillCAN_Filter(canFilter);
	if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
	{
		Serial.println("Fehler beim CAN-Filtersetup");
		while(1){}
	}
	else
	{
		Serial.println("CAN-Filtersetup erfolgreich beendet");
	}

	// start CAN-Instance:
	if(HAL_CAN_Start(&CanUtility_hcan) != HAL_OK)
	{
		Serial.print("CAN-Setup konnte nicht erfolgreich beendet werden. CAN-Status: ");
		Serial.print(HAL_CAN_GetState(&CanUtility_hcan));
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&CanUtility_hcan),HEX));
		while(1){}
	}
	else
	{
		Serial.println("CAN-Peripherie einsatzbereit");	
	}
}


/*
Ueberprueft ob in einem CANRX-FIFO eine nicht ausgelesende Nachricht liegt.
Input: 	fifo         	- welcher FIFO soll ueberprueft werden 
				          moegliche Werte: 0,1
return:	0				- im FIFO liegt keine Nachricht
		1				- im FIFO liegt eine Nachricht
*/
bool CanUtility_CheckMailbox(bool const fifo)
{
    if(!fifo)
	{
		return CAN->RF0R & CAN_RF0R_FMP0;
	}
	else
	{
		return CAN->RF1R & CAN_RF1R_FMP1;
	}
}


/*
Interrupt Handler für CAN-RX0
Wird aufgerufen, wenn:
	- neue CAN message in FIFO0 verfügbar
*/ 
void CAN_RX0_IRQHandler()
{
	if(CanUtility_CheckMailbox(0))
	{
		Canmsg recMessage;
		recMessage.Recieve(0);
		
		if(Canmsg_bufferCanRecPointer < CAN_MSG_CAN_BUFFER_REC_SIZE)
		{
			Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer] = recMessage;
			Canmsg_bufferCanRecPointer++;
		}else{
			/* Nachricht wird verworfen */
		}
	}
		
}



