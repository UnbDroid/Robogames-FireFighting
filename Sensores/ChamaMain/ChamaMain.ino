
#define FLAME_PIN_AN  
#define FLAME_PEAK 

int sFlameLast=0, sFlameNow, flamePresence=0;

void ISeeFire(){
  /*flameLast=flameNow;*/
  flameNow=analogRead(FLAME_PIN_AN);
  /*if((sFlameNow-sFlameLast)>FLAME_PEAK){
    flamePresence=1;
  }*/
}

void setup() {
  Serial.begin(9600);
  pinMode(FLAME_PIN_AN, INPUT);
}

void loop() {
  ISeeFire();
  Serial.println(sFlameNow);
  /*Serial.println(flamePresence);*/
  delay(50);
}
