// Funções do giroscópio

/*------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Funções para inicializar o giroscópio e para a comunicação com ele*/

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

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Filtro do giroscópio*/

void AttGyroFilter(int imax){
  //O erro é dado por uma função linear (y=ax+b, com x sendo o tempo decorrido), o filtro abaixo encontra essa função, para então aplicá-la
  int i, count1=0, count2=0;
  for(i=0;i<imax;i++){
    GetGyroValue();
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
  gyroFilterA = (count2 - count1)/500;    //a princípio dividiria por 489, mas as funções chamadas gastam também certo tempo (não sei quanto especificamente), então esse tempo é levado a mais em consideração. Não foi feito nenhum cálculo, o valor 489 -> 500 foi apenas o que deu melhor resultado
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Pegando e transformando os valores do giroscópio*/

int GetGyro(){
  return yaw/14640;
}

void UpdateGyro(){
  //função que pega os valores de velocidade angular e integra no tempo (pra pegar a posição angular)
  tGyroAnt = tGyroNow;
  tGyroNow = millis();
  GetGyroValue();  
  AngSpeedZ -= (gyroFilterA*tGyroNow + gyroFilterB);
  gyroTime = tGyroNow - tGyroAnt;
  yaw += gyroTime*AngSpeedZ;
}

void GetGyroValue()
{
  //Rotina para leitura dos valores de velocidade angular em torno de z
  //a leitura do giroscópio é dada em dois bits, nos endereços apresentados abaixo
  byte zMSB = ReadRegister(105, 0x2D);  //zMSB == z Most Significant Byte
  byte zLSB = ReadRegister(105, 0x2C);  //zLSB == z Least Significant Byte
  AngSpeedZ = ((zMSB << 8) | zLSB);     //junção do bit menos significativo com o mais significativo, para dar o valor inteiro referente à velocidade angular em z
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/
