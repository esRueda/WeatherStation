#ifndef LIGHTSENSOR
#define LIGHTSENSOR

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "servo.h"

void initLightsensor(void);
double measureBrightness(void);
int find_brightestDegree(void);

#endif //LIGHTSENSOR
