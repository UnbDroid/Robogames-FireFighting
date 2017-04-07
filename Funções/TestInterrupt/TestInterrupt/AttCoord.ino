//Especificações para o motor

#define Encoder1 3 // o retorno do encoder     A PORTA DELE PRECISA SER A 2 OU A 3, PARA FUNCIONAR O attachInterrupt()
#define MB 7 // entarada A da ponte H
#define MA 8 // entrada B da ponte H
#define MOTOR_VEL 9 // O Motor (uso pra mandar potencia)

//-------------------------------------------------------------------------------------------------------
#include <NewPing.h>
                                                  // Usado aqui apenas para testes
#define MAX_DIST 200
#define TRIGGER 10
#define ECHO 11

NewPing US(TRIGGER, ECHO, MAX_DIST);
float dist;
int cont0=0;

//--------------------------------------------------------------------------------------------------

// ENTRA NO CÓDIGO

#define COORD_X 1
#define COORD_Y 2

#define WEEL_DIAM 11.5                  //Diâmetro da roda    ATUALIZAR PARA O VALOR CERTO


volatile long int encCount[2], coord[2];                      // Vetores que armazenam quantas vezes o encoder foi ativado (nos eixos X e Y), e a posição do robô (também nos eixos X e Y)

int dir;                                                 // Variável alterada em momentos específicos do código, para saber em que direção o robô está avançando, para atualizar de forma correta sua posição nos eixos X e Y

void AttCoord() {                                        // Atualiza o valor de uma das coordenadas (x ou y) do robô
  coord[abs(dir)-1] = DegreeToCm ( Degree( encCount[abs(dir)-1] ) );          //transformando a contagem do encoder de uma das direções, na posição (em centímetros) do robô, em relação à posição inicial do mesmo
}

float DegreeToCm( float dg){                              // Converte o ângulo da roda para centímetros percorridos por ela
  return (((dg * PI)/360) * WEEL_DIAM);
}

 float Degree(long int count){                             // Converte a contagem do encoder para graus
  return (count * 360 / (16 * 131));
}



void AddEncoder(){                                          // Função do interrupt, que é chamada a cada vez que o encoder retorna sinal 1
  encCount[abs(dir)-1]  +=  ((dir)/abs(dir));
}

//------------------------------------------------------------------------------------------

void setup() {

  encCount[0]=0;                                      //zerando os vetores de localização
  encCount[1]=0;
  coord[0]=0;
  coord[1]=0;
  
  pinMode(MA, OUTPUT);                                //declarando os pinos do motor
  pinMode(MB, OUTPUT);
  pinMode(MOTOR_VEL, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(Encoder1), AddEncoder, RISING);                           //declaração do pino de interrupt, assim a função chamada dessa forma

  pinMode(12, OUTPUT);                    //Vcc artificial, usado para fazer testes com o US junto com o encoder. Não entra no código principal
  digitalWrite(12, 1);

  dir=COORD_X;                            //fornecendo a direção inicial do robô (arbitrário, nesse código específico)
  
  Serial.begin(9600);
}

void loop() {
  AttCoord();
  
  Serial.println(coord[abs(dir)-1]/*Degree( encCount[abs(dir)-1] )*/);                    //printando a distância percorrida pela roda, de acordo com a conversão feita (ou quantos graus ela girou, dependendo do teste)
  
  dist=US.ping_cm();
  if((dist<15)&&(dist>0)){                //Se tiver algo proximo do ultrassom, a roda gira. Se não estiver muito próximo, gira para o lado contrário. Se o ultrassom não detectar nada, a roda para.   Trecho para testes
    digitalWrite(MA,HIGH);
    digitalWrite(MB,LOW);
    analogWrite(MOTOR_VEL,130);
    cont0=0;
    dir=COORD_X;
  }
  else{
    if(dist>=15){
      digitalWrite(MA,LOW);
      digitalWrite(MB,HIGH);
      dir=-COORD_X;
    }
    else{
      cont0++;
      if(cont0>5){                      //Como a ultrassom retorna 0 de vez em quando, mesmo com algo constantemente em sua frente, torna-se necessário esse 0 se repertir algumas vezes até estar claro que realmente não há nada à frente
        digitalWrite(MA,HIGH);
        digitalWrite(MB,HIGH);
      }
    }
  }
  
}
