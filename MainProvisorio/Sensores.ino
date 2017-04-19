
//____________________________________________________________________________________________________________________________________________________________________________________
// ULTRASSONS
void ReadUS(){
    usfDist=usf.ping_cm();

    uslDistLast=uslDistNow;
    uslDistNow=usl.ping_cm();

    usrDistLast=usrDistNow;
    usrDistNow=usr.ping_cm();

    
    if((lookGapLeft==1)&&(uslDistNow > (uslDistLast + MIN_DIST_GAP))){
      gapCountLeft++;
      gapFound=1;
    }
    if((lookGapRight==1)&&(usrDistNow > (usrDistLast + MIN_DIST_GAP))){
      gapCountRight++;
      gapFound=1;
    }
}
//____________________________________________________________________________________________________________________________________________________________________________________
// GIROSCÓPIO
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
//___________________________________________________________________________________________________________________________________________________________________________________
// CHAMA
void ISeeFire(){
  sFlameLast=sFlameNow;/**/
  sFlameNow=analogRead(FLAME_PIN_AN);
  if((sFlameNow-sFlameLast)>FLAME_PEAK){
    flamePresence=1;
  }/**/
}
//__________________________________________________________________________________________________________________________________________________________________________________
