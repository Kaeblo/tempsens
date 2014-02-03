#include "stm32f4xx.h"
#include "pins.h"
#include "main.h"
#include "ADC.h"
#include "MIC.h"



/*global variables*/
 uint8_t hundreds, tens, ones;

extern uint8_t DisplayOn;


int main(void)
{

	int ConvertedValue = 0,i; //Converted value readed from ADC
	float Temp =0 , MeasData =0;
	/*volatile uint32_t I2SData, ADDED_MIC = 0, MAXIMUM =0; ;
	uint32_t mask;*/

	init_function();
	PDM_init();
	adc_configure();//Start configuration
	//TIM_Cmd(TIM3,ENABLE);

 while (1){

	 if(ADC_EN){
     ConvertedValue = adc_convert();//Read the ADC converted value
     MeasData = Vdd* ConvertedValue/4095;
     Temp = ((MeasData-2.72)*100)+26;
     SSWrite(Temp,1);
     ADC_EN = 0;
	 }


  }

}

