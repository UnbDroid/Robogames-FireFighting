

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

void SetupInterrupt(){
  encCountLeft[0]=0;
  encCountLeft[1]=0;
  encCountRight[0]=0;
  encCountRight[1]=0;
  coord[0]=0;
  coord[1]=0;

  attachInterrupt(digitalPinToInterrupt(ENCODER_LEFT), AddEncoderLeft, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER_RIGHT), AddEncoderRight, RISING);
}

