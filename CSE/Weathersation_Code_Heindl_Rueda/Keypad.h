#ifndef KEYPAD
#define KEYPAD

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

//functions keypad
void make_Columns_high();
void make_Columns_Input();
void init_keypad_pins();
unsigned int read_keypad();

#endif //KEYPAD