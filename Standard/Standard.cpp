#include <Arduino.h>
#include <Standard.h>

void Standard::wait(int a){
  CrcLib::Update();
  unsigned long x = millis(); 
  unsigned long y = x + a;
        
  while(x < y){
    CrcLib::Update();
    x = millis();
  };
            
};

void Standard::waitfast(int a){
  CrcLib::Update();
  unsigned long x = micros(); 
  unsigned long y = x + a;
        
  while(x < y){
    CrcLib::Update();
    x = micros();
  };
            
};
