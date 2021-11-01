#include <CrcLib.h>



int HC_SR04(int trigPin, int echoPin){
    CrcLib::Update()

    long sent;
    long got;

    long duration; 
    int distance; 
    
    
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    
    
    while(echoPin == LOW){
       CrcLib::Update();
       
    }
    
    duration = got - sent;

    distance = duration*0.034/2;

    return distance;
}

int wait(int time){
    CrcLib::Update();
    
    int x = time/5;

    for(int i; i <= x; i++){
        CrcLib::Update();
        delay(5);
    }

}
