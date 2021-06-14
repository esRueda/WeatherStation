#ifndef PIN_INIT
#define PIN_INIT

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

#define OUTPUT 0
#define INPUT 1

void pin_init(void);
void set_data_bus_direction(int dir);

#endif 