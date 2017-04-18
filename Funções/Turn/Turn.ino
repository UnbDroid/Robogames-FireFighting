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


#define MAL 8   // entrada A da ponte H da roda esquerda
#define MBL 9   // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  10   // entrada de potência do motor esquerdo
#define MAR 11   // entrada A da ponte H da roda direito
#define MBR 12   // entrada B da ponte H da roda direito
#define MOTORR_VEL  13   // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2


int dir;


void AttGyroFilter(int imax){
  int i, count1=0, count2=0;
  for(i=0;i<imax;i++){
    //Serial.println(4);
    GetGyroValue();
    //Serial.println(5);
    count1 += AngSpeedZ;
  }
  gyroFilterB = count1/imax;

  count1 = 0;
  GetGyroValue();
  count1 += AngSpeedZ;
  GetGyroValue();
  count1 += AngSpeedZ;
  GetGyroValue();
  count1 += AngSpeedZ;
  count1 = count1/3;
  delay(489);
  GetGyroValue();
  count2 += AngSpeedZ;
  GetGyroValue();
  count2 += AngSpeedZ;
  GetGyroValue();
  count2 += AngSpeedZ;
  count2 = count2/3;
  gyroFilterA = (count2 - count1)/500;/**/
}

int GetGyro(){
  return yaw/14640;
}

void UpdateGyro(){
  tGyroAnt = tGyroNow;
  tGyroNow = millis();
  // Atualiza os valores de X, Y e Z
  GetGyroValue();  
  // Mostra os valores no serial monitor
  AngSpeedZ -= (gyroFilterA*tGyroNow + gyroFilterB);
  gyroTime = tGyroNow - tGyroAnt;
  yaw += gyroTime*AngSpeedZ;
}

void GetGyroValue()
{
  // Rotina para leitura dos valores de velocidade angular em torno de z
    //a leitura do giroscópio é dada em dois bits, nos endereços apresentados abaixo
  //Serial.println(3);
  byte zMSB = ReadRegister(105, 0x2D);  //zMSB == z Most Significant Byte
  //Serial.println(4);
  byte zLSB = ReadRegister(105, 0x2C);  //zLSB == z Least Significant Byte
  //Serial.println(5);
  AngSpeedZ = ((zMSB << 8) | zLSB);     //junção do bit menos significativo com o mais significativo, para dar o valor inteiro referente à velocidade angular em z
  //Serial.println(6);
}


int SetupGyroscope(int scale)
{
  //From  Jim Lindblom of Sparkfun's code
  // Enable x and turn off power down:
  WriteRegister(gyroAddress, CTRL_REG1, 0b00001111);        //desse jeito só o giroscópio só pega leitura em z. Para pegar leitura em x e y, trocar para 1 o antepenúltimo e penúltimo bit, respectivamente
  // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
  WriteRegister(gyroAddress, CTRL_REG2, 0b00000000);
  // Configure CTRL_REG3 to generate data ready interrupt on INT2
  // No interrupts used on INT1, if you'd like to configure INT1
  // or INT2 otherwise, consult the datasheet:
  WriteRegister(gyroAddress, CTRL_REG3, 0b00001000);
  // CTRL_REG4 controls the full-scale range, among other things:
  if(scale == 250){
    WriteRegister(gyroAddress, CTRL_REG4, 0b00000000);
  }else if(scale == 500){
    WriteRegister(gyroAddress, CTRL_REG4, 0b00010000);
  }else{
    WriteRegister(gyroAddress, CTRL_REG4, 0b00110000);
  }
  // CTRL_REG5 controls high-pass filtering of outputs, use it
  // if you'd like:
  WriteRegister(gyroAddress, CTRL_REG5, 0b00000000);
  //WriteRegister(gyroAddress, REFERENCE, 0b00000000);
}


void WriteRegister(int deviceAddress, byte address, byte val) 
{
    Wire.beginTransmission(deviceAddress); // start transmission to device 
    Wire.write(address);       // send register address
    Wire.write(val);         // send value to write
    Wire.endTransmission();     // end transmission
}
int ReadRegister(int deviceAddress, byte address)
{
    //Serial.println(10);
    int value, out=0;
    Wire.beginTransmission(deviceAddress);
    //Serial.println(11);
    Wire.write(address); // register to read
    //Serial.println(12);
    Wire.endTransmission();
    //Serial.println(13);
    Wire.requestFrom(deviceAddress, 1); // read a byte
    //Serial.println(14);
    //pega tempo 
    while((!Wire.available())&&(out<200)) 
    
    { 
      out++;
      Serial.println("nao conectou");
        // waiting
    }
    if(out<200){
       
       value = Wire.read();
    }
    else{
      //tempo de agr
       ///variacao -> quanto tempo ficou perdido-> uma relacao (media) com o giro -> soma com o valor do ultimo angulo
      value = ReadRegister(deviceAddress, address);
    }
    return value;
}


void OnFwd(int motor, int power){
  if(motor==MOTOR_LEFT){
    int vel;
    digitalWrite(MAL, (1+(power/abs(power)))/2);
    digitalWrite(MBL, (1-(power/abs(power)))/2);
    Serial.print((1+(power/abs(power)))/2);
    Serial.print("        ");
    Serial.println((1-(power/abs(power)))/2);
    vel=abs(power);
    analogWrite(MOTORL_VEL, vel);
    Serial.println(abs(power));
  }
  else{
    if(motor==MOTOR_RIGHT){
      digitalWrite(MAR, (1+(power/abs(power)))/2);
      digitalWrite(MBR, (1-(power/abs(power)))/2);
      analogWrite(MOTORR_VEL, abs(power));
      dir = dir*(power/abs(power));
    }
    else{
      //manda mensagem de erro
    }
  }
}

void SetupWeels(){
  pinMode(MAL, OUTPUT);
  pinMode(MBL, OUTPUT);
  pinMode(MOTORL_VEL, OUTPUT);
  pinMode(MAR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MOTORR_VEL, OUTPUT);
}


int turnStart=1;

int cont=0;

void Turn(int degree){
  if(turnStart){
    OnFwd(MOTOR_RIGHT, 0);
    OnFwd(MOTOR_LEFT, 0);
    yaw=0;
    AttGyroFilter(1000);
    turnStart=0;
  }
  UpdateGyro();
  UpdateGyro();
  UpdateGyro();
  if(abs(GetGyro())<abs(degree)){
    OnFwd(MOTOR_RIGHT, (degree/abs(degree))*100);
    OnFwd(MOTOR_LEFT, -(degree/abs(degree))*100);
  }
  else{
    OnFwd(MOTOR_RIGHT, 0);
    OnFwd(MOTOR_LEFT, 0);
    //busy = STATE_ANALISER
    turnStart=1;
  }
}     //mudar o dir logo depois de chamar essa função




void setup() {
  SetupWeels();
  SetupGyroscope(2000);

}

void loop() {
  /*if(cont%2==0){
    Turn(90);
  }
  else{
    Turn(-90);
  }*/
  /*if((cont/1000)%2==0){
    OnFwd(MOTOR_RIGHT, 90);
    OnFwd(MOTOR_LEFT, 90);
  }
  else{
    OnFwd(MOTOR_RIGHT, -90);
    OnFwd(MOTOR_LEFT, -90);
  }
  cont++;*/
  Turn(90);
  delay(500);
  Turn(-180);
  delay(500);
}
