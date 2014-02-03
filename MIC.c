#include "MIC.h"

void PDM_init(){
	/*GPIO_InitTypeDef portinit;	//GPIO init struktúra

	I2S_InitTypeDef I2S_InitType;

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE); // spi kimenetek engedélyezése
	 RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI2EN,ENABLE);// i2s/spi engedélyezése

	  portinit.GPIO_Mode = GPIO_Mode_AF;
	  portinit.GPIO_Pin = GPIO_Pin_3;
	  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  portinit.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOC, &portinit); // PC3- MIC DOUT

	  portinit.GPIO_Mode = GPIO_Mode_AF;
	  portinit.GPIO_OType = GPIO_OType_PP;
	  portinit.GPIO_Pin = GPIO_Pin_10;
	  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  portinit.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOB, &portinit); // PB10- MIC CLK


	  SPI_I2S_DeInit(SPI2); //  reset SPI peripheral
	   I2S_InitType.I2S_AudioFreq = I2S_AudioFreq_44k;
	   I2S_InitType.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
	   I2S_InitType.I2S_Mode = I2S_Mode_MasterRx;  //master adatfogadás
	   I2S_InitType.I2S_DataFormat = I2S_DataFormat_16b;
	   I2S_InitType.I2S_Standard = I2S_Standard_MSB;
	   I2S_InitType.I2S_CPOL = I2S_CPOL_High;
	   I2S_Init(SPI2, &I2S_InitType);

	   GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2);
	   GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2);

	  // RCC_PLLI2SCmd(ENABLE);
	   SPI_Cmd(SPI2, ENABLE);


	  return;*/

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_SPI2); /* I2S2_CK    */

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_SPI2); /* I2S2_SD    */


    I2S_InitTypeDef I2S_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    SPI_I2S_DeInit(SPI2);
    I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
    I2S_InitStructure.I2S_Standard = I2S_Standard_LSB;
    I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_High;
    I2S_InitStructure.I2S_Mode = I2S_Mode_MasterRx;
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;

    RCC_I2SCLKConfig(RCC_I2S2CLKSource_PLLI2S);
    RCC_PLLI2SCmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_PLLI2SRDY)==RESET);

    I2S_Init(SPI2, &I2S_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Enable the Rx buffer not empty interrupt */
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE);
    I2S_Cmd(SPI2, ENABLE);
}

uint32_t I2SReceiveData(){
	volatile uint32_t data;
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)); //we wait while Rx buffer is empty
	data = SPI_I2S_ReceiveData(SPI2); //read data
	return data;
}
