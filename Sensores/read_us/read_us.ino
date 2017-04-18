//---------------------------------------------------------
//Inicializacao dos ultrassons
                                                  //ENTRA NO CODIGO
#include <NewPing.h>                           

#define MAX_DIST 200   //distancia maxima que o ultrassom "ve"

#define USF_TRIG 23   //Ultrassom frontal
#define USF_ECHO 25 
#define USL_TRIG 27   //Ultrassom da esquerda (left)
#define USL_ECHO 29 
#define USR_TRIG 31   //Ultrassom da direita (right)
#define USR_ECHO 33 

NewPing usf(USF_TRIG, USF_ECHO, MAX_DIST);
NewPing usl(USL_TRIG, USL_ECHO, MAX_DIST);
NewPing usr(USR_TRIG, USR_ECHO, MAX_DIST);

#define MIN_DIST_GAP 15

int lookGap=0, gapCountRight=0, gapCountLeft=0;
//-----------------------------------------------------------

//----------------------------------------------------------



//variaveis                                                     //ENTRA NO CODIGO
float usfDist, uslDistLast, uslDistNow, usrDistLast, usrDistNow;


void ReadUS(){
    //usfDistLast=usfDistNow;
    usfDist=usf.ping_cm();

    uslDistLast=uslDistNow;
    uslDistNow=usl.ping_cm();

    usrDistLast=usrDistNow;
    usrDistNow=usr.ping_cm();

    if(lookGap){
      if(uslDistNow > (uslDistLast + MIN_DIST_GAP)){
        gapCountLeft++;
      }
      if(usrDistNow > (usrDistLast + MIN_DIST_GAP)){
        gapCountRight++;
      }
    }
}

//----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  ReadUS();
}

void loop() {
  if(usfDist<15){
    lookGap=0;
  }
  else{
    lookGap=1;
  }
  ReadUS();
  Serial.print("CountLeft:  ");
  Serial.print(gapCountLeft);
  Serial.print("         ");
  Serial.print("CountRight:  ");
  Serial.println(gapCountRight);
  delay(50);
}
