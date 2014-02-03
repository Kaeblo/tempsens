#ifndef __main__
#define __main__

typedef unsigned short int u16;
typedef unsigned char u8;

void init_function(){
  GPIO_InitTypeDef portinit;	//GPIO init struktúra
  TIM_TimeBaseInitTypeDef timerinit; // Timer init struktúra
  NVIC_InitTypeDef nvicinit; // NVIC init struktúra
  EXTI_InitTypeDef extiinit; // EXTI init struktúra

  // Órajelet adunk a GPIOA és GPIOD portoknak
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD, ENABLE);

  portinit.GPIO_Mode = GPIO_Mode_OUT;
  portinit.GPIO_OType = GPIO_OType_PP;
  portinit.GPIO_Pin = A | B | C | D | E | DIG1 | DIG3;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, &portinit); // LED pinek beállítása


  GPIO_StructInit(&portinit);
  portinit.GPIO_Mode = GPIO_Mode_IN;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOD, &portinit); // BTN pin beállítása

  portinit.GPIO_Mode = GPIO_Mode_OUT;
  portinit.GPIO_OType = GPIO_OType_PP;
  portinit.GPIO_Pin = G | DIG2 | F | DP;
  portinit.GPIO_PuPd = GPIO_PuPd_NOPULL;
  portinit.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOD, &portinit); // LED pinek beállítása

  // Órajelet adunk a TIM4 idõzítõnek
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB1Periph_TIM3, ENABLE);

  // Beállítjuk az idõzítõ paramétereit TIM3 TIM4
  TIM_TimeBaseStructInit(&timerinit);
  timerinit.TIM_CounterMode = TIM_CounterMode_Up; // felfelé számolunk
  timerinit.TIM_Period = 41999; // eddig számolunk
  timerinit.TIM_Prescaler = 999; // a busz órajelet ennyivel osztjuk le
  TIM_TimeBaseInit(TIM4, &timerinit);

  TIM_TimeBaseStructInit(&timerinit);
  timerinit.TIM_CounterMode = TIM_CounterMode_Up; // felfelé számolunk
  timerinit.TIM_Period = 41999; // eddig számolunk
  timerinit.TIM_Prescaler =0; // 200Hz a három kijelzõhöz. 9 volt
  TIM_TimeBaseInit(TIM3, &timerinit);

  // Emgedélyezzük, hogy a TIM4 és TIM3 újratöltéskor IT-t generáljon
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  // Órajelet adunk a SYSCFG eszköznek (külsõ interrupthoz kell)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // Engedélyezzük, hogy az A port 0-s lába külsõ megszakítást generálhasson
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  // Beállítjuk, hogy mire keletkezzen megszakítás
  extiinit.EXTI_Line = EXTI_Line0; // 0-s vonal
  extiinit.EXTI_LineCmd = ENABLE; // engedélyez
  extiinit.EXTI_Mode = EXTI_Mode_Interrupt; // interrupt legyen
  extiinit.EXTI_Trigger = EXTI_Trigger_Rising; // felfutó élre
  EXTI_Init(&extiinit);

  // Beállítjuk a prioritás modelt (1 bit csoport, 3 bit al prioritás)
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // TIM4 timer IT beállítása
  nvicinit.NVIC_IRQChannel = TIM4_IRQn; // TIM4 IT
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // engedélyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 0; // legmagasabb csoport prioritás
  nvicinit.NVIC_IRQChannelSubPriority = 3; // csoporton belül a legmagasabb prioritás
  NVIC_Init(&nvicinit);

  // TIM3 timer IT beállítása
   nvicinit.NVIC_IRQChannel = TIM3_IRQn; // TIM4 IT
   nvicinit. NVIC_IRQChannelCmd = ENABLE; // engedélyez
   nvicinit.NVIC_IRQChannelPreemptionPriority = 0; // legmagasabb csoport prioritás
   nvicinit.NVIC_IRQChannelSubPriority = 1; // csoporton belül a legmagasabb prioritás
   NVIC_Init(&nvicinit);

  // EXTI0 IT beállítása
  nvicinit.NVIC_IRQChannel = EXTI0_IRQn; //EXTI0 vonal
  nvicinit. NVIC_IRQChannelCmd = ENABLE; // engedélyez
  nvicinit.NVIC_IRQChannelPreemptionPriority = 0; // alacsony csoport prioritás
  nvicinit.NVIC_IRQChannelSubPriority = 2; // csoporton belül a legmagasabb prioritás
  NVIC_Init(&nvicinit);

  // TIM4 elindítása
  TIM_Cmd(TIM4, ENABLE);
}

#endif
