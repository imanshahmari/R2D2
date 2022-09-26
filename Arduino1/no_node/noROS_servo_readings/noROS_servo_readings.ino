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


//Buzzer
//int buzzerPin = 11;
//uint16_t buzzer_active = 0;


String myStringFront;
String myStringBack;

int MAX_DISTANCE = 100;

NewPing sonar_front(TRIGGER_PIN_front, ECHO_PIN_front, MAX_DISTANCE);
NewPing sonar_back(TRIGGER_PIN_back, ECHO_PIN_back, MAX_DISTANCE);


//Functions 
void distanceReadFront(){
  float result = sonar_front.ping_cm();
  String myString = "f" + String(result);
  Serial.println(myString);
  }
void distanceReadBack(){
  float result = sonar_back.ping_cm();
  String myString = "b" + String(result);
  Serial.println(myString);
  }

void servoWrite(){
  String my_word = Serial.readString();
  String commandChar = my_word.substring(0,1);
  String value = my_word.substring(1);
  if(commandChar == "s"){
    //String value = my_word.substring(1);
    myservo.write(commandChar.toInt());
    Serial.println(commandChar.toInt());
  }
  Serial.println(my_word);
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

  myservo.attach(3);
  
  
  //Buzzer
  //pinMode(buzzerPin, OUTPUT);  
  

  timer.every(3000000, distanceReadFront);
  timer.every(3000000, distanceReadBack);
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
