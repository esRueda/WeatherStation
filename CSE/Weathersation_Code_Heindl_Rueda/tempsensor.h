#ifndef TEMPSENSOR
#define TEMPSENSOR

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

//functions temperature sensor
void init_temp(void);
void measure_begin(void);
float calc_temp(void);

#endif //TEMPSENSOR