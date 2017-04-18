
#define FLAME_PIN_AN  A0
#define FLAME_PEAK 100

int sFlameLast=0, sFlameNow, flamePresence=0;

void ISeeFire(){
  /*flameLast=flameNow;*/
  sFlameNow=analogRead(FLAME_PIN_AN);
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
