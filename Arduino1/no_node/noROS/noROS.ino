// this sketch cycles three servos at different rates 

//#include <ServoTimer2.h>  // the servo library
//#define servoPin  3
#include <arduino-timer.h>
#include "NewPing.h"
#include <Servo.h>

Timer<4, micros> timer;

//ServoTimer2 servoRoll;    // declare variables for up to eight servos
//uint16_t servo_pos= 1350;
int servo_pos = 120; 
Servo myservo; 



//Ultrasonic connections
const int TRIGGER_PIN_front = A1;
const int ECHO_PIN_front = A0;
const int TRIGGER_PIN_back = A3;
const int ECHO_PIN_back = A2;
float duration;

// Motor A connections
int enA = 6;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 5;
int in3 = 13;
int in4 = 12;


// Control Values Motor
enum Control {forward=0,backward,AforwardBbackward,BforwardAbackward};
Control control_motor = forward;
uint16_t a_vel = 0; 
uint16_t b_vel = 0;


//Buzzer
//int buzzerPin = 11;
//uint16_t buzzer_active = 0;


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
  String my_word = Serial.readString();
  if(my_word.substring(0,1) == 's'){
    String value = my_word.substring(1);
    myservo.write(value.toInt());
  }
}

void speedControl() {
  // Turn on motors
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW );
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 150);
  analogWrite(enA, 150);
  
}





void setup()
{
  Serial.begin(9600,SERIAL_8E2);
  
  //servo
  //servoRoll.attach(servoPin); 

  //Ultrasonic sensors
  pinMode (TRIGGER_PIN_front, OUTPUT);
  pinMode (ECHO_PIN_front, INPUT);
  pinMode (TRIGGER_PIN_back, OUTPUT);
  pinMode (ECHO_PIN_back, INPUT);

  myservo.attach(3);
  
  
  //Buzzer
  //pinMode(buzzerPin, OUTPUT);  
  

  // Set all the motor control pins to outputs
  //pinMode(enA, OUTPUT);
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
  
  timer.every(100000, distanceReadFront);
  timer.every(100000, distanceReadBack);
  timer.every(100000, servoWrite);
  //timer.every(100000, speedControl);
  //timer.every(1, distanceRead,TRIGGER_PIN_front,ECHO_PIN_front,"b");
  
}

void loop()
{ 
  timer.tick();
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
