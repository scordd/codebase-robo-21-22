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
    Serial.print(" LeftStick(x,y,press): ("); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X)); Serial.print(","); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y)) ; Serial.print(","); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::HATL));
    Serial.print(") RightStick(x,y,press):("); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X)); Serial.print(","); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y)); Serial.print(","); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::HATR));
    
    Serial.print(") LeftGachette("); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L));
    Serial.print(") L1:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::L1));
    
    Serial.print(" RightGachette("); Serial.print(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R));
    Serial.print(") R1:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::R1));

    Serial.print(" SELECT:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::SELECT));
    Serial.print(" START:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::START));
    Serial.print(" LOGO:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::LOGO));
    
    Serial.print(" ARROW_RIGHT:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT));
    Serial.print(" ARROW_UP:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP));
    Serial.print(" ARROW_LEFT:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT));
    Serial.print(" ARROW_DOWN:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN));

    Serial.print(" COLORS_RIGHT:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT));
    Serial.print(" COLORS_UP:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP));
    Serial.print(" COLORS_LEFT:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT));
    Serial.print(" COLORS_DOWN:"); Serial.print(CrcLib::ReadDigitalChannel(BUTTON::COLORS_DOWN));
    
  }
  else
  {
    Serial.print("No remote controller successfully communicates with the CrcDuino");
  }

}
