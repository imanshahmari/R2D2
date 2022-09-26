const int TRIGGER_PIN = A1;
const int ECHO_PIN = A0;
const int inter_time = 100;

void setup() {
  Serial.begin(9600);
  pinMode (TRIGGER_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
}

void loop() {
  unsigned long duration;
  float distance;
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(100.0);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distance = (duration / 2.0) / 29.0;
  Serial.print("Data:");
  Serial.print (millis() / 100.0);
  Serial.print(", d = ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(inter_time);
}
