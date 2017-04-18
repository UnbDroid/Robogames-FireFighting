
#define WEEL_DIAM 15

int allowEncoder=1, dir;
volatile long int encCountLeft[2], encCountRight[2], coord[2];

float DegreeToCm( float dg){                              // Converte o ângulo da roda para centímetros percorridos por ela
  return (((dg * PI)/360) * WEEL_DIAM);
}

 float Degree(long int count){                             // Converte a contagem do encoder para graus
  return (count * 360 / (16 * 131));
}


void AddEncoderLeft(){
  if(allowEncoder){
    encCountLeft[abs(dir)-1]  +=  ((dir)/abs(dir));
  }
}

void AddEncoderRight(){
  if(allowEncoder){
    encCountRight[abs(dir)-1]  +=  ((dir)/abs(dir));
  }
}

void AttCoord() {                                        // Atualiza o valor de uma das coordenadas (x ou y) do robô
  coord[abs(dir)-1] = (DegreeToCm ( Degree( encCountRight[abs(dir)-1] ) ) + DegreeToCm ( Degree( encCountLeft[abs(dir)-1] ) ) ) / 2;          //transformando a contagem do encoder de uma das direções, na posição (em centímetros) do robô, em relação à posição inicial do mesmo
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
