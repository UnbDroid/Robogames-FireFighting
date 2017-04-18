

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
    }
    else{
      //manda mensagem de erro
    }
  }
}

