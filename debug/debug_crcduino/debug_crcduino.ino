
// Debug code for CrcDuino

#include <CrcLib.h>

using namespace Crc;


void setup() {
    CrcLib::Initialize();

    Serial.begin(9600);
    
}
void loop() {
    CrcLib::Update();

    Serial.print("The code execution time is ");
    Serial.print(CrcLib::GetDeltaTimeMillis());
    Serial.println(" ms.");
    
}
