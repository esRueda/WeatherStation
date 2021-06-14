#ifndef MENU
#define MENU

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include "display.h"

int startMenu(void);
int waitForKey(void);
void changeAlarmHigh(void);
void changeAlarmLow(void);
void alarmHigher(void);
void alarmLower(void);

#endif //MENU