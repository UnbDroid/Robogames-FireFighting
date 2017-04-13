#define mic_in A3
void setup() {
  Serial.begin(9600);
  pinMode(mic_in, INPUT);
}

void loop() {
  int s;
  s = analogRead(mic_in);
  Serial.println(s);
  delay(100);
}
