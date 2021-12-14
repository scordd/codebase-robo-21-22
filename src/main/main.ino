#include <CrcLib.h>

#define LF CRC_PWM_1
#define RF CRC_PWM_2
#define BL CRC_PWM_3
#define BR CRC_PWM_4
#define ARM1 CRC_PWM_5
#define ARM2 CRC_PWM_6

using namespace Crc;

int mode = 1;

void setup() {
  CrcLib::Initialize();
  Serial.begin(19200);

  CrcLib::InitializePwmOutput(LF);
  CrcLib::InitializePwmOutput(RF);
  CrcLib::InitializePwmOutput(BL);
  CrcLib::InitializePwmOutput(BR);
  CrcLib::InitializePwmOutput(ARM1);
  CrcLib::InitializePwmOutput(ARM2);
}


void loop() {
  CrcLib::Update();
  Serial.println();

  if (CrcLib::IsCommValid())
  {
    // Put ALL main code in here. This way, main.ino will only run if CrcConnect is online, connected and functional.

    if (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L) == 1) {
          mode = (mode - 1);
      }
    
    if (CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R) == 1) {
          mode++;
      }





  }
  else {
    Serial.print("No controller connected, file will not work.");
    CrcLib::Update();
  }
}

// Functions here? Or maybe a library....
