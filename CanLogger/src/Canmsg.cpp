#define HAL_CAN_MODULE_ENABLED"
#include <Arduino.h>
#include "Canmsg.h"    

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

 
void CANutil::Init(void)
{
	//ToDo: 	Hier CAN Initialisierung einfügen
	// STM32F303RET6 PLLCLK is 72 MHz
	// Clock for CAN is APB1CLOCK with 36 MHz
	// To get multiple of 1MHz setup CAN State Machine with 18 Cycles
	// https://www.mikrocontroller.net/topic/200078
	// BS1 = 12 TQ, BS2 = 5 TQ + Sync = 1 TQ, Sum: 18TQ
	
	/*
	 alte berechnung:
	CAN_TS1 = BS1-1, CAN_TS2 = BS2-1;  
	#define CAN_TS1 11
	#define CAN_TS2 4
	#define CAN_SJW 1		
	*/

	//BRP = Prescalor - 1
	// 1   Mbit Prescalor 2, 
	#define CAN_1000_KBIT 1
	// 500 kBit Prescalor 4
	#define CAN_500_KBIT 3
	// 400 kBit Prescalor 5
	#define CAN_400_KBIT 4
	// 250 kBit Prescalor 8
	#define CAN_250_KBIT 7
	// 200 kBit Prescalor 10
	#define CAN_200_KBIT 9
	// 125 kBit Prescalor 16
	#define CAN_125_KBIT 15
	// 125 kBit Prescalor 20
  	#define CAN_100_KBIT 19

	// Select baud rate
 	 #define CAN_BRP CAN_500_KBIT

	//set up CAN-Mode
	CAN_HandleTypeDef hcan;

	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.TimeTriggeredMode = ENABLE;
	hcan.Init.TimeSeg2 = CAN_BS2_5TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_12TQ;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.Prescaler = CAN_BRP;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoRetransmission = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;

	HAL_CAN_MspInit(&hcan);
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	//GPIO pins

	GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Alternate = GPIO_AF9_CAN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//IRQ
	/*
	IRQn_Type irqn = CAN_RX1_IRQn;
	HAL_NVIC_SetPriority(irqn,1,5);
	HAL_NVIC_EnableIRQ(irqn);
	HAL_CAN_IRQHandler(&hcan);
	*/

	//initialize CAN-periphery
	HAL_CAN_Init(&hcan);

	//Messagefilter
	CAN_FilterTypeDef canFilter;
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
	
	CANutil::bufferCanRecPointer=0;

	HAL_CAN_Start(&hcan);

	//CAN->MCR |= CAN_MCR_INRQ; /* Enter CAN Init Mode */
	//while ((CAN->MSR & CAN_MSR_INAK) != static_cast<uint32_t>(CAN_MSR_INAK)) /* Wait 4 Init */
	{
;		/* add time out here for a robust application */
	}
	//CAN->MCR &= ~CAN_MCR_SLEEP; /* Release from Sleep Mode */
	//CAN->BTR = (CAN_SJW << 24 | CAN_TS2 << 20 | CAN_TS1 << 16 | CAN_BRP); /* Set Timing */

	//CAN->MCR |= CAN_MCR_TTCM; //enable Time triggered Communication
	
	// Initializing Filter to store any incomming Messages in FIFO 0 
	//CAN->FMR |= CAN_FMR_FINIT; /* Enter Filter initialization Mode */										 
	//CAN->FA1R &= ~CAN_FA1R_FACT0; // deaktivate Filter 0
	
	//CAN->FS1R |= CAN_FS1R_FSC0; //set 32-bit scale configuration for filter 0
	//CAN->FM1R &= ~CAN_FM1R_FBM0; //set 32-bit mask mode for filter 0
	
	//CAN->sFilterRegister[0].FR1 = 0; //init 32-bit mask for filter 0
	//CAN->sFilterRegister[0].FR2 = 0; //let everything through for filter 0
	
	//CAN->FFA1R &= ~CAN_FFA1R_FFA0; //assign filter 0 to FIFO 0
	//CAN->FA1R |= CAN_FA1R_FACT0; //activate Filter 0
	
	//CAN->FMR &= ~CAN_FMR_FINIT; /* Leave Filter initialization Mode */
	
	//Interrupt Einstellungen
	//CAN->IER |= CAN_IER_FMPIE0; //enable interrupt for FIFO 0 if message available, Register: CAN_IER
	//NVIC_EnableIRQ(CAN_RX0_IRQn); //Interrupt hinzufügen
	
	//CAN->MCR &= ~CAN_MCR_INRQ; /* Leave CAN Init Mode */
	//while ((CAN->MSR & CAN_MSR_INAK) == CAN_MSR_INAK) /* Wait 4 Init Finished */
	{
	;	/* add time out here for a robust application */
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
	if (  CAN->sTxMailBox[0].TIR & CAN_TI0R_TXRQ)
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
	}
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

