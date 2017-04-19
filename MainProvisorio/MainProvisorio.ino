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



int busy = WALK_STAIRS;
/*---------------------------------------------------------------------------*/

//---------------------------------------------------------
//Inicializacao dos ultrassons
                                                  
#include <NewPing.h>                           

#define MAX_DIST 200   //distancia maxima que o ultrassom "ve"

#define USF_TRIG    //Ultrassom frontal
#define USF_ECHO  
#define USL_TRIG    //Ultrassom da esquerda (left)
#define USL_ECHO  
#define USR_TRIG    //Ultrassom da direita (right)
#define USR_ECHO  

NewPing usf(USF_TRIG, USF_ECHO, MAX_DIST);
NewPing usl(USL_TRIG, USL_ECHO, MAX_DIST);
NewPing usr(USR_TRIG, USR_ECHO, MAX_DIST);

float usfDistNow, uslDistNow, usrDistNow, usfDistLast, uslDistLast, usrDistLast;
//-----------------------------------------------------------

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

/*-----------------------------------------------------------------------------------*/
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

int countSteps=0, flameDown=0;     //Variavel para main provisório, contando cada passo

/*----------------------------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/* Variaveis para controle das funções*/
int turnStart=1;                        //turnStart serve para a função identificar se é a primeira vez que chama a função Turn

/*----------------------------------------------------------------------------------------------------*/


/*
  --------------------------------------inicizalizacao---------------------------
  funcao de inicializar os sensores/ponte H/ motores/ encoders
*/
void SetupWheels(){
  pinMode(MAL, OUTPUT);
  pinMode(MBL, OUTPUT);
  pinMode(MOTORL_VEL, OUTPUT);
  pinMode(MAR, OUTPUT);
  pinMode(MBR, OUTPUT);
  pinMode(MOTORR_VEL, OUTPUT);
}
/*--------------------------------------------------------------------------------*/

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

void AnaliseState(){
  if(flameDown){
    busy = BACK_HOME
  }
  else{
    if((countSteps==0)||(countSteps==0)){
      busy = TURN_LEFT;
      
    }
    
    else{
      if((countSteps==0)||(countSteps==0)){
        busy = TURN_RIGHT;
      }
      
      else {
        if((countSteps==0)||(countSteps==0)){
          busy = TURN_AROUND;
        }

        else{
          if((countSteps==0)||(countSteps==0)){
            busy = CHECK_ROOM;
          }

          else{
            if((countSteps==0)||(countSteps==0)){
              busy = CHECK_ROOM;
            }

            else{
              if((countSteps==0)||(countSteps==0)){
                busy = WALK_DISTANCE;
              }

              else{
                if((countSteps==0)||(countSteps==0)){
                  busy = WALK_TO_WALL;
                }
              }
            }
          }
        }
      }
    }
    countSteps++;
  }
  
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
        
      case FORWARD:
        OnFwd(MOTOR_RIGHT, 90);
        OnFwd(MOTOR_LEFT, 90);
        break;

      case TURN_LEFT:
        Turn(-90);            //função Turn(int degree) já existe, só falta passar para cá
        break;

      case TURN_RIGHT:
        Turn(90);
        break;

      case TURN_AROUND:
        Turn(180);
        break;

      case CHECK_ROOM:
        //Função para checar a sala
        break;

      case CANDLE:
        //Função para ir para perto da vela e apagá-la
        break;

      case BACK_HOME:
        //Função que calcula a rota para voltar para o inicio, e manda o robô pra lá
        break;

      case WALK_DISTANCE:
        //Função para o robô percorrer uma distância específica
        break;

      case WALK_TO_WALL:
        //Função para o robô chegar a uma distância específica da parede na frente
        break;
  }
}

/*______________________________________-----------------------------------______________________________________________*/
/*--------------------------------------___________________________________----------------------------------------------*/

/*______________________________________FUNCOES DE BAIXO NIVEL !!!!!!!!!!!!______________________________________________*/

void ReadUS(){
  usfDistLast=usfDistNow;
  usfDistNow=usf.ping_cm();
  uslDistLast=uslDistNow;
  uslDistNow=usl.ping_cm();
  usrDistLast=usrDistNow;
  usrDistNow=usr.ping_cm();
  if(uslDistNow>(uslDistLast+20)){
    gapCountLeft++;
  }
  if(usrDistNow>(usrDistLast+20)){
    gapCountRight++;
  }
}

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
void Turn(int degree){
  if(turnStart){
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
    
    busy = STATE_ANALISER;
    
    turnStart=1;
  }
}
/*_______________________________________________________________________________________________________________________*/

/*
 filtros para os sensores que necessitam
*/

void setup()
{
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
  Heart();
}
