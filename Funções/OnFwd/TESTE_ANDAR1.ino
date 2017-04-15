#define MAL 8             // entrada A da ponte H da roda esquerda
#define MBL 9             // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  10    // entrada de potência do motor esquerdo
#define MAR 11            // entrada A da ponte H da roda direito
#define MBR 12            // entrada B da ponte H da roda direito
#define MOTORR_VEL  13    // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2

void SetupWeels(){
  pinMode(MAL, OUTPUT);
  pinMode(MBL, OUTPUT);
  pinMode(MOTORL_VEL, OUTPUT);
  pinMode(MAR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MOTORR_VEL, OUTPUT);
}
void setup() {
  SetupWeels();
  Serial.begin(9600);
  
}
void TravarMot()
{
    digitalWrite(MAL, 1);
    digitalWrite(MBL, 1);
    digitalWrite(MAR, 1);
    digitalWrite(MBR, 1); 
}
int vel = 100;
void loop() {
    TravarMot();
    delay(2000);
    analogWrite(MOTORL_VEL, vel);
    analogWrite(MOTORR_VEL, vel);
    digitalWrite(MAL, 0);
    digitalWrite(MBL, 1);
    digitalWrite(MAR, 0);
    digitalWrite(MBR, 1);
    delay(3000);

}
