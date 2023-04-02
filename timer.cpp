#include "timer.h"
#include <Arduino.h>

Timer::Timer(){
  this->previousMillis = 0; 
  this->interval = INTERVAL;
}

Timer::Timer(int interval){
  this->interval = interval;
  this->previousMillis = 0; 
}

bool Timer::DoneYet(){
  unsigned long  currentMillis = millis();
  return(currentMillis - this->previousMillis >= this->interval);
}

void Timer::Mark(){
  this->previousMillis = millis();
}
