#define Encoder1 12 // o retorno do encoder 
#define MB 7 // entarada A da ponte H
#define MA 8 // entrada B da ponte H
#define MOTOR_VEL 9 // O Motor (uso pra mandar potencia)
int z;
void setup() {
  pinMode(Encoder1, INPUT);
  pinMode(MB, OUTPUT);
  pinMode(MA, OUTPUT);
  pinMode(MOTOR_VEL, OUTPUT);
  digitalWrite(MA,HIGH);
  digitalWrite(MB,HIGH); // travar o motor se tiver girando, usando a ponte H
  delay(1000);
  Serial.begin(9600);
  z = 0; // inicializar o numero de rodadas como zero
}

void loop() {
  int i=0, cont=0;
  digitalWrite(MA,HIGH);
  digitalWrite(MB,HIGH); // travar o motor se tiver girando, usando a ponte H, uso quando realizar uma rotacao inteira
  delay(1000);
  digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW); 
  analogWrite(MOTOR_VEL,130); // mandar potencia pro motor
  while(i<131){ // a reducao que tem entre a saida do motor e o encoder
    while(cont < 15){ // quantidade de 1s que o encoder tem que retornar para um giro (por dentro, sem a reducao)
      if(digitalRead(Encoder1)){
          while(digitalRead(Encoder1)); // para  nunca pegar mais do que um 1 da leitura de um furo so no encoder
          cont+=1;
        }
      }
      cont = 0; /// zerando para utilizar
      i+=1;
  }
  Serial.println(z); // printar o numero de voltas dadas desde o inicio do codigo
  z+=1;
}
