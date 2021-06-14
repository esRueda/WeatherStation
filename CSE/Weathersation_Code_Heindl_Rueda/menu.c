#include "menu.h"

extern int alarm;
extern double low_alarm;
extern double high_alarm;
extern int lastState;
extern int enableTempUpdate;
extern int fast;

int startMenu(void){
  enableTempUpdate=0;  
  if(lastState==0){     // display shows "Press # to start!"
    clearDisplayLine(20,1);
  }
  else if(lastState==1){
    clearMenu();        // clears the menu / homescreen (same function)
  }
  else{  
    clearDisplay(500);  // clearing whole display (500 pixel)
  }
  
  //print the menu
  changeLine(1);
  char sun[] = {'6',':',' ','L','o','c','a','t','e',' ','s','u','n'};
  printString2display(sun, sizeof(sun));

  changeLine(3);
  char logged[] = {'2',':',' ','V','i','e','w',' ','l','o','g','g','e','d',' ','d','a','t','a'};
  printString2display(logged, sizeof(logged));
  
  changeLine(5);
  if(!fast){ char fastmode[] = {'3',':',' ','F','a','s','t',' ','m','o','d','e'};
            printString2display(fastmode, sizeof(fastmode));}
  else{ char fastmode[] = {'3',':',' ','S','l','o','w',' ','m','o','d','e'};
        printString2display(fastmode, sizeof(fastmode));}

  changeLine(7);
  char AlarmHigh[] = {'4',':',' ','S','e','t',' ','u','p','p','e','r',' ','A','l','a','r',',m',' ','V','a','l','u','e'};
  printString2display(AlarmHigh, sizeof(AlarmHigh));
  
  changeLine(9);
  char AlarmLow[] = {'5',':',' ','S','e','t',' ','l','o','w','e','r',' ','A','l','a','r',',m',' ','V','a','l','u','e'};
  printString2display(AlarmLow, sizeof(AlarmLow));

  lastState=1; 
  
  int key = waitForKey();
  
  switch(key){
    case 6 : return 6; break; //1
    case 2 : return 2; break; //2
    case 3 : changeFastMode(); break; //3
    case 4 : alarmHigher(); break; //4
    case 5 : alarmLower(); break; //5
    case 10 : return 0; break; //*
  }
  return 0;
}

int waitForKey(void){
  int key = read_keypad();
  
  while(((key < 2) || (key > 6)) && (key != 10)){
    key = read_keypad();
    temp_measuring();
  }
  return key;
}

void alarmHigher(void){
  clearMenu();
  
  char alarm_high_print[20];
  changeLine(1);
  snprintf(alarm_high_print ,20, "High alarm: %.2f", high_alarm);
  printString2display(alarm_high_print, sizeof(alarm_high_print));
  
  changeLine(5);
  char temp[] = "4 to decrease by 1 degree";
  printString2display(temp, sizeof(temp));
  
  changeLine(7);
  char temp2[] = "6 to increase by 1 degree";
  printString2display(temp2, sizeof(temp2));
  
  changeLine(14);
  char exit[] = "Press * to exit";
  printString2display(exit, sizeof(exit));
   
  int key_old = 0;
  int key = 0;
  int errorActive=0;
  
  do{
    key = read_keypad();
    temp_measuring();
          
    if(key != key_old){  // no preesing // you have to release the button
      if(key == 4){ // decreasing
        if(high_alarm > (low_alarm+1.0)){ // low and high can not be the same 
          high_alarm -= 1.0;

          changeLine(1);
          snprintf(alarm_high_print ,20, "High alarm: %.2f", high_alarm);
          printString2display(alarm_high_print, sizeof(alarm_high_print));

          if(errorActive){ // delete error message
            changeLine(9);
            char clear_errormsg_l1[] = "                      ";
            printString2display(clear_errormsg_l1, sizeof(clear_errormsg_l1));
            changeLine(10);
            char clear_errormsg_l2[] = "                      ";
            printString2display(clear_errormsg_l2, sizeof(clear_errormsg_l2));
            errorActive=0;
          }
        }
        else{ //error // low and high are the same
          changeLine(9);
          char errormsg_l1[] = "high alarm cannot be";
          printString2display(errormsg_l1, sizeof(errormsg_l1));
          changeLine(10);
          char errormsg_l2[] = "lower than low alarm";
          printString2display(errormsg_l2, sizeof(errormsg_l2));
          errorActive=1;
        }
      }
      else if(key == 6){ //increasing
        high_alarm += 1.0; 

        changeLine(1);
        snprintf(alarm_high_print ,20, "High alarm: %.2f", high_alarm);
        printString2display(alarm_high_print, sizeof(alarm_high_print));
        
        if(errorActive){ // delete error message
          changeLine(9);
          char clear_errormsg_l1[] = "                      ";
          printString2display(clear_errormsg_l1, sizeof(clear_errormsg_l1));
          changeLine(10);
          char clear_errormsg_l2[] = "                       ";
          printString2display(clear_errormsg_l2, sizeof(clear_errormsg_l2));
          errorActive=0;
        }
      }
    }
    key_old = key;
  }while(key != 10);
  
    clearAlarmMenu();
    changeLine(1);
    char start[] = "Press # to start!";
    printString2display(start, sizeof(start));  
    lastState=0;// for deleting in startMenu() // only deleting line 1
}

void alarmLower(void){
  clearMenu();
  
  char alarm_low_print[20];
  changeLine(1);
  snprintf(alarm_low_print ,20, "Low alarm: %.2f", low_alarm);
  printString2display(alarm_low_print, sizeof(alarm_low_print));
  
  changeLine(5);
  char temp[] = "4 to decrease by 1 degree";
  printString2display(temp, sizeof(temp));
  
  changeLine(7);
  char temp2[] = "6 to increase by 1 degree";
  printString2display(temp2, sizeof(temp2));
  
  changeLine(14);
  char exit[] = "Press * to exit";
  printString2display(exit, sizeof(exit));
  
  int key_old = 0;
  int key = 0;
  int errorActive=0;
  do{
    key = read_keypad();
    temp_measuring();
    
    if(key != key_old){
      if(key == 6){
        if(low_alarm < (high_alarm-1.0)){
          low_alarm += 1.0;

          changeLine(1);
          snprintf(alarm_low_print ,20, "Low alarm: %.2f", low_alarm);
          printString2display(alarm_low_print, sizeof(alarm_low_print));
          changeLine(9);
          if(errorActive){
            changeLine(9);
            char clear_errormsg_l1[] = "                      ";
            printString2display(clear_errormsg_l1, sizeof(clear_errormsg_l1));
            changeLine(10);
            char clear_errormsg_l2[] = "                      ";
            printString2display(clear_errormsg_l2, sizeof(clear_errormsg_l2));
            errorActive=0;
          }
        }
        else{
          changeLine(9);
          char errormsg_l1[] = "low alarm cannot be";
          printString2display(errormsg_l1, sizeof(errormsg_l1));
          changeLine(10);
          char errormsg_l2[] = "higher than high alarm";
          printString2display(errormsg_l2, sizeof(errormsg_l2));
          errorActive=1;
        }  
      }
      else if(key == 4){
        low_alarm -= 1.0;
        
        changeLine(1);
        snprintf(alarm_low_print ,20, "Low alarm: %.2f", low_alarm);
        printString2display(alarm_low_print, sizeof(alarm_low_print));
        
          if(errorActive){
            changeLine(9);
            char clear_errormsg_l1[] = "                      ";
            printString2display(clear_errormsg_l1, sizeof(clear_errormsg_l1));
            changeLine(10);
            char clear_errormsg_l2[] = "                      ";
            printString2display(clear_errormsg_l2, sizeof(clear_errormsg_l2));
            errorActive=0;
          }
      }
      key_old = key;
    }
  }while(key != 10);
  
    clearAlarmMenu();
    changeLine(1);
    char start[] = "Press # to start!";
    printString2display(start, sizeof(start));
    lastState=0;// for deleting in startMenu() // only deleting line 1
}