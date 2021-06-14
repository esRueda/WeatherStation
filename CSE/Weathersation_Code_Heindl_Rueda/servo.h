#ifndef SERVO
#define SERVO

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

void servo_init(void);
int move_to_position(int pos);

#endif //SERVO