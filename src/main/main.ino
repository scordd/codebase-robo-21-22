#include <CrcLib.h>

#define LF CRC_PWM_1
#define RF CRC_PWM_2
#define BL CRC_PWM_3
#define BR CRC_PWM_4
#define ARM1 CRC_PWM_5
#define ARM2 CRC_PWM_6
#define Servo12inch CRC_PWM_7
#define Servo18inch CRC_PWM_8
#define Servo24inch CRC_PWM_9

using namespace Crc;

int mode = 1; // Focus on getting the robot to move for now, then we'll add modes.

void setup()
{
  CrcLib::Initialize();
  Serial.begin(19200);

  CrcLib::InitializePwmOutput(LF);
  CrcLib::InitializePwmOutput(RF);
  CrcLib::InitializePwmOutput(BL);
  CrcLib::InitializePwmOutput(BR);
  
  CrcLib::InitializePwmOutput(ARM1);
  CrcLib::InitializePwmOutput(ARM2);
  
  CrcLib::InitializePwmOutput(Servo12inch);
  CrcLib::InitializePwmOutput(Servo18inch);
  CrcLib::InitializePwmOutput(Servo24inch);
}

void loop()
{
  CrcLib::Update();
  Serial.println();

  if (CrcLib::IsCommValid())
  {
    // Put ALL main code in here. This way, main.ino will only run if CrcConnect is online, connected and functional.
    int j1xpos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    int j1ypos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);

    int j2xpos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    int j2ypos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);

    int l1analog = CrcLib::ReadDigitalChannel(BUTTON::L1);
    int l2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);

    int r1analog = CrcLib::ReadDigitalChannel(BUTTON::R1);
    int r2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);

    int right = CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT);
    int left = CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT);
    
    CrcLib::Update();
    
    if (j2xpos == 0 && j1ypos == 0 && l2trigger == 0 && r2trigger == 0){
      done();  
    }
    
    if (l2trigger != 0 || r2trigger != 0){
      if (l2trigger < 50) {
        turnl(100);
      }
      else if (r2trigger < 50) {
        turnr(100);
      }
    }
    
    if (j1ypos != 0 || j1xpos != 0){
      if (j1ypos < -126){
         forward(100);
      }
      else if(j1ypos > 126){
         backward(100);
      }
//       if (j1xpos < -126){
//          latl(100);
//       }
//       else if(j1xpos > 126){
//          latr(100);
//       }
    }



    
  }
  else
  {
    Serial.print("No controller connected, file will not work.");
    CrcLib::Update();
  }
}
  
  
void forward(int speed){
       CrcLib::SetPwmOutput(LF, speed);
       CrcLib::SetPwmOutput(BL, speed);
       CrcLib::SetPwmOutput(RF, -speed);
       CrcLib::SetPwmOutput(BR, -speed);
       CrcLib::Update();
}

void backward(int speed){
       CrcLib::SetPwmOutput(LF, -speed);
       CrcLib::SetPwmOutput(BL, -speed);
       CrcLib::SetPwmOutput(RF, speed);
       CrcLib::SetPwmOutput(BR, speed);
       CrcLib::Update();
}  

void turnr(int speed){
       CrcLib::SetPwmOutput(LF, speed);
       CrcLib::SetPwmOutput(BL, speed);
       CrcLib::SetPwmOutput(RF, speed);
       CrcLib::SetPwmOutput(BR, speed);
       CrcLib::Update();
}
  
void turnl(int speed){
       CrcLib::SetPwmOutput(LF, -speed);
       CrcLib::SetPwmOutput(BL, -speed);
       CrcLib::SetPwmOutput(RF, -speed);
       CrcLib::SetPwmOutput(BR, -speed);
       CrcLib::Update();
}

void latr(int speed){
       CrcLib::SetPwmOutput(LF, speed);
       CrcLib::SetPwmOutput(BL, -speed);
       CrcLib::SetPwmOutput(RF, -speed);
       CrcLib::SetPwmOutput(BR, speed);
       CrcLib::Update();
}

void latl(int speed){
       CrcLib::SetPwmOutput(LF, -speed);
       CrcLib::SetPwmOutput(BL, speed);
       CrcLib::SetPwmOutput(RF, speed);
       CrcLib::SetPwmOutput(BR, -speed);
       CrcLib::Update();
}

void done(){
       CrcLib::SetPwmOutput(LF, 0);
       CrcLib::SetPwmOutput(BL, 0);
       CrcLib::SetPwmOutput(RF, 0);
       CrcLib::SetPwmOutput(BR, 0);
       CrcLib::Update();
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
// Functions here? Or maybe a library....
