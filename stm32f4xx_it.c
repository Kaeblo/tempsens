#include "stm32f4xx_it.h"
#include "pins.h"
#include "ADC.h"
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

uint8_t DisplayOn = 0;
uint8_t CLAPLIMIT=12;

int16_t samp64 =0, MAXIMUM = 0;
void TIM3_IRQHandler(void)
{
	static uint8_t DisplayState;
	// lekérdezzük, hogy mi okozta a megszakítást
	if ( TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		if(DisplayOn == 2){

			switch(DisplayState){
			case 1:
				SSWriteNum(SevensegNum[hundreds],1);
				DisplayState = 2;
				break;
			case 2:
				SSWriteNum(SevensegNum[tens] | SevensegNum[10] ,2);
				DisplayState = 3;
				break;
			case 3:
				DisplayState = 1;
				SSWriteNum(SevensegNum[ones],3);
				break;
			default:
				DisplayState =1;
				}
		}
		else{ // switched off display
			switch(DisplayState){
			case 1:
				SSWriteNum(SevensegNum[11],1);
				DisplayState = 2;
				break;
			case 2:
				SSWriteNum(SevensegNum[11],2);
				DisplayState = 3;
				break;
			case 3:
				DisplayState = 1;
				SSWriteNum(SevensegNum[11],3);
				break;
			default:
				DisplayState =1;
				}
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM4_IRQHandler(void)
{
	static uint8_t SecCounter = 0;

	// lekérdezzük, hogy mi okozta a megszakítást
	if ( TIM_GetITStatus(TIM4, TIM_IT_Update))
	{
		SecCounter++;
		if(MAXIMUM > CLAPLIMIT){
			SecCounter =0;
			MAXIMUM = 0;
			DisplayOn = 2; //Switch dislay on
		}
		if(SecCounter == 9){
			if(MAXIMUM > CLAPLIMIT)DisplayOn = 2; //Switch dislay on
			if(DisplayOn == 2)DisplayOn = 0;//Switch display off
			SecCounter = 0; //reset SecCounter in every 5 sec
		}
		ADC_EN = 1;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
FunctionalState EnableTimer = ENABLE;
void EXTI0_IRQHandler(void)
{
	// lekérdezzük, hogy mi okozta a megszakítást
	if ( EXTI_GetITStatus(EXTI_Line0))
	{
		// negáljuk az állapotváltozónkat
		EnableTimer=!EnableTimer;
		// az állapotváltozónak megfelelõen engedélyezzük/tiltjuk a timert
		TIM_Cmd(TIM4, EnableTimer);
		// töröljük a megszakítást jelzõ flaget
		EXTI_ClearITPendingBit(EXTI_Line0);
	}

}


void SPI2_IRQHandler(void){
	uint16_t mask,i;
	uint16_t app;
	static uint8_t k = 0;

	/* Check if data are available in SPI Data register */
	  if (SPI_GetITStatus(SPI2, SPI_I2S_IT_RXNE) != RESET)
	  {

			app = SPI_I2S_ReceiveData(SPI2);// 64 decimation

			  mask = 0x01;
			  for(i=0;i<16;i++)
			  	  {
				  	  if(mask & app)
				  		  samp64 += 1;
				  	  else
				  		  samp64 -= 1;
				  	  mask <<= 1;
			  	  }
			 if(k==3)
				  {
				 	  k=0;
				 	  if(MAXIMUM <samp64) MAXIMUM = samp64;
				 	 // if(samp64 > CLAPLIMIT) DisplayOn = 1;
				 	  samp64 = 0;
				  }

			 k++;
	  	 }
	return;
}


