#define MAL 7   // entrada A da ponte H da roda esquerda
#define MBL 8   // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  9   // entrada de potência do motor esquerdo
#define MAR 10   // entrada A da ponte H da roda direito
#define MBR 11   // entrada B da ponte H da roda direito
#define MOTORR_VEL  12   // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2


int dir;

void setup() {
  SetupWeels();

}

void loop() {
  OnFwd(MOTOR_LEFT, 50);
  delay(1500);
  OnFwd(MOTOR_LEFT, 100);
  delay(1500);
  OnFwd(MOTOR_LEFT, 0);
  delay(1500);
  OnFwd(MOTOR_LEFT, -100);
  delay(1500);
  OnFwd(MOTOR_LEFT, -50);
  delay(1500);
}
