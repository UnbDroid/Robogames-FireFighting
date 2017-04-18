//Do Fwd:
#define MAL 8   // entrada A da ponte H da roda esquerda
#define MBL 9   // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  10   // entrada de potência do motor esquerdo
#define MAR 11   // entrada A da ponte H da roda direito
#define MBR 12   // entrada B da ponte H da roda direito
#define MOTORR_VEL  13   // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2


int dir;
//-----------------------------------------------------------------------------------------
//Do Interrupt:
#define ENCODER_LEFT 1
#define ENCODER_RIGHT 2

#define WEEL_DIAM 15

int allowEncoder=1;
volatile long int encCountLeft[2], encCountRight[2], coord[2];

//------------------------------------------------------------------------------------------


#include <PID_v1.h>

double powerLeft=100, powerRight=100;
double ctrlPID;
double Klp = 1, Kli = 1, Kld = 1;
double Krp = 1, Kri = 1, Krd = 1;


PID PID_ESQ(&ctrlPID, &powerLeft, 0, Klp, Kli, Kld, REVERSE);
PID PID_DIR(&ctrlPID, &powerRight, 0, Krp, Kri, Krd, DIRECT);

void SetupPID()
{
  PID_ESQ.SetOutputLimits(110, 180);
  PID_DIR.SetOutputLimits(110, 180);
  PID_ESQ.SetSampleTime(100);
  PID_DIR.SetSampleTime(100);
  PID_ESQ.SetMode(AUTOMATIC);
  PID_DIR.SetMode(AUTOMATIC);
}

void UpdatePID()
{
  ctrlPID = abs(encCountLeft[abs(dir)]-encCountRight[abs(dir)]);
  PID_ESQ.Compute();
  PID_DIR.Compute();
}


void FwdPID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, powerRight);
  OnFwd(MOTOR_LEFT, powerLeft);
}

void RevPID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, -powerRight);
  OnFwd(MOTOR_LEFT, -powerLeft);
}

void setup() {
  
  SetupWeels();
  
  encCountLeft[0]=0;
  encCountLeft[1]=0;
  encCountRight[0]=0;
  encCountRight[1]=0;
  coord[0]=0;
  coord[1]=0;

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), AddEncoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), AddEncoderRight, RISING);

  SetupPID();
}

void loop() {
  // put your main code here, to run repeatedly:

}
