#include "keypad.h"

void make_Columns_high(void)
{
  *AT91C_PIOC_SODR = AT91C_PIO_PC7; //set col 2 pin high 
  *AT91C_PIOC_SODR = AT91C_PIO_PC8; //set col 1 pin high 
  *AT91C_PIOC_SODR = AT91C_PIO_PC9; //set col 0 pin high 
}

void make_Columns_Input(void)
{
  //Colums - set to input
  *AT91C_PIOC_ODR = AT91C_PIO_PC7; //pin 39 - col 2
  *AT91C_PIOC_ODR = AT91C_PIO_PC8; //pin 40 - col 1
  *AT91C_PIOC_ODR = AT91C_PIO_PC9; //pin 41 - col 0
}

//initialize all pins needed for Keypad
void init_keypad_pins(void)
{
  
  // clear pin 27 == clear OE KEY BUS
  *AT91C_PIOD_CODR = AT91C_PIO_PD2; 
  
  //Colums
  *AT91C_PIOC_OER = AT91C_PIO_PC7; //set pin 39 as Output - Column 2
  *AT91C_PIOC_OER = AT91C_PIO_PC8; //set pin 40 as Output - Column 1
  *AT91C_PIOC_OER = AT91C_PIO_PC9; //set pin 41 as Output - Column 0
  
   //Rows 
   *AT91C_PIOC_ODR = AT91C_PIO_PC5; //set pin 37 as Input - Row 0
   *AT91C_PIOC_ODR = AT91C_PIO_PC4; //set pin 36 as Input - Row 1
   *AT91C_PIOC_ODR = AT91C_PIO_PC3; //set pin 35 as Input - Row 2
   *AT91C_PIOC_ODR = AT91C_PIO_PC2; //set pin 34 as Input - Row 3
}

unsigned int check_value;
//function for keypad 
unsigned int read_keypad(void)
{
   //initialize the needed pins
   init_keypad_pins();

   make_Columns_high();
   
   for(int i=0; i<3; i++)//loop columns
   { 
        *AT91C_PIOC_CODR = (1 << (9-i)); //set columns 0-2 to low
        for(int j=0; j<4;j++)// loop row
        { 
            if ((*AT91C_PIOC_PDSR & (1 << (5-j))) == (0 << (5-j)))
            {
              //deactivate keypad 
              *AT91C_PIOD_SODR = AT91C_PIO_PD2;
              //set the colums to input again
              make_Columns_Input();
              

              unsigned int value=j*3+i+1; //calculate value
              return value;
            }
            check_value=j*3+i+1;
        }
       *AT91C_PIOC_SODR = (1 << (9-i)); //set used col to high again
    }

    //dissable 74HC245 - set to high to disable
   *AT91C_PIOD_SODR = AT91C_PIO_PD2;
    
   make_Columns_Input();//make the columns to input
  
   return 0;//if no key detected found return 0
}

