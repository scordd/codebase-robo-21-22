#ifndef Standard_h
#define Standard_h

#include <Arduino.h>

class Standard
{
    public:
     Standard(int a, unsigned long x, unsigned long y);
     void wait();
     void waitfast();
    private:
     int a;
     unsigned long x;
     unsigned long y;
}

#endif