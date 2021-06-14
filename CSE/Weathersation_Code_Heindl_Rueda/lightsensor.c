#include "lightsensor.h"
#include "servo.h"

//an alarm can be shown
//extern int alarm;
extern double light_intensity;
extern int return_of_Temp;

//Photosensor
void initLightsensor(void){
  //enable PMC ADC -> peripheral identifier = 37 -> only up to 32 in PCER0 
  // -> PCER1 bit 5 = peripheral identifier 37
  *AT91C_PMC_PCER1 = (1<<5);
  
  //ADCClock = MCK / ( (PRESCAL+1) * 2 ) -> Prescale = 2:
  //ADDClock = 84Mhz / ((2+1)*2) = 14Mhz (14Mhz are given -> prescale 2 -> set bit 9)
  *AT91C_ADCC_MR = (1<<9);
}

//measure the brightness -> lower value = brighter
double measureBrightness(void){
  double result;
  double BitPerVolt = 4096/3.3; //4096 sampling rate - 3,3V max Voltage
  
  //Select channel in ADC_CHER (Channel 2)
  *AT91C_ADCC_CHER = (1<<2);
  
  //Start an ADC in ADC_CR (Register 1)
  *AT91C_ADCC_CR = (1<<1);
  
  //Wait until IDR_DRDY - ReadIn Register called AT91C_ADCC_SR
  while((*AT91C_ADCC_SR & (1<<24)) != (1<<24));

  result = *AT91C_ADCC_LCDR;
  result = result/BitPerVolt;
  return result;
}

int find_brightestDegree(void){
  int i; 
  double brightest = measureBrightness();
  int degree = 0; 
  double brightness_cur;
  
  for(i = 0; i<180; i++){
    //to ensure measurement every second
    if((i%20) == 0)
    {
      temp_measuring();
    }
    move_to_position(i);
    light_intensity= measureBrightness();
    if((brightness_cur=measureBrightness())<brightest){
      brightest=brightness_cur;
      light_intensity= brightness_cur;
      degree=i;
    }
    msdelay(2000);
  }
  move_to_position(degree);
  return degree;
}