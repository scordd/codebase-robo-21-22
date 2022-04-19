#include <CrcLib.h>

using namespace Crc;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);

  CrcLib::SetDigitalPinMode(CRC_DIG_1, OUTPUT);
  CrcLib::SetDigitalPinMode(CRC_DIG_2, OUTPUT);
  CrcLib::SetDigitalPinMode(CRC_DIG_3, OUTPUT);
  CrcLib::SetDigitalPinMode(CRC_DIG_4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  CrcLib::Update();

  CrcLib::SetDigitalOutput(CRC_DIG_1, LOW);
  CrcLib::SetDigitalOutput(CRC_DIG_2, HIGH);

}

void wait(int a) {
  CrcLib::Update();
  unsigned long x = millis();
  unsigned long y = x + a;

  while (x < y) {
    CrcLib::Update();
    x = millis();
  };

};
