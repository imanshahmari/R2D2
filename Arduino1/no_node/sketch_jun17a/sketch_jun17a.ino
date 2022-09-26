void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

  Serial.write(analogRead(A0)/4);
  // put your main code here, to run repeatedly:
  delay(1);
}
