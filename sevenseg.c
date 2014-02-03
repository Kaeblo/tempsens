#include "pins.h"

	//Writes one number to the given position on the seven segment display
void	SSWriteNum(uint8_t number,uint8_t position){
	 uint8_t i,masked,mask,d1,d2,d3;
	 volatile int delay;
		mask = 0x80;

		GPIO_WriteBit(GPIOA,DIG3, 1 ); //DIG3
		GPIO_WriteBit(GPIOD,DIG2, 1 ); //DIG2
		GPIO_WriteBit(GPIOA,DIG1, 1 );


		for(i=0;i<8;i++){
			  if(i<3){
				  masked = number & mask;
				  GPIO_WriteBit(GPIOD,sevensegnumbers[i],masked);
				  mask = mask>> 1;
			  }
			  else
			  {
				  masked = number & mask;
				  GPIO_WriteBit(GPIOA,sevensegnumbers[i],masked);
				  mask = mask>> 1;
			  }

			}

		if(position == 1) d1 = 0; else d1 = 1;
		if(position == 2) d2 = 0; else d2 = 1;
		if(position == 3) d3 = 0; else d3 = 1;
		GPIO_WriteBit(GPIOA,DIG3, d3 ); //DIG3
		GPIO_WriteBit(GPIOD,DIG2, d2 ); //DIG2
		GPIO_WriteBit(GPIOA,DIG1, d1 ); //DIG1

		return;
}


void SSWrite(float number, uint8_t dotpos){
	switch(dotpos){
	case 0:

			if(number>0)
			{
					if (  (number/100)  >= 1 )
							hundreds = number/100;  /* First digit */
					   else
							hundreds = 11;			  /* writing space */
			}
			else
				  hundreds = 11;	/* writing - */

			if ( (number/10) >= 1 )
				tens =  (( ((int)number) % 100) / 10) ; /* writing second digit */
			else
				tens = 11;	/* writing space */

			ones = ( ((int)number) % 10 ) ;
			break;
	case 1:

				if (  (number/10)  >= 1 )
					  hundreds = number/10;  /* First digit */
				  else
					  hundreds = 11;			  /* writing space */

				tens =  ( ((int)number) % 10) ; /* writing second digit */
				ones = ( (int)(number * 10) % 10);
				break;
	}


	TIM_Cmd(TIM3, ENABLE);
	return;
}

void SSClearDisplay(){};
