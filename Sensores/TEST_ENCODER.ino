#define Encoder1 12
int z;
void setup() {
  pinMode(Encoder1, INPUT);
  Serial.begin(9600);
  z = 0;
}

void loop() {
  int i=0, cont=0;
  while(i<131){
    while(cont < 1024){
      if(digitalRead(Encoder1)){
          cont+=1;
        }
      }
      cont = 0;
      i+=1;
  }
  Serial.println(z);
  z+=1;
}
