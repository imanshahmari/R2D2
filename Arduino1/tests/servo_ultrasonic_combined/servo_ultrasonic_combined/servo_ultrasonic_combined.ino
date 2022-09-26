#include <Servo.h>

//UlraSonic Sensor
const int TRIGGER_PIN = A1;
const int ECHO_PIN = A0;
const int inter_time = 100;
int pos = 0;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode (TRIGGER_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object
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

  if(distance < 7.5 && distance >0){
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    } 
  }
}
