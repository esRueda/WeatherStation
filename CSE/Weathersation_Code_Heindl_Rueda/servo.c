#include "servo.h"

void servo_init(void)
{
  //initialize the PWM
  *AT91C_PMC_PCER1 = (1<<4); //4 = instance 36 = PWM (peripheral identifier)
  
  //disable output - pin 17 - to make it cotrollable by the peripherals
  *AT91C_PIOB_PDR = (1<<17);
  //multi driver disable - only peripherals (PWM) can controll the pin
  *AT91C_PIOB_MDDR = (1<<17);  
  //pwm controll pin 17 (ABSR from datasheet equals ABM in .h file)
  *AT91C_PIOB_ABMR = (1<<17);
  
  //enable PWM
  *AT91C_PWMC_ENA = (1<<1);
  
  //set prescaler (clk/32)
  *AT91C_PWMC_CH1_CMR = 0x5;
  
  //set PWM_CPRD to 20ms (CPRDR = CPRD)
  *AT91C_PWMC_CH1_CPRDR = 52500; //oscilloscope -> calculated value (52500) -> did not work
  
  //move to mid position
  move_to_position(0);

}

int move_to_position(int pos)
{
  float val = 0;
  
  //if((pos >= 0) && (pos <= 180)){ //otherwise illegal position
    float abs_pos = (float)pos;
    val=(-0.037)*abs_pos*abs_pos+36.6*abs_pos+900;
    //explanation:
    //0 degrees = (val=900) --> offset
    //90 degrees = (val=3900) --> middle pos
    //180 degrees = (val6300) -->max
    //=> f(x)=A*x^2 + B*x + C 
  //}
    
    //set value for high of pwm (CDTY = CDTYR)
    *AT91C_PWMC_CH1_CDTYR = (int)val;
    
    return 0;

}