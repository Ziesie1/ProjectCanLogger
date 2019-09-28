#ifndef INTERRUPTCHECK_HPP
#define INTERRUPTCHECK_HPP

/* 
this file is to check in which interrupt caused problems with the µC
please set the interrupts that you don't want to check as a comment.
If a interrupt is called the Program will end up in one of the endless loops.
use a debugger to check which interrupt is called, or help yourself with the serial monitor, 
as the programm will not leave the interrupt either way ;)
*/


#ifdef __cplusplus
extern "C"{
#endif

#ifdef CUSTOMINTERRUPTCHECK

//void Default_Handler(){while(1){}}

void NMI_Handler(){while(1){}}

void HardFault_Handler(){while(1){}}
	
void MemManage_Handler(){while(1){}}
	
void BusFault_Handler(){while(1){}}
	
void UsageFault_Handler(){while(1){}}

void SVC_Handler(){while(1){}}

void DebugMon_Handler(){while(1){}}
	
void PendSV_Handler(){while(1){}}
	
//void SysTick_Handler(){while(1){}}
	
void WWDG_IRQHandler(){while(1){}}
	
void PVD_IRQHandler(){while(1){}}
	
void TAMP_STAMP_IRQHandler(){while(1){}}
	
void RTC_WKUP_IRQHandler(){while(1){}}
	
void FLASH_IRQHandler(){while(1){}}
	
void RCC_IRQHandler(){while(1){}}
	
//void EXTI0_IRQHandler(){while(1){}}
	
//void EXTI1_IRQHandler(){while(1){}}
	
void EXTI2_TSC_IRQHandler(){while(1){}}
	
//void EXTI3_IRQHandler(){while(1){}}
	
//void EXTI4_IRQHandler(){while(1){}}
	
void DMA1_Channel1_IRQHandler(){while(1){}}
	
void DMA1_Channel2_IRQHandler(){while(1){}}
	
void DMA1_Channel3_IRQHandler(){while(1){}}
	
void DMA1_Channel4_IRQHandler(){while(1){}}
	
void DMA1_Channel5_IRQHandler(){while(1){}}

void DMA1_Channel6_IRQHandler(){while(1){}}
	
void DMA1_Channel7_IRQHandler(){while(1){}}
	
void ADC1_2_IRQHandler(){while(1){}}
	
void USB_HP_CAN_TX_IRQHandler(){while(1){}}

//void USB_LP_CAN_RX0_IRQHandler(){while(1){}}
	
void CAN_RX1_IRQHandler(){while(1){}}
	
void CAN_SCE_IRQHandler(){while(1){}}
	
//void EXTI9_5_IRQHandler(){while(1){}}
	
//void TIM1_BRK_TIM15_IRQHandler(){while(1){}}
	
//void TIM1_UP_TIM16_IRQHandler(){while(1){}}
	
//void TIM1_TRG_COM_TIM17_IRQHandler(){while(1){}}
	
void TIM1_CC_IRQHandler(){while(1){}}
	
//void TIM2_IRQHandler(){while(1){}}
	
//void TIM3_IRQHandler(){while(1){}}
	
//void TIM4_IRQHandler(){while(1){}}
	
//void I2C1_EV_IRQHandler(){while(1){}}
	
//void I2C1_ER_IRQHandler(){while(1){}}
	
//void I2C2_EV_IRQHandler(){while(1){}}
	
//void I2C2_ER_IRQHandler(){while(1){}}
	
void SPI1_IRQHandler(){while(1){}}
	
void SPI2_IRQHandler(){while(1){}}
	
//void USART1_IRQHandler(){while(1){}}
	
//void USART2_IRQHandler(){while(1){}}
	
//void USART3_IRQHandler(){while(1){}}
	
//void EXTI15_10_IRQHandler(){while(1){}}

//void RTC_Alarm_IRQHandler(){while(1){}}

void USBWakeUp_IRQHandler(){while(1){}}
	
void TIM8_BRK_IRQHandler(){while(1){}}
	
//void TIM8_UP_IRQHandler(){while(1){}}
	
void TIM8_TRG_COM_IRQHandler(){while(1){}}
	
void TIM8_CC_IRQHandler(){while(1){}}
	
void ADC3_IRQHandler(){while(1){}}
	
void FMC_IRQHandler(){while(1){}}
	
void SPI3_IRQHandler(){while(1){}}
	
//void UART4_IRQHandler(){while(1){}}
	
//void UART5_IRQHandler(){while(1){}}
	
//void TIM6_DAC_IRQHandler(){while(1){}}
	
//void TIM7_IRQHandler(){while(1){}}

void DMA2_Channel1_IRQHandler(){while(1){}}
	
void DMA2_Channel2_IRQHandler(){while(1){}}
	
void DMA2_Channel3_IRQHandler(){while(1){}}

void DMA2_Channel4_IRQHandler(){while(1){}}
	
void DMA2_Channel5_IRQHandler(){while(1){}}
	
void ADC4_IRQHandler(){while(1){}}
	
void COMP1_2_3_IRQHandler(){while(1){}}
	
void COMP4_5_6_IRQHandler(){while(1){}}

void COMP7_IRQHandler(){while(1){}}
	
//void I2C3_EV_IRQHandler(){while(1){}}
	
//void I2C3_ER_IRQHandler(){while(1){}}
	
void USB_HP_IRQHandler(){while(1){}}
	
void USB_LP_IRQHandler(){while(1){}}
	
//void USBWakeUp_RMP_IRQHandler(){while(1){}}
	
void TIM20_BRK_IRQHandler(){while(1){}}
	
//void TIM20_UP_IRQHandler(){while(1){}}
	
void TIM20_TRG_COM_IRQHandler(){while(1){}}
	
void TIM20_CC_IRQHandler(){while(1){}}
	
void FPU_IRQHandler(){while(1){}}
	
void SPI4_IRQHandler(){while(1){}}

#endif //CUSTOMINTERRUPTCHECK

#ifdef __cplusplus
}
#endif

#endif //INTERRUPTCHECK_HPP