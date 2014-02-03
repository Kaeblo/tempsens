// ez a lábkiosztás a mar házira készített breadboardos kapcsolásomnál mûködik
#ifndef pinsheader_
#define pinsheader_
#include "stm32f4xx.h"

// A B C D E DIG1 DIG3
#define A 	 GPIO_Pin_5 	//PA6
#define B  	 GPIO_Pin_15	//PA15
#define C 	 GPIO_Pin_8		//PA8
#define D 	 GPIO_Pin_6		//PA6
#define E 	 GPIO_Pin_1		//PA1
#define DIG1 GPIO_Pin_2		//PA2
#define DIG3 GPIO_Pin_10	//PA10
// G DIG2 F DP
#define G 	 GPIO_Pin_3		//PD3
#define DIG2 GPIO_Pin_6		//PD6
#define F 	 GPIO_Pin_2		//PD2
#define DP 	 GPIO_Pin_8		//PD8

static const uint16_t sevensegnumbers[8] = {DP,G,F,E,D,C,B,A};
static const uint16_t sevensegdigits[3]  = {DIG1,DIG2,DIG3};

static const uint8_t SevensegNum[12] = { //dp gfedcba
			0x3F,0x06,0x5B, //0,1,2
			0x4F,0x66,0x6D, //3,4,5
			0x7D,0x07,0x7F, //6,7,8
			0x6F,			//9
			0x80,0x00, //DOT 10, SPACE 11,
			};

	/*******FUNCTIONS********/


	//Writes one number to the given position on the seven segment display
extern void	SSWriteNum(uint8_t number,uint8_t position);

//global variables
extern uint8_t hundreds, tens, ones;
extern void SSWrite(float number,uint8_t dotpos);

void SSClearDisplay();

#endif



