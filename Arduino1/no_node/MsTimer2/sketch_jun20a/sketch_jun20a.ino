#include <MsTimer2.h>

// this sketch cycles three servos at different rates 

//#include <ServoTimer2.h>  // the servo library
//#define servoPin  3

#include "NewPing.h"
#include <Servo.h>

//Timer<3, micros> timer;

//ServoTimer2 servoRoll;    // declare variables for up to eight servos
//uint16_t servo_pos= 1350;
int servo_pos = 75; 
Servo myservo; 



//Ultrasonic connections
const int TRIGGER_PIN_front = A1;
const int ECHO_PIN_front = A0;
const int TRIGGER_PIN_back = A3;
const int ECHO_PIN_back = A2;
float duration;

// Motor A connections
int enA = 5;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 6;
int in3 = 12;
int in4 = 4;


// Control Values Motor
enum Control {forward=0,backward,AforwardBbackward,BforwardAbackward};
Control control_motor = forward;
uint16_t a_vel = 0; 
uint16_t b_vel = 0;


//Buzzer
int buzzerPin = 11;
uint16_t buzzer_active = 0;


String myStringFront;
String myStringBack;

int MAX_DISTANCE = 100;

NewPing sonar_front(TRIGGER_PIN_front, ECHO_PIN_front, MAX_DISTANCE);
NewPing sonar_back(TRIGGER_PIN_back, ECHO_PIN_back, MAX_DISTANCE);


//Functions 
/*
void distanceReadFront(){
  digitalWrite(TRIGGER_PIN_front, HIGH);
  digitalWrite(TRIGGER_PIN_front, LOW);
  duration = pulseIn (ECHO_PIN_front, HIGH);
  float distance = (duration/ 2.0) / 29.0;
  String myString = "f" + String(distance);
  Serial.println(myString);
  }
*/
void distanceReadFront(){
  float result = sonar_front.ping_cm();
  String myString = "f" + String(result);
  Serial.println(String(myString));
  }
void distanceReadBack(){
  float result = sonar_back.ping_cm();
  String myString = "b" + String(result);
  Serial.println(String(myString));
  }

void servoWrite(){
  myservo.write(servo_pos);
  }

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





void setup()
{
  Serial.begin(9600,SERIAL_8E2);
  
  //servo
//  servoRoll.attach(servoPin); 

  //Ultrasonic sensors
  pinMode (TRIGGER_PIN_front, OUTPUT);
  pinMode (ECHO_PIN_front, INPUT);
  pinMode (TRIGGER_PIN_back, OUTPUT);
  pinMode (ECHO_PIN_back, INPUT);

  myservo.attach(3);
  
  
  //Buzzer
  pinMode(buzzerPin, OUTPUT);  
  

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
  
  MsTimer2::set(1000, distanceReadFront);
  MsTimer2::set(1000, distanceReadBack);
  MsTimer2::set(1000, servoWrite);
  //timer.every(100000, speedControl);
  //timer.every(1, distanceRead,TRIGGER_PIN_front,ECHO_PIN_front,"b");

  MsTimer2::start();
  
}

void loop()
{ 
  /*
  float distanceReading_front_output = distanceRead(TRIGGER_PIN_front,ECHO_PIN_front);
  myStringFront = "f";
  myStringFront += String(distanceReading_front_output);
  float distanceReading_back_output = distanceRead(TRIGGER_PIN_back,ECHO_PIN_back);
  myStringBack = "b";
  myStringBack += String(distanceReading_back_output);
  Serial.println(myStringFront);
  delayMicroseconds(10);
  Serial.println(myStringBack);
  delayMicroseconds(10);
  */
}
