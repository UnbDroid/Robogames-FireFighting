

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
