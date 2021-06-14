#include "tempsensor.h"

void init_temp(){  
   //Enable Digital Pin 2 -> PB25 enabled - in initpins()
   //set PB25 to high initially
   *AT91C_PIOB_OER = (1<<25);
   
   //Select Timer_Clock1 as TCCLK
   *AT91C_TC0_CMR = (*AT91C_TC0_CMR & 0x0);
   // Enable counter and make a sw_reset in TC0_CCR - TC Channel Control Register
   //*AT91C_TC0_CCR = (1<<0); // 0: no effect, 1 enables the clock if CLKDIS is not 1
   *AT91C_TC0_CCR = (1<<0); //enable clock
   *AT91C_TC0_CCR = (*AT91C_TC0_CCR|(1<<2)); //enable counter
   //Load counter to A when TIOA falling (TC_CMR0)
   *AT91C_TC0_CMR = (*AT91C_TC0_CMR | (0x2<<16));
   //Load counter to B when TIOA falling (TC_CMR0)
   *AT91C_TC0_CMR = (*AT91C_TC0_CMR | (0x2<<18));
     
   //TC0_IRQn = 27 SAM3X8E Timer Counter 0 (TC0) 
   NVIC_ClearPendingIRQ(TC0_IRQn);
   NVIC_SetPriority(TC0_IRQn,0);
   NVIC_EnableIRQ(TC0_IRQn);
    
   //Clear Line to Reset
   *AT91C_PIOB_CODR = (1<<25);
   msdelay(10); //t_reset
    
   //setup pulse high
   *AT91C_PIOB_SODR = (1<<25);
   msdelay(500);
}

void measure_begin(){ 
   //0: no effect, 1: counter reset and clock start
   *AT91C_TC0_CCR = (*AT91C_TC0_CCR|(1<<2));
   
   //enable Interrupts
   *AT91C_TC0_IER = AT91C_TC_LDRBS; 
   
   //enable PIN25 as ouput
   *AT91C_PIOB_OER = (1<<25);
   
   //start pulse
   *AT91C_PIOB_CODR = (1<<25);
   msdelay(1);  
   
   //ODR next to enable input (PIN25)
   *AT91C_PIOB_ODR = (1<<25);
}

float calc_temp(void)
{
  //disable interrupts
  *AT91C_TC0_IDR = AT91C_TC_LDRBS;
  
  //calc temperature and return
  int rb = *AT91C_TC0_RB;
  int ra = *AT91C_TC0_RA;
  double ret = (rb - ra);
  // T in us (tried a 1ms pulse and read the counter)
  ret = ret / 1.895; 
  ret = ret / (5.0);
  *AT91C_TC0_RB = 0x0;
  *AT91C_TC0_RA = 0x0;
  return ret/(22.4)-273.15; // calculated val 22.4 by measuring
}