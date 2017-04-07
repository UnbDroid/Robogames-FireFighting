

void OnFwd(int motor, int power){
  if(motor==MOTOR_LEFT){
    digitalWrite(MAL, 1);//(1+(power/abs(power)))/2);
    digitalWrite(MBL, 0);//-(1+(power/abs(power)))/2);
    digitalWrite(MOTORL_VEL, power);
  }
  else{
    if(motor==MOTOR_RIGHT){
      digitalWrite(MAR, 1);//(1+(power/abs(power)))/2);
      digitalWrite(MBR, 0);//-(1+(power/abs(power)))/2);
      digitalWrite(MOTORR_VEL, power);
      dir = dir*(power/abs(power));
    }
    else{
      //manda mensagem de erro
    }
  }
}

