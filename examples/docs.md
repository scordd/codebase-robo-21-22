This file will be used to document all new components and functions to be used in the robot.

- URM09 Ultrasonic Sensors (https://wiki.dfrobot.com/URM09_Ultrasonic_Sensor_Gravity_Trig_SKU_SEN0388)
Ground, DIO
Todo: convert docs code into CrcLib.

```
#define    VELOCITY_TEMP(temp)       ( ( 331.5 + 0.6 * (float)( temp ) ) * 100 / 1000000.0 ) // The ultrasonic velocity (cm/us) compensated by temperature

int16_t trigechoPin = 11;
uint16_t distance;
uint32_t pulseWidthUs;
void setup() {
  Serial.begin(9600);
  delay(100);
}
void loop() {
  int16_t  dist, temp;
  pinMode(trigechoPin,OUTPUT);
  digitalWrite(trigechoPin,LOW);

  digitalWrite(trigechoPin,HIGH);//Set the trig pin High
  delayMicroseconds(10);     //Delay of 10 microseconds
  digitalWrite(trigechoPin,LOW); //Set the trig pin Low

  pinMode(trigechoPin,INPUT);//Set the pin to input mode
  pulseWidthUs = pulseIn(trigechoPin,HIGH);//Detect the high level time on the echo pin, the output high level time represents the ultrasonic flight time (unit: us)

  distance = pulseWidthUs * VELOCITY_TEMP(20) / 2.0;//The distance can be calculated according to the flight time of ultrasonic wave,/
                                                    //and the ultrasonic sound speed can be compensated according to the actual ambient temperature
  Serial.print(distance, DEC);
  Serial.println("cm");
  delay(500);
}
```