#include <CrcLib.h>



int HC_SR04(int trigPin, int echoPin){

    bool status = LOW;
    bool state = LOW;
    bool done = LOW;
    
    unsigned long start;
    unsigned long stop;

    unsigned long duration; 
    int distance; 
    
    digitalWrite(trigPin, LOW);
    waitfast(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    waitfast(10);
    digitalWrite(trigPin, LOW);
    
    
    // Reads the echoPin, returns the sound wave travel time in microseconds
    while(done == LOW){
        
        status = digitalread(echopPin);
        
        if(status == HIGH && state == LOW){
            
            start = micros();
            state = HIGH;
        }
        else if(status == LOW && state == HIGH){
            
            stop = micros();
            status = LOW;
            state = LOW;
            done = LOW;
        } 
    }
    duration = stop - start;

    distance = duration*0.034/2;

    return distance;
}

void wait(int a){
  CrcLib::Update();
  unsigned long x = millis(); 
  unsigned long y = x + a;
        
  while(x < y){
    CrcLib::Update();
    x = millis();
  };
            
};

void waitfast(int a){
  CrcLib::Update();
  unsigned long x = micros(); 
  unsigned long y = x + a;
        
  while(x < y){
    CrcLib::Update();
    x = micros();
  };
            
};
