#include "pin_init.h"

void pin_init(void) //initialize all needed pins at beginning
{
  
    //clear wpen bit - needed for a few adresses to access
  *((volatile unsigned int *)0x400E14E4) = (0<<0);
  *((volatile unsigned int *)0x400E10E4) = (0<<0);
  *((volatile unsigned int *)0x400800E4) = (0<<0);
  
    SystemInit(); // Disables the Watchdog and setup the MCK 
    
    //enable Clocks
    *AT91C_PMC_PCER = (1<<14); //Port D // (PMC) Peripheral Clock Enable Register 0:31 PERI_ID
    *AT91C_PMC_PCER = (1<<13); //Port C // (PMC) Peripheral Clock Enable Register 0:31 PERI_ID
    *AT91C_PMC_PCER = (1<<12); //port B // (PMC) Peripheral Clock Enable Register 0:31 PERI_ID
    *AT91C_PMC_PCER = (1<<11); //port A // (PMC) Peripheral Clock Enable Register 0:31 PERI_ID
    *AT91C_PMC_PCER = (1<<27); //TC0    // (PMC) Peripheral Clock Enable Register 0:31 PERI_ID
 
    //PIO Enable Register
    *AT91C_PIOD_PER = (1 << 3); //enable PD3 (port 28)
    *AT91C_PIOD_PER = (1 << 2); //enable PD2 (port 27)
    *AT91C_PIOC_PER = (1 << 7); //enable PD7 (port 39)
    *AT91C_PIOC_PER = (1 << 8); //enable PD8 (port 40)
    *AT91C_PIOC_PER = (1 << 9); //enable PD9 (port 41)
    
    //set pull up resistor
    *AT91C_PIOD_PPUDR = (1 << 3); // PD3 (port 28)
    *AT91C_PIOD_PPUDR = (1 << 2); // PD2 (port 27)
    *AT91C_PIOC_PPUDR = (1 << 7); // PD7 (port 39)
    *AT91C_PIOC_PPUDR = (1 << 8); // PD8 (port 40)
    *AT91C_PIOC_PPUDR = (1 << 9); // PD9 (port 41)
    

    //set Outputs
    *AT91C_PIOD_OER = (1 << 3); //set pin 28 as Output
    *AT91C_PIOD_OER = (1 << 2); //set pin 27 as Output
    
    //set Inputs
    *AT91C_PIOC_ODR = (1 << 2); //set pin 34 as Input
    *AT91C_PIOC_ODR = (1 << 3); //set pin 35 as Input
    *AT91C_PIOC_ODR = (1 << 4); //set pin 36 as Input
    *AT91C_PIOC_ODR = (1 << 5); //set pin 37 as Input
    
    //display setup outputs
    *AT91C_PIOD_PER = (1<<0); //enable PD0 (port 25)
    *AT91C_PIOC_PER = (1<<12); //enable PC12 (port 51)
    *AT91C_PIOC_PER = (1<<13); //enable PC13 (port 50)
    *AT91C_PIOC_PER = (1<<14); //enable PC14 (port 49)
    *AT91C_PIOC_PER = (1<<15); //enable PC15 (port 48)
    *AT91C_PIOC_PER = (1<<16); //enable PC16 (port 47)
    *AT91C_PIOC_PER = (1<<17); //enable PC17 (port 46)
    *AT91C_PIOC_PER = (1<<18); //enable PC18 (port 45)
    *AT91C_PIOC_PER = (1<<19); //enable PC19 (port 44)
    
    *AT91C_PIOD_PPUDR = (1<<0); // PD0 (port 25)
    *AT91C_PIOC_PPUDR = (1<<12); // PC12 (port 51)
    *AT91C_PIOC_PPUDR = (1<<13); // PC13 (port 50)
    *AT91C_PIOC_PPUDR = (1<<14); // PC14 (port 49)
    *AT91C_PIOC_PPUDR = (1<<15); // PC15 (port 48)
    *AT91C_PIOC_PPUDR = (1<<16); // PC16 (port 47)
    *AT91C_PIOC_PPUDR = (1<<17); // PC17 (port 46)
    *AT91C_PIOC_PPUDR = (1<<18); // PC18 (port 45)
    *AT91C_PIOC_PPUDR = (1<<19); // PC19 (port 44)
    
    *AT91C_PIOD_OER = (1<<0); // PD0 (port 25) as Output
    *AT91C_PIOC_OER = (1<<12); // PC12 (port 51) as Output
    *AT91C_PIOC_OER = (1<<13); // PC13 (port 50) as Output
    *AT91C_PIOC_OER = (1<<14); // PC14 (port 49) as Output
    *AT91C_PIOC_OER = (1<<15); // PC15 (port 48) as Output
    *AT91C_PIOC_OER = (1<<16); // PC16 (port 47) as Output
    *AT91C_PIOC_OER = (1<<17); // PC17 (port 46) as Output
    *AT91C_PIOC_OER = (1<<18); // PC18 (port 45) as Output
    *AT91C_PIOC_OER = (1<<19); // PC19 (port 44) as Output
    
    //end display setup outputs
    
    //tempsensor
    //Enable Digital Pin 2 -> PB25 enabled
    *AT91C_PIOB_PER = (1<<25);
    
}
void set_data_bus_direction(int dir) //0 = OUTPUT, 1 = INPUT
{
  if(dir == OUTPUT)
  {
    //PIN 34 - PIN 41 Data Bus
    *AT91C_PIOC_OER = (1<<2); //Pin 34
    *AT91C_PIOC_OER = (1<<3); //PIN 35    
    *AT91C_PIOC_OER = (1<<4); //PIN 36 
    *AT91C_PIOC_OER = (1<<5); //PIN 37
    *AT91C_PIOC_OER = (1<<6); //PIN 38
    *AT91C_PIOC_OER = (1<<7); //PIN 39
    *AT91C_PIOC_OER = (1<<8); //PIN 40
    *AT91C_PIOC_OER = (1<<9); //PIN 41
  }
  else
  {
    //PIN 34 - PIN 41 Data Bus
    *AT91C_PIOC_ODR = (1<<2); //Pin 34
    *AT91C_PIOC_ODR = (1<<3); //PIN 35    
    *AT91C_PIOC_ODR = (1<<4); //PIN 36 
    *AT91C_PIOC_ODR = (1<<5); //PIN 37
    *AT91C_PIOC_ODR = (1<<6); //PIN 38
    *AT91C_PIOC_ODR = (1<<7); //PIN 39
    *AT91C_PIOC_ODR = (1<<8); //PIN 40
    *AT91C_PIOC_ODR = (1<<9); //PIN 41
  }
}