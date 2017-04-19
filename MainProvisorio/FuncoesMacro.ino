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

//_______________________________________________________________________________________________________________________________________________________________________________________________________________-
// CHECA SALA
