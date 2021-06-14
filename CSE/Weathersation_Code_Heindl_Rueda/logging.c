#include "logging.h"

double calcMin(int* arr, int size){
  int i = 0;
  double min = arr[0];
  
  while(i!= size){
    if(arr[i] < min){
      min = arr[i];
    }
    i++;
  }
  return min;
}

double calcMax(int* arr, int size){
  int i = 0;
  double max = arr[0];
  
  while(i!= size){
    if(arr[i] > max){
      max = arr[i];
    }
    i++;
  }
  return max;
}

double calcAvg(int* arr, int size){
  double sum = 0.0;
  double avg = 0.0;
  int i = 0;
  
  while(i != size){
    sum += arr[i];
    i++;
  }
  avg = sum/size;
  
  return avg;
}

DAY* calculateday(int* arr, int size, int d){
  DAY* newday = (DAY*)malloc(sizeof(DAY));
  
  newday->min = calcMin(arr, size);
  newday->max = calcMax(arr, size);
  newday->avg = calcAvg(arr, size);
  newday->day = d;
  
  return newday;
}
