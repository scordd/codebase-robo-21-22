#include <CrcLib.h>

#define LF CRC_PWM_1
#define RF CRC_PWM_2
#define BL CRC_PWM_3
#define BR CRC_PWM_4
#define ARM1_A CRC_PWM_10
#define ARM1_B CRC_PWM_11
#define ARM2 CRC_PWM_6
#define Servo12inch CRC_PWM_7
#define Servo16inch CRC_PWM_8
#define Servo24inch CRC_PWM_9

using namespace Crc;

int mode = 0;

int vitesse = 0;

int modes[] = {0, 25, 50, 100, 127}; // 4 modes: potentiometer (as 0), low, medium, high, ultra-high

bool armstate = HIGH;

unsigned long x = 0;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);

  CrcLib::InitializePwmOutput(LF);
  CrcLib::InitializePwmOutput(RF);
  CrcLib::InitializePwmOutput(BL);
  CrcLib::InitializePwmOutput(BR);

  CrcLib::InitializePwmOutput(ARM1_A);
  CrcLib::InitializePwmOutput(ARM1_B);
  CrcLib::InitializePwmOutput(ARM2);

  CrcLib::InitializePwmOutput(Servo12inch);
  CrcLib::InitializePwmOutput(Servo16inch);
  CrcLib::InitializePwmOutput(Servo24inch);
}

void loop() {
  CrcLib::Update();
  Serial.println();

  if (CrcLib::IsCommValid()) {
    // Put ALL main code in here. This way, main.ino will only run if CrcConnect is online, connected and functional.

    vitesse = modes[mode];

    int j1xpos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X);
    int j1ypos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y);

    int j2xpos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X);
    int j2ypos = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_Y);

    int l1analog = CrcLib::ReadDigitalChannel(BUTTON::L1);
    int l2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);

    int r1analog = CrcLib::ReadDigitalChannel(BUTTON::R1);
    int r2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);

    int PVC = CrcLib::ReadDigitalChannel(BUTTON::SELECT); // Choosse a button on the remote after more testing 

    int inch12 = CrcLib::ReadDigitalChannel(BUTTON::ARROW_RIGHT);
    int inch16 = CrcLib::ReadDigitalChannel(BUTTON::ARROW_LEFT);
    int inch24 = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);

    CrcLib::Update();

    // END ALL TRANSMISSION FROM JOYSTICKS AND L2 R2 TRIGGERS
    if (j2xpos == 0 && j1ypos == 0 || l2trigger == -128 && r2trigger == -128) {
      done();
    }

    // Switch speed modes (potentiometer, low, medium, high, ultra-high)
    if (l1analog != 0 || r1analog != 0) {
      if (l1analog != 0) {
        mode = mode - 1;
        if (mode < 0) {
        mode = 4;
        }
        vitesse = modes[mode];                          
      } else if (r1analog != 0) {
        mode++;
        if (mode > 4) {
          mode = 0;
        }
        vitesse = modes[mode];
        }
      }

    if (mode == 0) {
      // Tank drive forward-backward rotation (LF, BL, RF, BR) (Jacques)
      if (l2trigger != -128 || r2trigger != -128) {
        if (l2trigger > -110) {
          turnlanalog(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L));
        } else if (r2trigger > -110) {
          turnranalog(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R));
        }
      }

      // Lateral movement/rotation (Thomas)
      if (j1ypos != 0 || j1xpos != 0) {

        if (j1ypos > 20) { 
          forwardanalog(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
        } else if (j1ypos < -20) {
          backwardanalog(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y));
        }
        if (j1xpos < -20) {
          latlanalog(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
        } else if (j1xpos > 20) {
          latranalog(CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X));
        }
      }

    } else {
      // Tank drive forward-backward rotation (LF, BL, RF, BR) (Jacques)
      if (l2trigger != -128 || r2trigger != -128) {
        if (l2trigger < -100) {
          turnl(vitesse);
        } else if (r2trigger < -100) {
          turnr(vitesse);
        }
      }

      // Lateral movement/rotation (Thomas)
      if (j1ypos != 0 || j1xpos != 0) {

        if (j1ypos > 20) {
          forward(vitesse);
        } else if (j1ypos < -20) {
          backward(vitesse);
        }
        if (j1xpos < -20) {
          latl(vitesse);
        } else if (j1xpos > 20) {
          latr(vitesse);
        }
      }

    if (PVC == 1) {
      unsigned long y = millis();
      unsigned long z = y - x;
      done();
      if (z > 500) {
        if (armstate == HIGH) {
          CrcLib::SetPwmOutput(ARM1_A, 127);
          CrcLib::SetPwmOutput(ARM1_B, -127);
          CrcLib::Update();
        } else if (armstate == LOW) {
          CrcLib::SetPwmOutput(ARM1_A, -127);
          CrcLib::SetPwmOutput(ARM1_B, 127);
          CrcLib::Update();
        }
        unsigned long x = millis();
        CrcLib::Update();
      }
    }

    if (inch12 == 1) {
      CrcLib::SetPwmOutput(Servo12inch, 127);
      CrcLib::Update();
      Serial.println("New set servo");
    } else {
      CrcLib::SetPwmOutput(Servo12inch, -127);
      CrcLib::Update();
      Serial.println("No set");
    }

    if (inch16 == 1) {
      CrcLib::SetPwmOutput(Servo16inch, 127);
      CrcLib::Update();
      Serial.println("New set servo");
    } else {
      CrcLib::SetPwmOutput(Servo16inch, -127);
      CrcLib::Update();
      Serial.println("No set");
    }

    if (inch24 == 1) {
      CrcLib::SetPwmOutput(Servo24inch, 127);
      CrcLib::Update();
      Serial.println("New set servo");
    } else {
      CrcLib::SetPwmOutput(Servo24inch, -127);
      CrcLib::Update();
      Serial.println("No set");
    }
      
    } 

  } else {
    Serial.print("No controller connected, file will not work.");
    CrcLib::Update();
  }
}
void turnranalog(int t) {
  CrcLib::SetPwmOutput(LF, t);
  CrcLib::SetPwmOutput(BL, t);
  CrcLib::SetPwmOutput(RF, t);
  CrcLib::SetPwmOutput(BR, t);
  CrcLib::Update();
}

void turnlanalog(int t) {
  CrcLib::SetPwmOutput(LF, -t);
  CrcLib::SetPwmOutput(BL, -t);
  CrcLib::SetPwmOutput(RF, -t);
  CrcLib::SetPwmOutput(BR, -t);
  CrcLib::Update();
}

void forwardanalog(int t) {
  CrcLib::SetPwmOutput(LF, -t);
  CrcLib::SetPwmOutput(BL, -t);
  CrcLib::SetPwmOutput(RF, t);
  CrcLib::SetPwmOutput(BR, t);
  CrcLib::Update();
}

void backwardanalog(int t) {
  CrcLib::SetPwmOutput(LF, t);
  CrcLib::SetPwmOutput(BL, t);
  CrcLib::SetPwmOutput(RF, -t);
  CrcLib::SetPwmOutput(BR, -t);
  CrcLib::Update();
}

void latranalog(int t) {
  CrcLib::SetPwmOutput(LF, t);
  CrcLib::SetPwmOutput(BL, -t);
  CrcLib::SetPwmOutput(RF, -t);
  CrcLib::SetPwmOutput(BR, t);
  CrcLib::Update();
}

void latlanalog(int t) {
  CrcLib::SetPwmOutput(LF, -t);
  CrcLib::SetPwmOutput(BL, t);
  CrcLib::SetPwmOutput(RF, t);
  CrcLib::SetPwmOutput(BR, -t);
  CrcLib::Update();
}

void forward(int vitesse) {
  CrcLib::SetPwmOutput(LF, -vitesse);
  CrcLib::SetPwmOutput(BL, -vitesse);
  CrcLib::SetPwmOutput(RF, vitesse);
  CrcLib::SetPwmOutput(BR, vitesse);
  CrcLib::Update();
}

void backward(int vitesse) {
  CrcLib::SetPwmOutput(LF, vitesse);
  CrcLib::SetPwmOutput(BL, vitesse);
  CrcLib::SetPwmOutput(RF, -vitesse);
  CrcLib::SetPwmOutput(BR, -vitesse);
  CrcLib::Update();
}

void turnr(int vitesse) {
  CrcLib::SetPwmOutput(LF, vitesse);
  CrcLib::SetPwmOutput(BL, vitesse);
  CrcLib::SetPwmOutput(RF, vitesse);
  CrcLib::SetPwmOutput(BR, vitesse);
  CrcLib::Update();
}

void turnl(int vitesse) {
  CrcLib::SetPwmOutput(LF, -vitesse);
  CrcLib::SetPwmOutput(BL, -vitesse);
  CrcLib::SetPwmOutput(RF, -vitesse);
  CrcLib::SetPwmOutput(BR, -vitesse);
  CrcLib::Update();
}

void latr(int vitesse) {
  CrcLib::SetPwmOutput(LF, vitesse);
  CrcLib::SetPwmOutput(BL, -vitesse);
  CrcLib::SetPwmOutput(RF, -vitesse);
  CrcLib::SetPwmOutput(BR, vitesse);
  CrcLib::Update();
}

void latl(int vitesse) {
  CrcLib::SetPwmOutput(LF, -vitesse);
  CrcLib::SetPwmOutput(BL, vitesse);
  CrcLib::SetPwmOutput(RF, vitesse);
  CrcLib::SetPwmOutput(BR, -vitesse);
  CrcLib::Update();
}

void done() {
  CrcLib::SetPwmOutput(LF, 0);
  CrcLib::SetPwmOutput(BL, 0);
  CrcLib::SetPwmOutput(RF, 0);
  CrcLib::SetPwmOutput(BR, 0);
  CrcLib::Update();
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

void waitfast(int a) {
  CrcLib::Update();
  unsigned long x = micros();
  unsigned long y = x + a;

  while (x < y) {
    CrcLib::Update();
    x = micros();
  };

};
// Functions here? Or maybe a library....
