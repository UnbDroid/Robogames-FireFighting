/*esqueleto de codigo para implementar*/

/*---------------------------------------------------------------------------*/
/* Defines dos valores do busy*/

#define STATE_ANALISER 1

#define FORWARD 2                    // macro que manda o robo seguir adiante na casa

#define TURN_LEFT 3                  // macro que manda o robo girar 90 graus para a esquerda
#define TURN_RIGHT 4                 // macro que manda o robo girar 90 graus para a direita
#define TURN_AROUND 5                // macro que manda o robo girar 180 graus

#define CHECK_ROOM 6                 // macro que manda o robo checar o quarto em que ele está
#define CANDLE 7                     // macro que manda o robo apagar a vela

#define BACK_HOME 8                  // macro que manda o robo voltar pro inicio, de acordo com a posicao dele no mapa

#define WALK_DISTANCE 9              // macro que manda o robo andar a distancia especifica (somente sera chamada em um momento do codigo, antes de entrar no ultimo quarto, portanto se refere a apenas uma distancia especifica)

#define WALK_TO_WALL 10              // macro que manda o robo andar ate chegar perto da parede (necessário após checar a sala, para ficar relativamente no centro do corredor)
#define WALK_LOOK_LEFT 11
#define WALK_LOOK_RIGHT 12


int busy = STATE_ANALISER;
/*---------------------------------------------------------------------------*/
// Servo
#include <Servo.h>

Servo myservo;
#define SERVO_PIN 6

//---------------------------------------------------------
//Inicializacao dos ultrassons
                                                  
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

float usfDist, uslDistLast, uslDistNow, usrDistLast, usrDistNow;
int lookGapLeft=0, lookGapRight=0, gapFound=0;

//------------------------------------------------------------------------------------------------------

// Inicialização e variáveis do sensor de chama
#define FLAME_PIN_AN  A0
#define FLAME_PEAK 60
#define FLAME_DIST_MIN 400

int sFlameLast=0, sFlameNow, flamePresence=0;

/*---------------------------------------------------------------------------*/
/*Defines para os motores*/
#define MAL 8   // entrada A da ponte H da roda esquerda
#define MBL 9   // entrada B da ponte H da roda esquerda
#define MOTORL_VEL  10   // entrada de potência do motor esquerdo
#define MAR 11   // entrada A da ponte H da roda direito
#define MBR 12   // entrada B da ponte H da roda direito
#define MOTORR_VEL  13   // entrada de potência do motor direito

#define MOTOR_LEFT 1
#define MOTOR_RIGHT 2

/*-----------------------------------------------------------------------------------*/
//Do Interrupt:
#define ENCODER_LEFT 1
#define ENCODER_RIGHT 2

#define WEEL_DIAM 15

int allowEncoder=1;
volatile long int encCountLeft[2], encCountRight[2], coord[2];

/*-----------------------------------------------------------------------------------*/
// Variaveis do PID
#include <PID_v1.h>

double powerLeft=100, powerRight=100;
double ctrlPID;
double Klp = 1, Kli = 1, Kld = 1;
double Krp = 1, Kri = 1, Krd = 1;


PID PID_ESQ(&ctrlPID, &powerLeft, 0, Klp, Kli, Kld, REVERSE);
PID PID_DIR(&ctrlPID, &powerRight, 0, Krp, Kri, Krd, DIRECT);

//-------------------------------------------------------------------------------------------------------------
/* Includes, defines e variáveis do giroscópio*/

#include <Wire.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define REFERENCE 0X25

int AngSpeedZ, gyroTime, gyroAddress = 105;
unsigned long tGyroNow=0, tGyroAnt;
long int yaw=0;
float gyroFilterB=0, gyroFilterA = 0;
/*-----------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/
/* Variáveis de localização*/

#define COORD_X 1
#define COORD_Y 2

int gapCountRight=0, gapCountLeft=0;                           // número de vãos que o robô encontrou enquanto percorre a arena (depois alguem traduz a variável pro inglês, pq n me lembro agora a tradução kkk)
int x=0, y=0;
int dir=-COORD_Y , flameDetect=0, inRoom=0;


int countSteps=0, backHomeSteps=0, flameDown=0;     //Variaveis para main provisório, contando cada passo

/*----------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/* Variaveis para controle das funções*/
int turnStart=1;                        //turnStart serve para a função identificar se é a primeira vez que chama a função Turn

/*----------------------------------------------------------------------------------------------------*/


/*
  --------------------------------------inicizalizacao---------------------------
  funcao de inicializar os sensores/ponte H/ motores/ encoders
*/

/*--------------------------------------------------------------------------------*/




/*______________________________________________FUNCOES DE ALTO NIVEL !!_________________________________________________*/
/*
 funcao que recebe duas coordenadas (X,Y) atuais e retorna o local do robo numa matriz de 9 (qual casa)
 |20|21|22|
 |10|11|12|
 |00|01|02|
 ///////////////////// NOTE QUE AS AREAS NAO VAO SER QUADRADAS !! VOA DEPENDER DO TAMANHO DA ARENA E DOS QUARTOS
*/
/*_______________________________________________________________________________________________________________________*/  
/*
  funcao que seria chamada quando o robo estiver na porta de cada quarto que procura a vela ! -------- se achar, sera chamada a funcao de baixo - apagar a vela 
*/
/*_______________________________________________________________________________________________________________________*/
/*
  se a funcao de cima achar a vela essa funcao sera chamada e ela realizaria o processo de apagar a vela
*/
/*_______________________________________________________________________________________________________________________*/
/*
  sair do quarto e voltar para a orientacao certa para continur a trajetoria do robo caso a funcao de procurar a vela nao achar a vela
*/
/*_______________________________________________________________________________________________________________________*/
/*
  realiza a volta pra base sem depender do local que o robo atualmente esta ! usando a matriz da primeira funcao
*/
/*_______________________________________________________________________________________________________________________*/
/*
  comparacao entre as coordenadas X e Y atuais e a quantiadade de vao passados
*/
/*_______________________________________________________________________________________________________________________*/
/*
 caso a funcao de cima retorna que o valor das coordenadas eh inconveniente com a quantidade de vaos passados
 esta funcao faria o robo ir ao checkpoint para recomecar a parte na qual  ele se perdeu
*/
/*_______________________________________________________________________________________________________________________*/
/*
 ---------------------------------------FUNCAO CORACAO DO CODIGO !--------------------------
 ----DIRECIONARIA O ANDAMENTO DO CODIGO PARA AS OUTRAS FUNCOES USANDO VARIAVEL GLOBAR DO ESTADO ATUAL DO ROBO (o nome dessa variavel provavelmente sera busy)! 
 --------------- RECOMENDO DEIXAR A IMPLEMENTACAO DESSA FUNCAO PARA O ABDU/MINIBAU
*/

int GoHome(){
  if((countSteps==7)||(countSteps==11)){              //quartos de cima 
    if((backHomeSteps==0)||(backHomeSteps==2)||(backHomeSteps==4)){
      return TURN_LEFT;
    }
    else{
      return WALK_TO_WALL;
    }
  }

  
  else{                                           //quartos de baixo
    if(backHomeSteps==0){               
      if(countSteps==17){
        return TURN_LEFT;
      }
      else{
        return TURN_RIGHT;
      }
    }
    
    else{
      if(backHomeSteps==1){
        return WALK_LOOK_LEFT;
      }
      else{
        if(backHomeSteps==2){
          return TURN_RIGHT;
        }
        else{
          if((backHomeSteps==4)||(backHomeSteps==6)){
            return TURN_LEFT;
          }

          else{
            return WALK_TO_WALL;
          }
        }
      }
    }
  }
  backHomeSteps++;
}

void AnaliseState(){          //Sempre antes de virar para entrar em cada sala, tem que andar uma pequena distância para ficar no centro da porta
  if(flameDown){
    busy = GoHome();
  }
  else{
    if((countSteps==0)||(countSteps==12)||(countSteps==18)){
      busy = TURN_LEFT;
      
    }
    
    else{
      if((countSteps==2)||(countSteps==4)||(countSteps==6)||(countSteps==8)||(countSteps==10)||(countSteps==14)||(countSteps==16)||(countSteps==20)){
        busy = TURN_RIGHT;
      }
      
      else {
        if((countSteps==-1)){
          busy = TURN_AROUND;
        }/**/

        else{
          if((countSteps==7)||(countSteps==11)||(countSteps==17)||(countSteps==21)){
            busy = CHECK_ROOM;
          }
          
          else{
            /**/if((countSteps==-1)){
              busy = WALK_DISTANCE;
            }
            
            else{
              if((countSteps==1)||(countSteps==3)||(countSteps==9)||(countSteps==15)){
                busy = WALK_TO_WALL;
              }

              else{
                /**/if((countSteps==-1)){
                  busy = WALK_LOOK_LEFT;
                }

                else{
                  if((countSteps==5)||(countSteps==13)||(countSteps==19)){
                    busy = WALK_LOOK_RIGHT;
                  }
                }
              }
            }
          }
        }
      }
    }
    countSteps++;
  }
}

void Heart(){
  switch(busy){
      /*case WALK_STAIRS:
        //chama função para atravessar a escada
        //se a função de voltar pra casa ainda não tiver sido chamada
        break;*/
      case STATE_ANALISER:
        AnaliseState();
        break;
        
      /*case FORWARD:
        OnFwd(MOTOR_RIGHT, 90);
        OnFwd(MOTOR_LEFT, 90);
        break;*/

      case TURN_LEFT:
        Turn(-90);            
        break;

      case TURN_RIGHT:
        Turn(90);
        break;

      /*case TURN_AROUND:
        Turn(180);
        break;*/

      case CHECK_ROOM:
        //Função para checar a sala
        break;

      case CANDLE:
        //Função para ir para perto da vela e apagá-la
        break;

      /*case WALK_DISTANCE:
        //Função para o robô percorrer uma distância específica
        break;*/

      case WALK_TO_WALL:
        FwdPID();
        if(usfDist<10){
          busy = STATE_ANALISER;
        }
          
        break;

      case WALK_LOOK_LEFT:
          lookGapLeft=1;
          FwdPID();
        break;

      case WALK_LOOK_RIGHT:
          lookGapRight=1;
          FwdPID();
        break;
  }
}

/*______________________________________-----------------------------------______________________________________________*/
/*--------------------------------------___________________________________----------------------------------------------*/

/*______________________________________FUNCOES DE BAIXO NIVEL !!!!!!!!!!!!______________________________________________*/



/*_______________________________________________________________________________________________________________________*/
/*
  funcao que recebe a distancia como parametro e anda essa distancia pedida. (poderia ser arrompida pela leitura do vao no US)
*/

/*_______________________________________________________________________________________________________________________*/
/*
  funcoes de girar (podem ser varias para cada um angulo e direcao, ou uma que recebe angulo e direcao como parametro e faz para qualquer angulo "funcao inteligente")
  ---para escolher qual o tipo dessa funcao, recomendo o segundo pq poderia ser reutilizado e seria bem mais inteligente 
  MAS*** as vezes esse tipo de funcao eh mais arriscado do que implementar funcao para cada caso! entao se n der muito certo o segundo tipo, pode ser o primeiro
*/

/*_______________________________________________________________________________________________________________________*/

/*
 filtros para os sensores que necessitam
*/

void setup()
{

  SetupInterrupt();
  SetupWheels();
  SetupPID();
  SetupGyroscope(2000);
  pinMode(FLAME_PIN_AN, INPUT);
  sFlameNow=analogRead(FLAME_PIN_AN);
  myservo.attach(SERVO_PIN);
  /*
      setttar as parada todim
  -----------------------------------------------------------
    chamar a funcao de inicializar os bagul
    ;
    e anda certa distancia ate o vao atravessando a escada     --> Minibau: fiz de um jeito que essa parte fica inclusa no coração do codigo, não precisa ser chamada aqui
    depois de andar, somar o valor da distancia entre o ponto de inicio ate o final da escada (projetado no x ja)
  */
}

void loop()
{
  ReadUS();
  Heart();
}
