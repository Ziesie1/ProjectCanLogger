#include <Arduino.h>
#include "Canmsg.hpp" 

/*
    Canmsg::Canmsg()
    Zu Testzwecken, Standartwerte
*/
Canmsg::Canmsg()
    :stdIdentifier{0x100},extIdentifier{0},isExtIdentifier{false},
        rtr{false},time{0x1000},canLength{maxLength}
{
    for(byte i=0;i<this->maxLength;i++)
    {
        canBytes[i]=static_cast<char>(0x01+(0x22*i));
    }
}

Canmsg::operator String() const
{
    String s="Identifier: ";
    if(this->isExtIdentifier)
    {
        s+=String(this->stdIdentifier<<18|this->extIdentifier,HEX);
    }
    else
    {
        s+=String(this->stdIdentifier,HEX);
    }
    s+="h RTR: ";
    s+=String(this->rtr);
    s+=" Time: ";
    s+=String(this->time,HEX);
	s+="h Laenge: ";
    s+=String(this->canLength,HEX);
    s+="h Inhalt: ";
    for(byte i=0;i<this->canLength;i++)
    {
        if(this->canBytes[i]<0x10)
        {
            s+="0";
        }
        s+=String(this->canBytes[i],HEX);
        if(i<canLength-1)
        {
            s+=".";
        }   
    }
    s+=" h";
    return s;
}
 
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	/*
	From "stm32f3xx_hal_can"
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
  	{
		Serial.println("CAN1 clock noch nicht enabled");
		delay(5);
  	}
	Serial.println("CAN1 clock enabled");
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	while(__HAL_RCC_GPIOB_IS_CLK_DISABLED())
	{
		Serial.println("GPIOB clock noch nicht enabled");
		delay(5);
	}
	Serial.println("GPIOB clock enabled");

	GPIO_InitTypeDef canPins;
	// TX-Pin:
	canPins.Pin = GPIO_PIN_9;
	canPins.Mode = GPIO_MODE_AF_OD;
	canPins.Pull = GPIO_NOPULL;
	canPins.Speed = GPIO_SPEED_FREQ_HIGH;
	canPins.Alternate = GPIO_AF9_CAN;
	HAL_GPIO_Init(GPIOB, &canPins);
	
	// RX-Pin:
	canPins.Pin = GPIO_PIN_8;
	canPins.Mode = GPIO_MODE_INPUT;
	HAL_GPIO_Init(GPIOB, &canPins);

	Serial.println(CAN->MCR & CAN_MCR_INRQ);
	Serial.println(CAN->MSR & CAN_MSR_INAK);
	CAN->MCR |= CAN_MCR_INRQ;
	Serial.println(CAN->MCR & CAN_MCR_INRQ);
	Serial.println(CAN->MSR & CAN_MSR_INAK);
	
	
	//setup Interrupts:
}

CAN_HandleTypeDef hcan;
CAN_FilterTypeDef canFilter;

void CANutil::Init(void)
{	
	//initialize CAN-handler:
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 3;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_6TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = DISABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if(HAL_CAN_Init(&hcan) != HAL_OK)
	{
		Serial.print("Fehler während der CAN initialisierung. Status: ");
		Serial.print(HAL_CAN_GetState(&hcan));
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&hcan),HEX));
		while(1)
		{

		}
	}
	else
	{
		Serial.println("CAN erfolgreich initialisiert :)");
	}

	//initialize CAN-Filter 0:
	canFilter.FilterActivation = CAN_FILTER_ENABLE;
	canFilter.FilterScale = CAN_FILTERSCALE_32BIT;
	canFilter.FilterMode = CAN_FILTERMODE_IDMASK;
	canFilter.FilterBank = 0;
	canFilter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	canFilter.FilterIdLow = 0x0;
	canFilter.FilterIdHigh = 0x0;
	canFilter.FilterMaskIdLow = 0x0;
	canFilter.FilterMaskIdHigh = 0x0; 
	HAL_CAN_ConfigFilter(&hcan, &canFilter);
	
	// start CAN-Instance:
	if(HAL_CAN_Start(&hcan) != HAL_OK)
	{
		Serial.print("CAN-Setup konnte nicht erfolgreich beendet werden. CAN-Status: ");
		Serial.print(HAL_CAN_GetState(&hcan));
		Serial.print(" Fehlercode: 0x");
		Serial.println(String(HAL_CAN_GetError(&hcan),HEX));
		while(1)
		{

		}
	}
	else
	{
		Serial.println("CAN-Setup erfolgreich beendet");	
	}
}

/*
Ueberprueft ob in einem CANRX-FIFO eine nicht ausgelesende Nachricht liegt.
Input: 	fifo         	- welcher FIFO soll ueberprueft werden 
				          moegliche Werte: 0,1
return:	0				- im FIFO liegt keine Nachricht
		1				- im FIFO liegt eine Nachricht
*/
bool CANutil::CheckMailbox(bool const fifo)
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
Liest eine im FIFO liegende Nachricht ein
Input: 	fifo 		- aus welchem FIFO soll gelesen werden 
					  moegliche Werte: 0,1
return:	void
*/
void Canmsg::Recieve(bool const fifo)
{
    this->stdIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_STID) >> CAN_RI0R_STID_Pos); //stdIdentifier aus dem CAN_RIxR Register
	this->extIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_EXID) >> CAN_RI0R_EXID_Pos);
	this->isExtIdentifier = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_IDE) >> CAN_RI0R_IDE_Pos);
	this->rtr = ((CAN->sFIFOMailBox[fifo].RIR & CAN_RI0R_RTR) >> CAN_RI0R_RTR_Pos); //Remote transmission Reguest aus dem CAN_RIxR Register
	this->time = ((CAN->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_TIME) >> CAN_RDT0R_TIME_Pos); //timestamp aus dem CAN_RDTxR Register
	this->canLength = ((CAN->sFIFOMailBox[fifo].RDTR & CAN_RDT0R_DLC) >> CAN_RDT0R_DLC_Pos); //anzahl der bytes aus dem CAN_RDTxR Register
	this->canBytes[0] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA0) >> CAN_RDL0R_DATA0_Pos); //Daten bytes aus dem CAN_RDLxR Register
	this->canBytes[1] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA1) >> CAN_RDL0R_DATA1_Pos);
	this->canBytes[2] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA2) >> CAN_RDL0R_DATA2_Pos);
	this->canBytes[3] = ((CAN->sFIFOMailBox[fifo].RDLR & CAN_RDL0R_DATA3) >> CAN_RDL0R_DATA3_Pos);
	this->canBytes[4] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA4) >> CAN_RDH0R_DATA4_Pos); //Daten bytes aus dem CAN_RDHxR Register
	this->canBytes[5] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA5) >> CAN_RDH0R_DATA5_Pos);
	this->canBytes[6] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA6) >> CAN_RDH0R_DATA6_Pos);
	this->canBytes[7] = ((CAN->sFIFOMailBox[fifo].RDHR & CAN_RDH0R_DATA7) >> CAN_RDH0R_DATA7_Pos);
	if(!fifo)
	{
		CAN->RF0R |= CAN_RF0R_RFOM0; //Empfange Nachricht freigeben im CAN_RF0R Register
	}
	else
	{
		CAN->RF1R |= CAN_RF1R_RFOM1; //Empfange Nachricht freigeben im CAN_RF1R Register
	}
}

/*
Sendet eine CAN Nachricht
Input: 	void
return:	void
*/
void Canmsg::Send(void) const
{
	/*/if (  CAN->sTxMailBox[0].TIR & CAN_TI0R_TXRQ)
	{
		Serial.println("Can FIFO ist busy");
	}
	else
	{
		uint32_t ti0R = CAN_TI0R_TXRQ; //Transmit mailbox request setzen
		if(this->rtr)
		{
			ti0R |= CAN_TI0R_RTR;	//Remote transmission request setzen
		}else{
			//normale Nachricht
			CAN->sTxMailBox[0].TDHR =  this->canBytes[7] << CAN_TDH0R_DATA7_Pos |  this->canBytes[6] << CAN_TDH0R_DATA6_Pos 
														|  this->canBytes[5] << CAN_TDH0R_DATA5_Pos |  this->canBytes[4] << CAN_TDH0R_DATA4_Pos ; 
			CAN->sTxMailBox[0].TDLR =  this->canBytes[3] << CAN_TDL0R_DATA3_Pos |  this->canBytes[2] << CAN_TDL0R_DATA2_Pos 
														|  this->canBytes[1] << CAN_TDL0R_DATA1_Pos |  this->canBytes[0] << CAN_TDL0R_DATA0_Pos ;
			CAN->sTxMailBox[0].TDTR = this->canLength;
		}
		
		if(this->isExtIdentifier)
		{
			ti0R |= this->stdIdentifier << CAN_TI0R_STID_Pos | this->extIdentifier << CAN_TI0R_EXID_Pos | CAN_TI0R_IDE; //Extendedt identifier setzen
		}else{
			ti0R |= this->stdIdentifier << CAN_TI0R_STID_Pos | CAN_TI0R_TXRQ; //Standard identifier setzen
		}
		CAN->sTxMailBox[0].TIR = ti0R;
	}*/
	Serial.print("FreeLevel: ");
	Serial.println(HAL_CAN_GetTxMailboxesFreeLevel(&hcan));
}

/*
Interrupt Handler für CAN-RX0
Wird aufgerufen, wenn:
	- neue CAN message in FIFO0 verfügbar
*/ 
void CAN_RX0_IRQHandler()
{
	if(CANutil::CheckMailbox(0))
	{
		Canmsg recMessage;
		recMessage.Recieve(0);
		
		if(CANutil::bufferCanRecPointer < CAN_MSG_CAN_BUFFER_REC_SIZE)
		{
			CANutil::bufferCanRecMessages[CANutil::bufferCanRecPointer] = recMessage;
			CANutil::bufferCanRecPointer++;
		}else{
			/* Nachricht wird verworfen */
		}
	}
		
}

void CANutil::CanCheck()
{  
/* 
	while(bufferCanRecPointer>0)
	{
    	bufferCanRecPointer--;
    	Canmsg msg=bufferCanRecMessages[bufferCanRecPointer];
    	Serial.println(static_cast<String>(msg));
    	msg.Send();
	}*/
}

