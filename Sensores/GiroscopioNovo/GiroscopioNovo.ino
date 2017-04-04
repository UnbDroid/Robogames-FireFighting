#include <Wire.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0X25

int AngSpeedZ, gyroTime, gyroAddress = 105;
unsigned long tGyroNow=0, tGyroAnt;
long int yaw=0;
float gyroFilterB=0, gyroFilterA = 0;

void setup() {
  Serial.begin(9600);
  //Serial.println(1);
  SetupGyroscope(2000);
  Serial.println(2);
  AttGyroFilter(1000);
  Serial.println(3);
}

void loop() {
  /*GetGyroValue();
  Serial.println(AngSpeedZ)*/;
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  Serial.println(GetGyro());/**/
}
