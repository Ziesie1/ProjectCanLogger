#include "can/CanUtility.hpp"
#include "can/Canmsg.hpp"

CAN_HandleTypeDef CanUtility_hcan;
bool CanUtility_CanRecieveActive = false;

/* 
	function that initializes the GPIO-Pins for CAN peripherals
	Input: hcan	- pointer to CAN-handle
*/
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
	HAL_GPIO_Init(GPIOB, &canPin);	

	// setup Interrupts:
	/*HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	HAL_NVIC_SetPriority(CAN_RX0_IRQn,15,0U);
	HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);*/
}

/* 
	function that fills a CAN-handle with all the important information for the specific application
	Input: hcan	- pointer to CAN-handle
*/
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

/* 
	function that fills a CANFilter-handle with all the important information for the specific application
*/
void FillCAN_Filter(CAN_FilterTypeDef& canFilter)
{
	canFilter.FilterActivation = CAN_FILTER_DISABLE;
	canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterBank = 0;
	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterIdLow = 0x0;
	canFilter.FilterIdHigh = 0x0;
	canFilter.FilterMaskIdLow = 0x0;
	canFilter.FilterMaskIdHigh = 0x0; 
}

/* 
	function that initializes the CAN perihpherals, the recieve Buffer
	and activates the CAN peripherals
	return:	HAL_OK		- everything is working as it is supposed to be
						  note to recieve messages furthermore the "CanUtility_EnableRecieve()" 
						  function needs to be called 
			HAL_ERROR	- an error occured while initializing, 
						  check the serial output for further details
*/
HAL_StatusTypeDef CanUtility_Init(void)
{	
	//initialize CAN-handler:
	FillCAN_Handle(CanUtility_hcan);
	if(HAL_CAN_Init(&CanUtility_hcan) != HAL_OK)
	{
		Serial.print("Fehler während der CAN initialisierung. Status: ");
		Serial.print(HAL_CAN_GetState(&CanUtility_hcan)); 
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&CanUtility_hcan),HEX)); 
		return HAL_ERROR;
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
		return HAL_ERROR;
	}
	else
	{
		Serial.println("CAN-Filtersetup erfolgreich beendet");
	}

	/*if(HAL_CAN_ActivateNotification(&CanUtility_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		Serial.println("CAN-IRQ konnte nicht aktiviert werden");
		return HAL_ERROR;
	}
	else
	{
		Serial.println("CAN-IRQ aktiviert");
	}*/

	// start CAN-Instance:
	if(HAL_CAN_Start(&CanUtility_hcan) != HAL_OK)
	{
		Serial.print("CAN-Setup konnte nicht erfolgreich beendet werden. CAN-Status: "); 
		Serial.print(HAL_CAN_GetState(&CanUtility_hcan));
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&CanUtility_hcan),HEX)); 
		return HAL_ERROR;
	}
	else
	{
		CanUtility_CanRecieveActive = false;
		Serial.println("CAN-Peripherie einsatzbereit");	
	}
	Canmsg_bufferCanRecPointer = 0;
	return HAL_OK;
}

/* 
	interrupt handler that is called when messages are pending in FIFO 0
	Input: hcan	- pointer to CAN-handle
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, 0) != 0)
	{		
		Canmsg recMessage;
		recMessage.Recieve(0);
		if(Canmsg_bufferCanRecPointer < Canmsg_CAN_BUFFER_REC_SIZE)
		{
			Canmsg_bufferCanRecMessages[Canmsg_bufferCanRecPointer] = recMessage;
			Canmsg_bufferCanRecPointer++;
		}
		else
		{
			/* Nachricht wird verworfen */
			Serial.println("Nachricht verworfen");
		}
	}
}

/* 
	interrupt handler that is called every ms 
	NOTE: THIS IS JUST A WORKAROUND AND WILL PROBABLY BE CHANGED IN THE FUTURE
*/
void HAL_SYSTICK_Callback(void)
{
	if(CanUtility_CanRecieveActive)
	{
		HAL_CAN_RxFifo0MsgPendingCallback(&CanUtility_hcan);
	}
}

/*
Interrupt Handler für CAN-RX0
Wird aufgerufen, wenn:
	- neue CAN message in FIFO0 verfügbar
*/ 
/*void CAN_RX0_IRQHandler()
{
	HAL_CAN_IRQHandler(&CanUtility_hcan);
}*/

/* 
	function that configures the CAN-message-filter to process all incomming messages
	return:	HAL_OK		- everything is working as it is supposed to be,
						  the peripherals are activated
			HAL_ERROR	- an error occured while activating the peripherals, 
						  check if the Peripherals are allready initialized
						  or refer to CanUtility_hcan->ErrorCode
*/
HAL_StatusTypeDef CanUtility_EnableRecieve(void)
{
	CAN_FilterTypeDef canFilter;
	FillCAN_Filter(canFilter);
	canFilter.FilterActivation = CAN_FILTER_ENABLE;
	if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		CanUtility_CanRecieveActive = true;
		return HAL_OK;
	}
}

/* 
	function that configures the CAN-message-filter to discard all incomming messages
	return:	HAL_OK		- everything is working as it is supposed to be,
						  the peripherals are deactivated
			HAL_ERROR	- an error occured while activating the peripherals,
						  please refer to CanUtility_hcan->ErrorCode 
*/
HAL_StatusTypeDef CanUtility_DissableRecieve(void)
{
	CAN_FilterTypeDef canFilter;
	FillCAN_Filter(canFilter);
	if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
	{
		return HAL_ERROR;
	}
	else
	{
		CanUtility_CanRecieveActive = false;
		return HAL_OK;
	}
}
