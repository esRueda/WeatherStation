#ifndef DISPLAY
#define DISPLAY

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "pin_init.h"

//functions display
void init_display();
unsigned char Read_Status_Display(void);
void Write_Command_2_Display(unsigned char Command);
void Write_Data_2_Display(unsigned char Data);
void set_data_bus_direction(int dir);
void print_data_2_display(char value);
void printString2display(char* str, int size);
void changeLine(int line);
void changeLineColumn(int line, int col);
void clearDisplayLine(int pixelAmount,int line);
void clearMenu(void);
void clearLogging(void);
void clearAlarmMenu(void);




#endif //DISPLAY