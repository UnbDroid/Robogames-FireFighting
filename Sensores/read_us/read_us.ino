//---------------------------------------------------------
//Inicializacao dos ultrassons
                                                  //ENTRA NO CODIGO
#include <NewPing.h>                           

#define MAX_DIST 200   //distancia maxima que o ultrassom "ve"

#define USF_TRIG 4   //Ultrassom frontal
#define USF_ECHO 5 
#define USL_TRIG 2   //Ultrassom da esquerda (left)
#define USL_ECHO 3 
#define USR_TRIG 6   //Ultrassom da direita (right)
#define USR_ECHO 7 

NewPing usf(USF_TRIG, USF_ECHO, MAX_DIST);
NewPing usl(USL_TRIG, USL_ECHO, MAX_DIST);
NewPing usr(USR_TRIG, USR_ECHO, MAX_DIST);
//-----------------------------------------------------------

//variaveis para testes com os LED's          
int ledl=8, ledf=9, ledr=10;                    //ISSO NAO ENTRA NO CODIGO PRINCIPAL

//----------------------------------------------------------



//variaveis                                                     //ENTRA NO CODIGO
float usfDist, uslDist, usrDist;


void ReadUS(){
    usfDist=usf.ping_cm();
    uslDist=usl.ping_cm();
    usrDist=usr.ping_cm();
}

//----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  pinMode(ledl, OUTPUT);
  pinMode(ledf, OUTPUT);
  pinMode(ledr, OUTPUT);
}

void loop() {
  ReadUS();
  if((uslDist<=10)&&(uslDist>0)){
    digitalWrite(ledl, 1);
  }
  else{
    digitalWrite(ledl, 0);
  }
  if((usfDist<=10)&&(usfDist>0)){
    digitalWrite(ledf, 1);
  }
  else{
    digitalWrite(ledf, 0);
  }
  if((usrDist<=10)&&(usrDist>0)){
    digitalWrite(ledr, 1);
  }
  else{
    digitalWrite(ledr, 0);
  }
  /*Serial.print(uslDist);
  Serial.print("      ");
  Serial.print(usfDist);
  Serial.print("      ");
  Serial.println(usrDist);
  delay(400);
  Serial.print(usf.timing());
  Serial.print("      ");
  Serial.print(usl.timing());
  Serial.print("      ");
  Serial.println(usr.timing());
  delay(400);*/
}
