#include "display.h"
extern int printed_days;


void clearDisplayLine(int pixelAmount,int line){

  Write_Data_2_Display(line*30+0); //y pos
  Write_Data_2_Display((line*30+0)>>8); //x pos
  Write_Command_2_Display(0x24);
    for(int i=0;i<pixelAmount;i++)
  {
     Write_Data_2_Display(0x00); 
     Write_Command_2_Display(0xC0);
  }
}
void clearMenu(void){
  clearDisplayLine(20,1);
  clearDisplayLine(21,3);
  clearDisplayLine(14,5);
  clearDisplayLine(27,7);
  clearDisplayLine(27,9);
  clearDisplayLine(20,14);
}
void clearAlarmMenu(void){
  clearDisplayLine(17,1);
  clearDisplayLine(27,5);
  clearDisplayLine(27,7);
  clearDisplayLine(15,14);
}
void clearLogging(void){
     clearDisplayLine(30,1);
     clearDisplayLine(30,2);
     for(int i=0;i<=printed_days;i++){
       clearDisplayLine(30,3+i);
     }
     clearDisplayLine(20,14);
}

void clearDisplay(int pixelAmount)
{
 Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x24);
  
  for(int i=0;i<pixelAmount;i++)
  {
     Write_Data_2_Display(0x00); 
     Write_Command_2_Display(0xC0);
  }
  Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x24); 
}
void print_data_2_display(char value)
{
  //IF 10 -> 0xA
  //if 11 -> 0x10
  //if 12 -> 0x3
  char print = value - 0x20; //get ascii value
  Write_Data_2_Display(print); //write the data to the display
  Write_Command_2_Display(0xC0); //write and increment ADP - next position
}
void init_display()
{
  //delay to let the voltage settle in
  delay(50);
  
  *AT91C_PIOD_CODR = (1<<0); //Clear Reset display - pin 25 (PD0)
  delay(50);
  *AT91C_PIOD_SODR = (1<<0); //set Reset display - pin 25 (PD0)
  
  Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x40);//Set text home address 
  Write_Data_2_Display(0x00); 
  Write_Data_2_Display(0x40); 
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x1E); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x41); // Set text area 
  Write_Data_2_Display(0x1e); 
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0x43); // Set graphic area 
  Write_Command_2_Display(0x80); // text mode 
  Write_Command_2_Display(0x94); // Text on graphic off
  
  // writing "clearing..." so you know he is clearing right now
   changeLineColumn(15, 18);
   char clearing[] = "clearing ...";
   printString2display(clearing, sizeof(clearing));
   clearDisplay(500); // clearing whole display (500 pixel)
}

unsigned char Read_Status_Display(void)
{
  unsigned char temp;
  
  set_data_bus_direction(INPUT); //databus input
  *AT91C_PIOC_SODR = (1<<13); //74HC245 dir as input - pin 50 high
  *AT91C_PIOC_CODR = (1<<12); //output enable 74HC245 - active low - pin 51 low
  *AT91C_PIOC_SODR = (1<<14); //set C/D - pin 49  //-!
  *AT91C_PIOC_CODR = (1<<15); //clear chip select display - pin 48 (CE)
  *AT91C_PIOC_CODR = (1<<16); //clear read display - pin 47 (RD)
  *AT91C_PIOC_SODR = (1<<17); //set write display - pin 46 (WR)
  delay(1);
  temp = (char) ((*AT91C_PIOC_PDSR & (0xFF << 2)) >> 2); //read databus
  
  *AT91C_PIOC_SODR = (1<<15); //set chip select display  
  *AT91C_PIOC_SODR = (1<<16); //set read display
  *AT91C_PIOC_SODR = (1<<12); //output disable 74HC245
  *AT91C_PIOC_CODR = (1<<13); //74HC245 dir as output - pin 50 low
  
  return temp;
}

void Write_Command_2_Display(unsigned char Command)
{
  //wait until status ok (ok = 2 first 2 bits are set)
  while((Read_Status_Display() & 0x03) != 0x03){ //checking if first 2 bit are high
    msdelay(100);
  }
  
  *AT91C_PIOC_CODR = (0xFF<<2); //clear databus
  *AT91C_PIOC_SODR = (Command<<2); //set command to databus
  
  *AT91C_PIOC_CODR = (1<<13); //74HC245 dir as output - pin 50 low
  *AT91C_PIOC_CODR =(1<<12); //output enable 74HC245 - active low
  set_data_bus_direction(OUTPUT); //databus output
  *AT91C_PIOC_SODR = (1<<14); //set C/D high
  *AT91C_PIOC_CODR = (1<<15); //clear chip select display - pin 48 (CE)
  
  *AT91C_PIOC_CODR =(1<<17); //clear write display pin 47 (WR)
  Delay_write(20);
  *AT91C_PIOC_SODR = (1<<15); //set chip select display 
  *AT91C_PIOC_SODR =(1<<17); //set write display pin 47 (WR)
  *AT91C_PIOC_SODR = (1<<12); //output disable 74HC245
 
  set_data_bus_direction(INPUT); //databus input
}

void Write_Data_2_Display(unsigned char Data)
{
  //Wait until Read_Status_Display returns an OK
  while(( Read_Status_Display()  & 0x03) != 0x03){
    msdelay(100);
  }
  *AT91C_PIOC_CODR = (0xFF<<2); //clear databus
  *AT91C_PIOC_SODR = (Data<<2); //set command to databus
  
  *AT91C_PIOC_CODR = (1<<13); //74HC245 dir as output - pin 50 low
  *AT91C_PIOC_CODR =(1<<12); //output enable 74HC245 - active low
  set_data_bus_direction(OUTPUT); //databus output
  *AT91C_PIOC_CODR = (1<<14); //clear C/D 
  *AT91C_PIOC_CODR = (1<<15); //clear chip select display - pin 48 (CE)
  
  *AT91C_PIOC_CODR =(1<<17); //clear write display pin 47 (WR)
  Delay_write(20);
  *AT91C_PIOC_SODR = (1<<15); //set chip select display 
  *AT91C_PIOC_SODR =(1<<17); //set write display pin 47 (WR)
  *AT91C_PIOC_SODR = (1<<12); //output disable 74HC245
 
  set_data_bus_direction(INPUT); //databus input
}

void printString2display(char* str, int size){
  int i = 0;
  str[size] = '\0';
  while((i != size) && (str[i] != '\0')){
    //Write_Data_2_Display((str[i])); //write the data to the display
    print_data_2_display(str[i]);
    i++;
  }
}

void changeLine(int line){
  Write_Data_2_Display(line*30+0); //y pos
  Write_Data_2_Display((line*30+0)>>8); //x pos
  Write_Command_2_Display(0x24);
}

void changeLineColumn(int line, int col){
  Write_Data_2_Display(line*30+col); //y pos
  Write_Data_2_Display((line*30+col)>>8); //x pos
  Write_Command_2_Display(0x24);
}

