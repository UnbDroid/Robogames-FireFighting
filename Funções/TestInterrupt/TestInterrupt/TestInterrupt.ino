//--------------------------------------------------------------------------
#include <NewPing.h>
                                                  // Usado aqui apenas para testes
#define MAX_DIST 200
#define TRIGGER 10
#define ECHO 11

NewPing US(TRIGGER, ECHO, MAX_DIST);
float dist;

//------------------------------------------------------------------------

//Especificações para o motor

#define Encoder1 3 // o retorno do encoder     A PORTA DELE PRECISA SER A 2 OU A 3, PARA FUNCIONAR O attachInterrupt()
#define MB 7 // entarada A da ponte H
#define MA 8 // entrada B da ponte H
#define MOTOR_VEL 9 // O Motor (uso pra mandar potencia)

//------------------------------------------------------------------------

#define LED_DEBUG 13
int cont1, cont0=0;                                     //Variáveis para Debug

//-------------------------------------------------------------------------

volatile long int cont=0;               //variável que é alterada no interrupt

void FazCoisa(){                        //Função chamada quando recebe algum sinal no pino definido
  cont++;
}

//-------------------------------------------------------------------------------------

void setup() {
  
  pinMode(MA, OUTPUT);
  pinMode(MB, OUTPUT);
  pinMode(MOTOR_VEL, OUTPUT);

  
  pinMode(LED_DEBUG, OUTPUT);
  
  pinMode(12, OUTPUT);
  digitalWrite(12, 1);          //Vcc pro teste com o US
  
  attachInterrupt(digitalPinToInterrupt(Encoder1), FazCoisa, RISING);         //Aqui se define qual é o pino responsável pelo interrupt (usado na função "digitalPinToInterrupt(PINO)"), qual o nome da função (ela não recebe nenhum parâmetro), e quando que a função é chamada ("RISING" para quando o sinal passa de 0 para 1, "CHANGE" quando vai de 0 para 1 ou de 1 para 0, as demais classificações vc procura)
  
  Serial.begin(9600);
}

void loop() {
  cont1++;          //quantas vezes o loop foi feito
  
  Serial.print(dist);
  Serial.print("            ");
  Serial.print(cont);
  Serial.print("        ");
  Serial.println(cont1);

 
  /*digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW);
  analogWrite(MOTOR_VEL,30);*/
  
  dist=US.ping_cm();
  if((dist<15)&&(dist>0)){                //Se tiver algo proximo do ultrassom, a roda gira. Se não, para.   Trecho para testes
    digitalWrite(MA,HIGH);
    digitalWrite(MB,LOW);
    analogWrite(MOTOR_VEL,130);
    cont0=0;
  }
  else{
    if((dist>=15)||(cont0>3)){
      digitalWrite(MA,HIGH);
      digitalWrite(MB,HIGH);
    }
    else{
      cont0++;
      
    }
  }/**/
}




