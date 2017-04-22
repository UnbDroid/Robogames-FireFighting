// Setup dos motores

void SetupWheels(){
  pinMode(MAL, OUTPUT);
  pinMode(MBL, OUTPUT);
  pinMode(MOTORL_VEL, OUTPUT);
  pinMode(MAR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MOTORR_VEL, OUTPUT);
}

//________________________________________________________________________________________________________________________________________________________________________________
  // DESLOCAMENTO:


  
      //  OnFwd:
      
void OnFwd(int motor, int power){
  if(motor==MOTOR_LEFT){
    int vel;
    digitalWrite(MAL, (1+(power/abs(power)))/2);
    digitalWrite(MBL, (1-(power/abs(power)))/2);
    vel=abs(power);
    analogWrite(MOTORL_VEL, vel);
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
//----------------------------------------------------------------------------
        // Desliga motores


//---------------------------------------------

        //  PID
void SetupPID()
{
  PID_ESQ.SetOutputLimits(110, 180);
  PID_DIR.SetOutputLimits(110, 180);
  PID_ESQ.SetSampleTime(100);
  PID_DIR.SetSampleTime(100);
  PID_ESQ.SetMode(AUTOMATIC);
  PID_DIR.SetMode(AUTOMATIC);
}

void UpdatePID()
{
  ctrlPID = abs(encCountLeft[abs(dir)]-encCountRight[abs(dir)]);
  PID_ESQ.Compute();
  PID_DIR.Compute();
}


void FwdPID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, powerRight);
  OnFwd(MOTOR_LEFT, powerLeft);
}

void RevPID(){
  UpdatePID();
  OnFwd(MOTOR_RIGHT, -powerRight);
  OnFwd(MOTOR_LEFT, -powerLeft);
}

//________________________________________________________________________________________________________________________________________________________________________________
// TURN
void Turn(int degree){
  if(turnStart){
    allowEncoder = 0;
     
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

    allowEncoder = 1;
    busy = STATE_ANALISER;
    
    turnStart=1;
  }
}
//__________________________________________________________________________________________________________________________________________________________________________________________
//WALK DISTANCE
void WalkDist(int dist){
  int distLast=coord[dir];
  while((coord[dir]-distLast)<dist){
    FwdPID();
  }
  OnFwd(MOTOR_RIGHT, 0);
  OnFwd(MOTOR_LEFT, 0);
}
//_________________________________________________________________________________________________________________________________________________________________________________
//APAGA O FOGO
void PutFireDown(){
  int pos;
  //     COLOQUE AQUI A FUNÇÃO PARA BALANÇAR O ESGUICHO, JOGANDO AGUA

}
//_______________________________________________________________________________________________________________________________________________________________________________________________________________-
// CHECA SALA
void CheckRoom(){
  int pos, candlePos=-1;
  allowEncoder=0;
  if((countSteps==7)||(countSteps==17)){
    WalkDist(20);
  }
  else{
    WalkDist(40);
  }
  for(pos=0/*posicao minima*/; pos<180/*posicao maxima*/; pos++){
    ISeeFire();
    myservo.write(pos);
    if(flamePresence){
      candlePos = pos;
      pos=180;
    }
  }
  if(candlePos==-1){          //Nao achou vela, volta
    Turn(180);
    allowEncoder=0;
    ReadUS();
    while(usfDist<10){
      ReadUS();
      FwdPID();
    }
    OnFwd(MOTOR_LEFT, 0);
    OnFwd(MOTOR_RIGHT, 0);
  }
  else{
    Turn(candlePos/*ou algo parecido, tem que ver o angulo certo*/);
    myservo.write(-candlePos);
    allowEncoder = 0;
    ISeeFire();
    while(sFlameNow<FLAME_DIST_MIN){
      ISeeFire();
      FwdPID();
    }
    OnFwd(MOTOR_LEFT, 0);
    OnFwd(MOTOR_RIGHT, 0);

    PutFireDown();

    
  }
}

