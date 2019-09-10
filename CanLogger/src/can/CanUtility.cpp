#include "can/CanUtility.hpp"
#include "utilities/SerialCommunication.hpp"

CAN_HandleTypeDef CanUtility_hcan;
bool CanUtility_CanRecieveActive = false;

Canmsg** CanUtility_bufferCanRecMessages;
int CanUtility_bufferCanRecPointer;

int CanUtility_discardedMessages = 0;
int CanUtility_discardedMessagesLastState = 0;
bool CanUtility_toManyMsgs = false;

bool CanUtility_initialized = false;

CAN_TransmissionMode CanUtility_currentMode = CAN_TransmissionMode_Normal;
CAN_SpeedTypedef CanUtility_transmissionSpeed = CAN_500_KBIT;

/* 
	interrupt handler that is called when messages are pending in FIFO 0
	Input: hcan	- pointer to CAN-handle
*/
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, 0) != 0)
	{		
		if(CanUtility_bufferCanRecPointer < CanUtility_CAN_BUFFER_REC_SIZE)
		{
			if(!CanUtility_bufferCanRecMessages[CanUtility_bufferCanRecPointer])
			{
				CanUtility_bufferCanRecMessages[CanUtility_bufferCanRecPointer] = new Canmsg{0x7ff, 0x3ffff, true, false, 0xffff, 0};
			}
			CanUtility_RecieveMessage(0, CanUtility_bufferCanRecMessages[CanUtility_bufferCanRecPointer]);
			CanUtility_bufferCanRecPointer++;
		}
		else
		{
			/* Nachricht wird verworfen */
			CanUtility_hcan.Instance->RF0R |= CAN_RF0R_RFOM0; //Empfange Nachricht freigeben im CAN_RF0R Register
    		CanUtility_discardedMessages++;
		}
	}
}

/* 
	interrupt handler that is called when FIFO 0 is full
	Input: hcan	- pointer to CAN-handle
*/
/*void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan)
{
		
}*/

/* 
	interrupt handler that is called when a overflow in FIFO 0 is occured
	Input: hcan	- pointer to CAN-handle
*/
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->ErrorCode == HAL_CAN_ERROR_RX_FOV0)
	{
		HAL_NVIC_DisableIRQ(CAN_RX0_IRQn);
		while(CAN->RF0R & CAN_RF0R_FMP0)
		{
			SET_BIT(hcan->Instance->RF0R, CAN_RF0R_RFOM0);
			CanUtility_discardedMessages++;
		}
		CanUtility_toManyMsgs = true;
		CanUtility_discardedMessages++;
		hcan->ErrorCode = HAL_CAN_ERROR_NONE;
		HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
	}
}

#ifdef __cplusplus
extern "C" {
#endif

/*
Interrupt Handler für CAN-RX0
Wird aufgerufen, wenn:
	- neue CAN message in FIFO0 verfügbar
*/
void CAN_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&CanUtility_hcan);
}

#ifdef __cplusplus
}
#endif

/*
	function that enters the initialization mode of the CAN hardware
	note:   to leave the initilization mode call the function "CanUtility_leaveInitMode()"
			it is highly recommended to leave the initialization mode in the same function 
			it is entered to ensure the usability of the CAN hardware
*/
void CanUtility_enterInitMode(void)
{
	CanUtility_hcan.Instance->MCR |= CAN_MCR_INRQ;
    while(!(CanUtility_hcan.Instance->MSR & CAN_MSR_INAK))
    {}
}

/*
	function that leaves the initialization mode of the CAN hardware
	intended to be called after "CanUtility_enterInitMode()" and applying some settings
*/
void CanUtility_leaveInitMode(void)
{
	CanUtility_hcan.Instance->MCR &= ~CAN_MCR_INRQ;
    while(CanUtility_hcan.Instance->MSR & CAN_MSR_INAK)
    {}
}

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
	
	HAL_NVIC_SetPriority(CAN_RX0_IRQn,0,2);
	HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
	//resetInterrupts:

	// TX-Pin:
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);
	
	// RX-Pin:
	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8);	

	__HAL_RCC_CAN1_CLK_DISABLE();
	while(__HAL_RCC_CAN1_IS_CLK_ENABLED())
	{}
}

/* 
	function that fills a CAN-handle with all the important information for the specific application
	Input: 	hcan	- pointer to CAN-handle
			speed	- transmission speed of the CAN-bus
*/
void FillCAN_Handle(CAN_HandleTypeDef& hcan, CAN_SpeedTypedef speed)
{
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = speed;
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
	Input:	speed		- transmission speed of the CAN-bus
	return:	HAL_OK		- everything is working as it is supposed to be
						  note to recieve messages furthermore the "CanUtility_EnableRecieve()" 
						  function needs to be called 
			HAL_ERROR	- an error occured while initializing, 
						  check the serial output for further details
*/
HAL_StatusTypeDef CanUtility_Init(CAN_SpeedTypedef speed)
{	
	if(!CanUtility_initialized)
	{
		//initialize CAN-handler:
		FillCAN_Handle(CanUtility_hcan, speed);
		if(HAL_CAN_Init(&CanUtility_hcan) != HAL_OK)
		{
			String s = "Fehler während der CAN initialisierung. Status: ";
			s += String(HAL_CAN_GetState(&CanUtility_hcan),HEX); 
			s += " Fehlercode: 0x";
			s += String(HAL_CAN_GetError(&CanUtility_hcan),HEX); 
			utilities::scom.printError(s);
			return HAL_ERROR;
			CanUtility_currentMode = CAN_TransmissionMode_Normal;
			CanUtility_transmissionSpeed = speed;
		}
		else
		{
			utilities::scom.printDebug("CAN MCR und BTR erfolgreich initialisiert");
		}

		//initialize CAN-Filter 0:
		CAN_FilterTypeDef canFilter;
		FillCAN_Filter(canFilter);
		if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
		{
			utilities::scom.printError("Fehler beim CAN-Filtersetup");
			return HAL_ERROR;
		}
		else
		{
			utilities::scom.printDebug("CAN-Filtersetup erfolgreich beendet");
		}

		if((HAL_CAN_ActivateNotification(&CanUtility_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) ||
			(HAL_CAN_ActivateNotification(&CanUtility_hcan, CAN_IT_RX_FIFO0_OVERRUN) != HAL_OK))
		{
			utilities::scom.printDebug("CAN-IRQ konnte nicht aktiviert werden");
			return HAL_ERROR;
		}
		else
		{
			utilities::scom.printDebug("CAN-IRQ aktiviert");
		}

		// start CAN-Instance:
		if(HAL_CAN_Start(&CanUtility_hcan) != HAL_OK)
		{
			String s = "CAN-Setup konnte nicht erfolgreich beendet werden. CAN-Status: "; 
			s += String(HAL_CAN_GetState(&CanUtility_hcan));
			s += " Fehlercode: 0x";
			s += String(HAL_CAN_GetError(&CanUtility_hcan),HEX);
			utilities::scom.printError(s); 
			return HAL_ERROR;
		}
		else
		{
			CanUtility_CanRecieveActive = false;
			utilities::scom.printDebug("CAN-Peripherie einsatzbereit");	
		}
	
		CanUtility_bufferCanRecMessages = new Canmsg*[CanUtility_CAN_BUFFER_REC_SIZE];
		for(int i=0; i<CanUtility_CAN_BUFFER_REC_SIZE; i++)
		{
			CanUtility_bufferCanRecMessages[i] = new Canmsg{0x7ff, 0x3ffff, true, false, 0xffff, 0};
		}
		CanUtility_bufferCanRecPointer = 0;
		CanUtility_discardedMessages = 0;
		CanUtility_discardedMessagesLastState = 0;
		CanUtility_toManyMsgs = false;

		CanUtility_initialized = true;
		return HAL_OK;
	}
	return HAL_ERROR;
}

/* 
	function that deinitializes the CAN perihpherals and the recieve Buffer
	return:	HAL_OK		- everything is working as it is supposed to be
			HAL_ERROR	- an error occured while deinitializing, 
						  check the serial output for further details
*/
HAL_StatusTypeDef CanUtility_DeInit(void)
{
	if(HAL_CAN_DeInit(&CanUtility_hcan)!= HAL_OK)
	{
		String s = "Fehler während der CAN deinitialisierung. Status: ";
		s += String(HAL_CAN_GetState(&CanUtility_hcan),HEX); 
		s += " Fehlercode: 0x";
		s += String(HAL_CAN_GetError(&CanUtility_hcan),HEX);
		utilities::scom.printError(s); 
		return HAL_ERROR;
	}
	for(int i=0; i<CanUtility_CAN_BUFFER_REC_SIZE; i++)
	{
		delete CanUtility_bufferCanRecMessages[i];
		CanUtility_bufferCanRecMessages[i] = nullptr;
	}
	delete[] CanUtility_bufferCanRecMessages;
	CanUtility_bufferCanRecMessages = nullptr;
	CanUtility_bufferCanRecPointer = 0;
	CanUtility_discardedMessages = 0;
	CanUtility_discardedMessagesLastState = 0;
	CanUtility_toManyMsgs = false;
	
	CanUtility_initialized = false;
	utilities::scom.printDebug("CAN-Baustein deinitialisiert");
	return HAL_OK;
}

/* 
	function that configures the CAN-message filter to process all incomming messages
	return:	HAL_OK		- everything is working as it is supposed to be,
						  the peripherals are activated
			HAL_ERROR	- an error occured while activating the message filter, 
						  check if the Peripherals are allready initialized
						  or refer to CanUtility_hcan->ErrorCode
*/
HAL_StatusTypeDef CanUtility_EnableRecieve(void)
{
	if(CanUtility_initialized)
	{
		CAN_FilterTypeDef canFilter;
		FillCAN_Filter(canFilter);
		canFilter.FilterActivation = CAN_FILTER_ENABLE;
		if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
		{
			utilities::scom.printError("Fehler beim aktivieren des CAN Filters");
			return HAL_ERROR;
		}
		else
		{
			CanUtility_CanRecieveActive = true;
			utilities::scom.printDebug("CAN-Nachrichten werden bearbeitet");
			return HAL_OK;
		}
	}
	utilities::scom.printDebug("CAN-Baustein noch nicht initialisiert");
	return HAL_ERROR;
}

/* 
	function that configures the CAN-message filter to discard all incomming messages
	return:	HAL_OK		- everything is working as it is supposed to be,
						  the peripherals are deactivated
			HAL_ERROR	- an error occured while deactivating the message filter,
						  please refer to CanUtility_hcan->ErrorCode 
*/
HAL_StatusTypeDef CanUtility_DissableRecieve(void)
{
	CAN_FilterTypeDef canFilter;
	FillCAN_Filter(canFilter);
	if(HAL_CAN_ConfigFilter(&CanUtility_hcan, &canFilter) != HAL_OK)
	{
		utilities::scom.printError("Fehler beim deaktivieren des CAN Filters");
		return HAL_ERROR;
	}
	else
	{
		CanUtility_CanRecieveActive = false;
		utilities::scom.printDebug("CAN-Nachrichten werden ignoriert");
		return HAL_OK;
	}
}

/*
	returns state of CAN-block
	return:	true	- CAN-block is currently recieving
			false	- CAN-block is currently not recieving
*/
bool CanUtility_isRecieveActive(void)
{
	return CanUtility_CanRecieveActive;
}


/*
    copies the values of a CAN-message in the recieve FIFO into an instanse of Canmsg
    releases CAN-message from FIFO after the copy process
    Input:  fifo  	- specifies the FIFO that the message is pending in 
			msg		- pointer to a Canmsg object, the message should be stored in
*/
HAL_StatusTypeDef CanUtility_RecieveMessage(bool const fifo, Canmsg * msg)
{
	if(CanUtility_initialized)
	{
		if(HAL_CAN_GetRxFifoFillLevel(&CanUtility_hcan, fifo) != 0 && msg)
    	{
	    	msg->stdIdentifier = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RIR & CAN_RI0R_STID) >> CAN_RI0R_STID_Pos);
			msg->extIdentifier = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RIR & CAN_RI0R_EXID) >> CAN_RI0R_EXID_Pos);
			msg->isExtIdentifier = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RIR & CAN_RI0R_IDE) >> CAN_RI0R_IDE_Pos);
			msg->rtr = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RIR & CAN_RI0R_RTR) >> CAN_RI0R_RTR_Pos); //Remote transmission Reguest aus dem CAN_RIxR Register
			msg->time = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_TIME) >> CAN_RDT0R_TIME_Pos); //timestamp aus dem CAN_RDTxR Register
			msg->canLength = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_DLC) >> CAN_RDT0R_DLC_Pos); //anzahl der bytes aus dem CAN_RDTxR Register	
			if(!(msg->rtr))
			{
				msg->data[0] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA0) >> CAN_RDL0R_DATA0_Pos); //Daten bytes aus dem CAN_RDLxR Register
				msg->data[1] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA1) >> CAN_RDL0R_DATA1_Pos);
				msg->data[2] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA2) >> CAN_RDL0R_DATA2_Pos);
				msg->data[3] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA3) >> CAN_RDL0R_DATA3_Pos);
				msg->data[4] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA4) >> CAN_RDH0R_DATA4_Pos); //Daten bytes aus dem CAN_RDHxR Register
				msg->data[5] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA5) >> CAN_RDH0R_DATA5_Pos);
				msg->data[6] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA6) >> CAN_RDH0R_DATA6_Pos);
				msg->data[7] = ((CanUtility_hcan.Instance->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA7) >> CAN_RDH0R_DATA7_Pos);
			}
			else
			{
				for(int i=0; i<msg->maxLength; i++)
				{
					msg->data[i] = 0;
				}
			}

			if(!fifo)
			{
				CanUtility_hcan.Instance->RF0R |= CAN_RF0R_RFOM0; //Empfange Nachricht freigeben im CAN_RF0R Register
			}
			else
			{
				CanUtility_hcan.Instance->RF1R |= CAN_RF1R_RFOM1; //Empfange Nachricht freigeben im CAN_RF1R Register
			}
			return HAL_OK;
    	}
		return HAL_ERROR;	
	}
	return HAL_ERROR;
}

/*
    sends the message via tx-mailbox 0
    Input:	pointer to the message that is about to be send
	return: HAL_OK		- everything is working as it is supposed to be
			HAL_ERROR	- an error occured while activating the peripherals, 
						  check if the Peripherals are allready initialized
					      or refer to CanUtility_hcan->ErrorCode
*/
HAL_StatusTypeDef CanUtility_SendMessage(Canmsg *const msg)
{
	if(msg && CanUtility_initialized && CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent && CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent_Loopback)
	{
		if(HAL_CAN_GetTxMailboxesFreeLevel(&CanUtility_hcan) != 0)
		{
			CAN_TxHeaderTypeDef header;
			if(!msg->isExtIdentifier)
    		{
    		  	header.IDE = CAN_ID_STD;
    		}
    		else
    		{
      			header.IDE = CAN_ID_EXT;
    		}
    		header.StdId = msg->stdIdentifier;
    		header.ExtId = (msg->stdIdentifier<<18)|msg->extIdentifier;
    		if(!msg->rtr)
    		{
      			header.RTR = CAN_RTR_DATA;
    		}
    		else
    		{
      			header.RTR = CAN_RTR_REMOTE;
    		}
			header.DLC = msg->canLength;
			header.TransmitGlobalTime = DISABLE;
			uint8_t data[8];
			for(uint8_t i=0; i<header.DLC; i++)
			{
				data[i] = msg->data[i];
			}
			uint32_t mailbox = 0;
			return HAL_CAN_AddTxMessage(&CanUtility_hcan, &header, data, &mailbox);
		}
  		else
  		{
    		utilities::scom.printError("CAN-Nachricht konnte nicht gesendet werden(Alle TX Mailboxen belegt).");
   			return HAL_ERROR;
  		}
	}
	return HAL_ERROR;
}

/*
	function that gives the message from the recieve buffer, that was put in first(timewise)
	return:	valid pointer	- pointer to the Message 
							  note message will be deleted from buffer
			nullptr			- FiFo was empty
*/
Canmsg* CanUtility_readFirstMessageFromBuffer(void)
{
	HAL_NVIC_DisableIRQ(CAN_RX0_IRQn);
	Canmsg* temp = nullptr;
	if(CanUtility_bufferCanRecPointer > 0)
	{
		CanUtility_bufferCanRecPointer--;
		temp = CanUtility_bufferCanRecMessages[0];
		for(int i=0; i<CanUtility_bufferCanRecPointer; i++)
		{
			CanUtility_bufferCanRecMessages[i] = CanUtility_bufferCanRecMessages[i+1];
		}
		CanUtility_bufferCanRecMessages[CanUtility_bufferCanRecPointer] = new Canmsg{0x7ff, 0x3ffff, true, false, 0xffff, 0};
	}
	if(CanUtility_initialized)
	{
		HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
	}	
	return temp;
}

/*
	returns the ammount of messages stored in the CAN-recieve buffer
	return:	int	- number of messages in the recieve buffer
*/
int CanUtility_getbufferCanRecMessagesFillLevel(void)
{
	HAL_NVIC_DisableIRQ(CAN_RX0_IRQn);
	int temp = CanUtility_bufferCanRecPointer; 
	if(CanUtility_initialized)
	{
		HAL_NVIC_EnableIRQ(CAN_RX0_IRQn);
	}
	return temp;
}

/*
	tells if any messages are pending in the recieve buffer
	return:	true	- messages are pending
			false	- no messages are pending
*/
bool CanUtility_isMessagePending(void)
{
	return (CanUtility_getbufferCanRecMessagesFillLevel()!=0);	
}

/*
	checks if an overrun occured since the last time checked
	return: true	- overrun has occured
			false	- overrun has not occured 
*/
bool CanUtility_hasFiFoOverflowOccured(void)
{
	if(CanUtility_toManyMsgs)
	{
		CanUtility_toManyMsgs = false;
		return true;
	}
	else
	{
		return false;
	}
}

/*
	checks if any messages were discarded since last checked
	return:	true	- messages were discarded
			false	- no messages were discarded 
*/
bool CanUtility_whereNewMessagesDiscarded(void)
{	
	bool temp = CanUtility_discardedMessages != CanUtility_discardedMessagesLastState;
	CanUtility_discardedMessagesLastState = CanUtility_discardedMessages;
	if(CanUtility_discardedMessages == 0)
	{
		return false;
	}
	return temp;
}

/*
	returns the ammount of messages that were discarded since the last init of CAN
	return:	int	- number of messages that were discarded since the last init of CAN
*/
int CanUtility_howManyMessagesWhereDiscarded(void)
{
	return CanUtility_discardedMessages;
}

/*
	resets the counter of the discarded messages
*/
void CanUtility_resetDiscardcounter(void)
{
	CanUtility_discardedMessages = 0;
}

/*
	returns the current transmission mode 
	return:	CAN_TransmissionMode	- value that represents the current transmission mode of the CAN hardware
*/
CAN_TransmissionMode CanUtility_getTransmissionMode(void)
{
	return CanUtility_currentMode;
}

/*
	returns the current transmission speed 
	return:	CAN_SpeedTypedef	- value that represents the current transmission baud rate of the CAN hardware
*/
CAN_SpeedTypedef CanUtility_getTransmissionSpeed(void)
{
	return CanUtility_transmissionSpeed;
}

/*
    sets the transmission mode of the CAN hardware
    Input:	mode that the CAN hardware should be set in
	return: HAL_OK		- everything is working as it is supposed to be
			HAL_ERROR	- an error occured while setting up the peripherals, 
						  check if the Peripherals are allready initialized
*/
HAL_StatusTypeDef CanUtility_setTransmissionMode(CAN_TransmissionMode const mode)
{
	if(CanUtility_initialized && (mode <= CAN_TransmissionMode_Silent_Loopback))
	{
		CanUtility_enterInitMode();
		/*Silent*/
		if((mode == CAN_TransmissionMode_Silent_Loopback || mode == CAN_TransmissionMode_Silent) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent_Loopback) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent))
		{
			CanUtility_hcan.Instance->BTR |= CAN_BTR_SILM;
		}
		else if((mode == CAN_TransmissionMode_Loopback || mode == CAN_TransmissionMode_Normal) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Normal) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Loopback))
		{
			CanUtility_hcan.Instance->BTR &= ~CAN_BTR_SILM;
		}
		/*Loopback*/
		if((mode == CAN_TransmissionMode_Silent_Loopback || mode == CAN_TransmissionMode_Loopback) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent_Loopback) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Loopback))
		{
			CanUtility_hcan.Instance->BTR |= CAN_BTR_LBKM;
		}
		else if((mode == CAN_TransmissionMode_Silent || mode == CAN_TransmissionMode_Normal) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Silent) && (CanUtility_getTransmissionMode() != CAN_TransmissionMode_Normal))
		{
			CanUtility_hcan.Instance->BTR &= ~CAN_BTR_LBKM;
		}
		CanUtility_leaveInitMode();
		CanUtility_currentMode = mode;
		return HAL_OK;
	}
	return HAL_ERROR;
}

/*
    sets the transmission baud rate of the CAN hardware
    Input:	baud rate that the CAN hardware should communicate with
	return: HAL_OK		- everything is working as it is supposed to be
			HAL_ERROR	- an error occured while setting up the peripherals, 
						  check if the Peripherals are allready initialized
*/
HAL_StatusTypeDef CanUtility_setTransmissionSpeed(CAN_SpeedTypedef speed)
{
	if(CanUtility_initialized && (speed <= (CAN_BTR_BRP_Msk+1)))
	{
		CanUtility_enterInitMode();
		CanUtility_hcan.Instance->BTR &= ~CAN_BTR_BRP;
		CanUtility_hcan.Instance->BTR |= ((speed - 1) & CAN_BTR_BRP);
		CanUtility_leaveInitMode();
		CanUtility_transmissionSpeed = speed;
		return HAL_OK;
	}
	return HAL_ERROR;
}
