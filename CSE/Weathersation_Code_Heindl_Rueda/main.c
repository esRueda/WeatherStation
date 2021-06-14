#include <stdio.h>
#include <stdlib.h>

#include "system_sam3x.h" 
#include "at91sam3x8.h" 
#include "core_cm3.h"
#include "pin_init.h"
#include "display.h"
#include "Keypad.h"
#include "tempsensor.h"
#include "lightsensor.h"
#include "servo.h"
#include "logging.h"
#include "menu.h"


//global Values
unsigned int Key_Btn=0;       // return of read_keypad()
int lastState=0;              // for deleting display
int action=0;                 // return value of startMenu()

unsigned int msTicks = 0;     // needed for delays
//unsigned int Count_btn=0;
//unsigned int i = 0;
unsigned int counter=0;       // PIOD_Handler() //needed for Led
unsigned int temp=0;          // PIOD_Handler() //needed for Led
unsigned int RB = 0;          // check if RA==RB
unsigned int RA = 0;          // used for debugging at Tempsensor


//temperature   //interrupts
unsigned int temp_ready = 0;  // for temp sensor 
int time = 1200000;             // measuring time of Temperature
int fast =0;                  // 1=fastmode, 0=slowmode
int enableTempUpdate=0;       // 1=update Temperature digits
unsigned int tempcount = 0;   // increases with Interrupt
int return_of_Temp=0;         // only for debugging

//variables for return of photosensor
double light_intensity = 0.0;
int degree = 0;

//ALARM 
int alarm = 0;  
//int alarmchanged = 0;
double low_alarm = 19.00;
double high_alarm = 23.00;


//logging
int templogging[1440] = {0};  // 60 minutes * 24 hours = 1 day //change to 1440
int pos = 0;                  // position in the array
//int pos_change = 0;     // detect if value was meassured
DAY* week[7];                 // 1 week = 7 days
int day = 0;
int printed_days=0;           // to know how many days were printed
/*??*/void loggedData(void);


/*
void Set_Led(void){
  if ((*(unsigned int *)AT91C_PIOD_ODSR & (1 << 3)) == (1 << 3)){
            *AT91C_PIOD_CODR = AT91C_PIO_PD3;
          }
          else{
            *AT91C_PIOD_SODR = AT91C_PIO_PD3;
          }
}*/

//fastmode
void changeFastMode(void){
    if(fast == 0){            // change to fast mode
      time = 20000;
      fast = 1;
    }else{                    // change to slow mode
      time = 1200000;
      fast = 0;
    }
    clearMenu();
    changeLine(1);
    char start[] = "Press # to start!";
    printString2display(start, sizeof(start));
    lastState=0;// for deleting in startMenu() // only deleting line 1
}


void PIOD_Handler(void){
   counter=msTicks/20000;
   if(counter>temp){
     //Set_Led();
     temp=counter;
   }
   
}
//interrputhandler temp
void TC0_Handler(void)
{
  if((*AT91C_TC0_SR & AT91C_TC_LDRBS) == AT91C_TC_LDRBS){
    temp_ready = *AT91C_TC0_SR && (1<<6); //1: RB Load has occurred since the last read of the Status Register, if WAVE = 0.
  }
  RB++;
}
void SysTick_Handler(void){
  msTicks++;
  tempcount++;
}
void delay(int Value)
{ 
  // slow delay
  msTicks = 0;
  while(msTicks/1000 < Value);
  return;
}
void msdelay(int Value)
{ 
  //value equals time in milliseconds - one tick per millisecond - wait until value
  msTicks = 0;
  while(msTicks < Value);
  return;
}

void Delay_write(int Value){  //ugly delay
  int i;
  for(i=0;i<Value;i++)
    asm("nop");
}

void alarmupdate(void){
  if(templogging[pos-1] < low_alarm || templogging[pos-1] > high_alarm)
  {
    if(alarm==0){ //changed from NO ALARM to ALARM -> printing ALARM
        //printing ALARM
        changeLineColumn(14, 25);
        char alarmText[] = "ALARM";
        printString2display(alarmText, sizeof(alarmText));
        alarm = 1; //set ALARM
     }
   }
   else{
      if(alarm==1){//changed from ALARM to NO ALARM -> deleting print
        //Deleting ALARM
        changeLineColumn(14, 25);
        char clear_alarm[] = "     ";
        printString2display(clear_alarm, sizeof(clear_alarm));
        alarm = 0; //deleting ALARM
     }
   }
}
void updateTemp(int temp){
  char temperature_print[4];
  changeLineColumn(1,13);
  snprintf(temperature_print ,4, "%d   ", temp);
  printString2display(temperature_print, sizeof(temperature_print));
}
void updateLight(double light){
  char light_intensity_print[4];
  changeLineColumn(3,17);
  snprintf(light_intensity_print ,6, "%0.2f ", light);
  printString2display(light_intensity_print, sizeof(light_intensity_print));
}
void temp_measuring(void){
    //temp
    if(tempcount >= time){  //start measurement every min - fast mode every sec
      tempcount = 0;        // increases at SysTick_Handler()
      measure_begin();      //tempsensor.c
      RA++;                 // for Debugging (see if RA == RB)
    }
    if(temp_ready == 1){ //when interrupt detected - calculate temperature - templogging
      templogging[pos] = calc_temp(); // save Temperature in templogging
      pos++;  // increase pos of Templogging 
      alarmupdate(); // see if Alarm has occured
      if(enableTempUpdate==1){  // only 1 if homescreen is shown on the display
        updateTemp(templogging[pos-1]); // updates only the digits of temperature
        updateLight(measureBrightness()); // same for Brightness
      }
      if(pos >= 1440){ //if Temperature array full -> fill from beginning -> store day in day array -> calc values for day
        pos = 0;
        week[day%7] = calculateday(&templogging[0], 20, day); // calc average, min, max for this Day
        day++;
      }
      temp_ready = 0; // waiting for new interrupt
    }
}

void homescreen(int temp, double light_intensity, int degree){
  //printing Temperature
  char temperature_print[19];
  changeLine(1);
  snprintf(temperature_print ,19, "Temperature: %d", temp);
  printString2display(temperature_print, sizeof(temperature_print));

  //printing Brightness
  char light_intensity_print[21];
  changeLine(3);
  snprintf(light_intensity_print ,21, "Light Intensity: %0.2f", light_intensity);
  printString2display(light_intensity_print, sizeof(light_intensity_print));
  
  //printing detected degrees
  char degree_print[12];
  changeLine(5);
  snprintf(degree_print ,12, "Degree: %i", degree);
  printString2display(degree_print, sizeof(degree_print));
 
  //printing Exit line
  changeLine(14);
  char menutext[] = "Press # for menu!";
  printString2display(menutext, sizeof(menutext));
}



void loggedData(void){    
  week[day%7] = calculateday(&templogging[0], pos, day); // calc average, min, max for no finished Day
 
  //clearDisplay(150); 
  changeLine(1);
  char init_text[] = "DAY  |  AVG  |  MIN  |  MAX";
  printString2display(init_text, sizeof(init_text));
  
  changeLine(2);
  char line_text[] = "------------------------------";
  printString2display(line_text, sizeof(line_text));
  
  changeLine(14);
  char menutext[] = "Press * to exit!";
  printString2display(menutext, sizeof(menutext));
  
  

  int i = (day%7)+1; // start day = [0] => day 1
  int j = 3; // start line
  char day_print[30];
  
  while(i != (day%7))
  {
    if(((day < 7) && (i < day)) || (day > 7)){
      snprintf(day_print , 30, " %3i | %3.2f | %3.2f | %3.2f", (week[i]->day)+1, week[i]->avg, week[i]->min, week[i]->max);
      changeLine(j);
      printString2display(day_print, sizeof(day_print));
      j++;
      printed_days=j-3; //for deleting // start from 3 to J
    }
    i++;//increment i %7
    i = i % 7;  //only 7 elements in array
  }
  
  //at least 1 row to print
  snprintf(day_print , 30, " %3i | %3.2f | %3.2f | %3.2f", (week[i]->day)+1, week[i]->avg, week[i]->min, week[i]->max);
  changeLine(j);
  printString2display(day_print, sizeof(day_print));
  
  int key =0;
  do{ 
    key = read_keypad();
    //to ensure measurement every second
    temp_measuring();

  }while(key != 10); // press 10 to exit to homescreen
}

/*-------------------MAIN-------------------*/
void main(void){ 
 
    SysTick_Config(SystemCoreClock / 1000);

    //initialize all needed pins 
    pin_init();  // defined in pin_init.c
    
    //init tempsensor
    init_temp();
    measure_begin();

    //init servo
    servo_init();
    
    //init photosensor
    initLightsensor();

  //initialize display - clear (value from before is stored on display)
    init_display();
   
  

      
    //init week array with 0 for every value for every day
  DAY* newday = (DAY*)malloc(sizeof(DAY));
  newday->avg = 0.0;
  newday->day = 0;
  newday->max = 0.0;
  newday->min = 0.0;
  int i;
  for(i = 0; i < 7; i++){
    week[i] = newday;
  }
    changeLine(1);
    char start[] = "Press # to start!";
    printString2display(start, sizeof(start));
    lastState=0; // for deleting in startMenu() // only deleting line 1


    while(1){ 

      temp_measuring();

      Key_Btn = read_keypad(); // reading Keypad // waiting for key 12
      if(Key_Btn == 12){ //# detected - start menu
        action=0;  
        action = startMenu();
        if(action == 6)
        {
         clearMenu();
         degree = find_brightestDegree();
         light_intensity = measureBrightness();
         homescreen(templogging[pos-1], light_intensity, degree);
         enableTempUpdate=1; // enable function call updateTemp() in temp_measuring()
         lastState=1;
         
        }
        else if(action == 2)
        {
          enableTempUpdate=0;
          clearMenu();
          loggedData(); // shows the logged data --> exit via Key 10
          clearLogging(); // clearing the logging display
          homescreen(templogging[pos-1], light_intensity, degree);
          enableTempUpdate=1; // enable function call updateTemp() in temp_measuring()
          lastState=1;  
        }
      }       
    }// while(1) 
} // main 


