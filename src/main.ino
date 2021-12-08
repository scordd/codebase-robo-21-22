#include <CrcLib.h>

using namespace Crc;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);
}


void loop() {
  CrcLib::Update();
  Serial.println();

  if (CrcLib::IsCommValid())
  {
    // Put ALL code in here. This way, main.ino will only run if CrcConnect is online, connected and functional.



  }
  else
  {
    Serial.print("No controller connected, file will not work.");
  }

}

// Functions here? Or maybe a library....
