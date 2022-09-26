//Servo
#include <Servo.h>

//Ultrasonic
const int TRIGGER_PIN = A1;
const int ECHO_PIN = A0;
const int inter_time = 100;
int pos = 90;    // variable to store the servo position
//Motor A
int enA = 3;
int in1 = 6;
int in2 = 9;
// Motor B connections
int enB = 1;
int in3 = 10;
int in4 = 11;

Servo myservo;

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 210);
  analogWrite(enB, 255);

  // Turn on motor A 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Turn on motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(2000);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}




void setup() {
  myservo.attach(5);  // attaches the servo on pin 9 to the servo object

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

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
  
  delay(inter_time);
  for (pos = 90; pos <= 90; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  directionControl();

}
