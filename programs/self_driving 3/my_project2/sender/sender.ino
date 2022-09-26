// this sketch cycles three servos at different rates 
#include <arduino-timer.h>
#include "NewPing.h"
#include "config.h"
#include <Servo.h>


int servo_pos = 45; 
int MAX_DISTANCE = 150;
String inputString = "";// a String to hold incoming data
String controlString = "";
bool stringComplete = false;  // whether the string is complete
bool servoInput = false;
enum Control {forward=0,backward,AforwardBbackward,BforwardAbackward};
// control_motor = (Control) actuation_msg.data[2];

Timer<2, micros> timer;
Servo myservo;
NewPing sonar_front(TRIGGER_PIN_front, ECHO_PIN_front, MAX_DISTANCE);
NewPing sonar_back(TRIGGER_PIN_back, ECHO_PIN_back, MAX_DISTANCE);


//Functions 
void distanceReadFront(){
  float result = sonar_front.ping_cm();
  String myString = "uf" + String(result);
  Serial.println(String(myString));
  }
void distanceReadBack(){
  float result = sonar_back.ping_cm();
  String myString = "ub" + String(result);
  Serial.println(String(myString));
  }

void servoWrite(String control){
  int servoValue = control.substring(1).toInt();
  myservo.write(servoValue);
}

void buzzer(String control){
  int buzzer_active = control.substring(1).toInt();
  if(buzzer_active == 1){
  digitalWrite(buzzerPin, HIGH);
  }
  else{digitalWrite(buzzerPin, LOW);}
  
}

void speedControl(String control) {
  Control mode = (Control) control.substring(1,2).toInt();
  int righWheel = control.substring(2,5).toInt();
  int leftWheel = control.substring(5,8).toInt();

  analogWrite(enB, righWheel);
  analogWrite(enA, leftWheel);
  
  switch(mode){
    case forward: 
      // Turn on motor B
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Turn on motor A
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); 
    break;
    
    case backward:
      // Turn on motor A 
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Turn on motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    break;
    
    case AforwardBbackward:
      // Turn on motor A 
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Turn on motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH); 
    break;
    
    case BforwardAbackward:
      // Turn on motor A 
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Turn on motor B
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW); 
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  
  //servo
  //servoRoll.attach(servoPin); 

  //Ultrasonic sensors
  pinMode (TRIGGER_PIN_front, OUTPUT);
  pinMode (ECHO_PIN_front, INPUT);
  pinMode (TRIGGER_PIN_back, OUTPUT);
  pinMode (ECHO_PIN_back, INPUT);

  myservo.attach(servoPin);
  
  
  //Buzzer
  pinMode(buzzerPin, OUTPUT);  
  

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
  //timer.every(500000, distanceReadBack);
  //timer.every(100000, servoWrite);
  //timer.every(100000, speedControl);
  //timer.every(1, distanceRead,TRIGGER_PIN_front,ECHO_PIN_front,"b");
  
}

void loop(){ 
  timer.tick();
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
      controlString = inputString;
      controlString.remove(inputString.length()-1);
      controlString.remove(inputString.length()-1);
      if (inputString.charAt(0) == 's') {
        servoWrite(controlString);
        controlString = "";
      }
      else if (inputString.charAt(0) == 'm') {
        speedControl(controlString);
        controlString = "";
      }
      else if (inputString.charAt(0) == 'b') {
        buzzer(controlString);
        controlString = "";
      }      
    }
  }
}
