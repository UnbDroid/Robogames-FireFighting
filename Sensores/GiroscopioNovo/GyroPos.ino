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
