#include <Wire.h>

#define MOT_ESQ 3
#define MOT_DIR 11

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // Configura o L3G4200 para 200, 500 ou 2000 graus/seg
  setupL3G4200D(2000);
  //Setup_PID(); 
  // Aguarda a resposta do sensor
  delay(1000); 
}
void loop()
{
  int start = millis();

  while(millis() - start < 50)
  {
    update_gyro();
  }
  Serial.println(get_gyro());
}

