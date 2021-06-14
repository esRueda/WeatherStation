#ifndef LOGGING
#define LOGGING

#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"

typedef struct day_values{
  double min;
  double max;
  double avg;
  int day;
} DAY;

double calcMin(int* arr, int size);
double calcMax(int* arr, int size);
double calcAvg(int* arr, int size);

DAY* calculateday(int* arr, int size, int d);

#endif //LOGGING