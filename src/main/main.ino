#include <CrcLib.h>

#define LF CRC_PWM_1
#define RF CRC_PWM_2
#define BL CRC_PWM_3
#define BR CRC_PWM_4
#define ARM CRC_PWM_5
#define TOPHATCH CRC_DIG_11
#define BOTHATCH CRC_DIG_12
#define IN1 CRC_DIG_1
#define IN2 CRC_DIG_2
#define IN3 CRC_DIG_3
#define IN4 CRC_DIG_4

using namespace Crc;

int mode = 0;

int vitesse = 0;

int modes[] = {0, 25, 70, 127}; // 4 modes: potentiometer (as 0), low, medium, high, ultra-high

int ballstatus = 0;

bool armstate = HIGH;

unsigned long x = 0;

void setup() {
  CrcLib::Initialize();
  Serial.begin(9600);

  CrcLib::InitializePwmOutput(LF);
  CrcLib::InitializePwmOutput(RF);
  CrcLib::InitializePwmOutput(BL);
  CrcLib::InitializePwmOutput(BR);

  CrcLib::InitializePwmOutput(ARM);

  CrcLib::SetDigitalPinMode(TOPHATCH, OUTPUT);
  CrcLib::SetDigitalPinMode(BOTHATCH, OUTPUT);
  CrcLib::SetDigitalPinMode(IN1, OUTPUT);
  CrcLib::SetDigitalPinMode(IN2, OUTPUT);
  CrcLib::SetDigitalPinMode(IN3, OUTPUT);
  CrcLib::SetDigitalPinMode(IN4, OUTPUT);

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

    int select = CrcLib::ReadDigitalChannel(BUTTON::SELECT);
    int start = CrcLib::ReadDigitalChannel(BUTTON::START);
    
    int l2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);
    int r2trigger = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);

    int individual = CrcLib::ReadDigitalChannel(BUTTON::ARROW_UP);
    int full = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
    
    int backwardArm = CrcLib::ReadDigitalChannel(BUTTON::L1);
    int forwardArm = CrcLib::ReadDigitalChannel(BUTTON::R1);

    
    
    CrcLib::Update();

    // END ALL TRANSMISSION FROM JOYSTICKS AND L2 R2 TRIGGERS
    if (j2xpos == 0 && j1ypos == 0 || l2trigger == -128 && r2trigger == -128) {
      done();
    }

    // Switch speed modes (potentiometer, low, medium, high)
    if (select != 0 || start != 0) {
        if (select != 0) {
        mode = mode - 1;
        if (mode < 0) {
          mode = 3;
        }
        vitesse = modes[mode];
        wait(500);
        
      } else if(start != 0){
        mode++;
        if (mode > 3) {
          mode = 0;
        }
        vitesse = modes[mode];
        wait(500);
        } 
      }

 if (mode == 0) {

      // Tank drive forward-backward rotation (LF, BL, RF, BR) (Jacques)
      if (l2trigger != -128 || r2trigger != -128) {
        if (r2trigger < -100) {
          turnranalog(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R));
        } else if (l2trigger < -100) {
          turnlanalog(CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L));
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
        if (r2trigger < -100) {
          turnr(vitesse);
        } else if (l2trigger < -100) {
          turnl(vitesse);
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
          latl(50);
        } else if (j1xpos > 20) {
          latr(50);
        }
      }
 }

    // new arm code (still needs to be revised by another person + tested)
    if (backwardArm == true || forwardArm == true) {
      if (backwardArm == true) {
        CrcLib::SetPwmOutput(ARM, 20);
        wait(700);
        CrcLib::SetPwmOutput(ARM, 0);
        }
      if (forwardArm == true) {
        CrcLib::SetPwmOutput(ARM, -80);
        wait(500);
        CrcLib::SetPwmOutput(ARM, -10);
        }
        CrcLib::Update();
      }

    if(individual == HIGH){
      botopen();
      wait(1600);
      botstop();
      wait(5000);
      botclose();
      wait(2500);
      botstop();
      ballstatus = 0;
      CrcLib::Update();
    }
    
    if(full == HIGH){
      topopen();
      botopen();
      wait(1600);
      while(full == HIGH){
        topstop();
        botstop();
        CrcLib::Update();
        full = CrcLib::ReadDigitalChannel(BUTTON::ARROW_DOWN);
        CrcLib::Update();
      }
      topclose();
      wait(1000);
      botclose();
      wait(1500);
      topstop();
      wait(1000);
      botstop();
      ballstatus = 0;
    }
    
    // dispenser code (needs to be tested)
    if (ballstatus == 0){
      topopen();
      wait(1600);
      topstop();
      wait(2000);
      topclose();
      wait(2500);
      topstop();
      CrcLib::Update();
      ballstatus = 1;
    }
    


      CrcLib::Update();
      Serial.println("New set servo");

      
    done();
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
  CrcLib::SetPwmOutput(LF, t);
  CrcLib::SetPwmOutput(BL, t);
  CrcLib::SetPwmOutput(RF, t);
  CrcLib::SetPwmOutput(BR, t);
  CrcLib::Update();
}

void backwardanalog(int t) {
  CrcLib::SetPwmOutput(LF, -t);
  CrcLib::SetPwmOutput(BL, -t);
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
  CrcLib::SetPwmOutput(LF, vitesse);
  CrcLib::SetPwmOutput(BL, vitesse);
  CrcLib::SetPwmOutput(RF, vitesse);
  CrcLib::SetPwmOutput(BR, vitesse);
  CrcLib::Update();
}

void backward(int vitesse) {
  CrcLib::SetPwmOutput(LF, -vitesse);
  CrcLib::SetPwmOutput(BL, -vitesse);
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


void topopen(){
  CrcLib::SetDigitalOutput(TOPHATCH, HIGH);
  CrcLib::SetDigitalOutput(IN1, LOW);
  CrcLib::SetDigitalOutput(IN2, HIGH);
  CrcLib::Update();
  }

void topclose() {
  CrcLib::SetDigitalOutput(TOPHATCH, HIGH);
  CrcLib::SetDigitalOutput(IN1, HIGH);
  CrcLib::SetDigitalOutput(IN2, LOW);
  CrcLib::Update();
  }

void topstop(){
  CrcLib::SetDigitalOutput(TOPHATCH, LOW);
  CrcLib::Update();
  }
  
void botopen() {
  CrcLib::SetDigitalOutput(BOTHATCH, HIGH);
  CrcLib::SetDigitalOutput(IN3, LOW);
  CrcLib::SetDigitalOutput(IN4, HIGH);
  CrcLib::Update();
  }

void botclose() {
  CrcLib::SetDigitalOutput(BOTHATCH, HIGH);
  CrcLib::SetDigitalOutput(IN3, HIGH);
  CrcLib::SetDigitalOutput(IN4, LOW);
  CrcLib::Update();
  }

void botstop(){
  CrcLib::SetDigitalOutput(BOTHATCH, LOW);
  CrcLib::Update();
  }
