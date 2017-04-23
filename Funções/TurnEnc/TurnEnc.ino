//Do Fwd:
#define MAL 8   // entrada A da ponte H da roda esquerda
#define MBL 9   // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  10   // entrada de potência do motor esquerdo
#define MAR 11   // entrada A da ponte H da roda direito
#define MBR 12   // entrada B da ponte H da roda direito
#define MOTORR_VEL  13   // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2
#define ANALISE_STATE 1
#define TURN 2 

int dir=1;
//-----------------------------------------------------------------------------------------
//Do Interrupt:
#define ENCODER_LEFT 1
#define ENCODER_RIGHT 2

#define WEEL_DIAM 15

int allowEncoder=1;
volatile long int encCountLeft[2], encCountRight[2], coord[2];

//------------------------------------------------------------------------------------------


#include <PID_v1.h>

int DegreesToTurn;
int Busy;
int Last_Enc[2]; ////0 esquerda 1 direita
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

void setup() {
  
  SetupWeels();
  
  SetupInterrupt();

  SetupPID();
  Serial.begin(9600);
}
void Heart()
{
  switch (Busy){
    case ANALISE_STATE:
          delay(5000);
          break;
    case TURN:
          TurnEnc();
   }
}
void AnaliseState()
{
  delay(5000); //ATENCAO !!! ELE SO VAI COMECAR A GIRAR 5 SEGN+GUNDOS DPS DO INICIO DO CODIGO, E VAI CONTINUAR GIRANDO E ESPERANDO 5s .
  DegreesToTurn = 90;
  Last_Enc[0] = encCountLeft[0];
  Last_Enc[1] = encCountRight[0];
  Busy = TURN;
}

void loop() {
  Serial.println("comeeceiiiiie");
  Heart();  
}
