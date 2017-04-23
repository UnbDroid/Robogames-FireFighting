
void SetupWeels(){
  pinMode(MAL, OUTPUT);
  pinMode(MBL, OUTPUT);
  pinMode(MOTORL_VEL, OUTPUT);
  pinMode(MAR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MOTORR_VEL, OUTPUT);
}
double FromDegreesToEnc()
{
  /*vou colocar valor qualquer aqui, voce vai ver quantos graus esse valor gira e vai descomentar a segunda parte da 
  funcao colocando o valor de graus q o robo girou no lugar da variavel angulo_test */

  
  return 3000;
  /*
    double EncValue;
    EncValue = 3000*DegreesToTurn/angulo_test;
    return  EncValue;
  */
} 

void TurnEnc()
{
  if(DegreesToTurn>0)
  {
      if(encCountLeft[0]<Last_Enc[0]+(FromDegreesToEnc())){
         ClockWisePID();
      }else{
        DegreesToTurn = 0;
        Busy = ANALISE_STATE;  
      }    
  }else if(DegreesToTurn<0)
  {
      if(encCountRight[0]<Last_Enc[1]+(FromDegreesToEnc())){
         AntiClockWisePID();
      }else{
        DegreesToTurn = 0;
        Busy = ANALISE_STATE;  
      }    
  }
}

void ClockWisePID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, -powerRight);
  OnFwd(MOTOR_LEFT, powerLeft);
}

void AntiClockWisePID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, powerRight);
  OnFwd(MOTOR_LEFT, -powerLeft);
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
    }
    else{
      //manda mensagem de erro
    }
  }
}
