// this sketch cycles three servos at different rates 
#include "NewPing.h"
#include <ServoTimer2.h>  // the servo library
#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/UInt8.h>
#define servoPin  3

#define TRIGGER_PIN_front A1
#define ECHO_PIN_front A0
#define TRIGGER_PIN_back A3
#define ECHO_PIN_back A2

#define MAX_DISTANCE 400

ServoTimer2 servoRoll;    // declare variables for up to eight servos
uint16_t servo_pos= 1350;


//Ultrasonic connections
//const int TRIGGER_PIN_front = A1;
//const int ECHO_PIN_front = A0;

//const int TRIGGER_PIN_back = A3;
//const int ECHO_PIN_back = A2;
NewPing sonar_front(TRIGGER_PIN_front, ECHO_PIN_front, MAX_DISTANCE);
NewPing sonar_back(TRIGGER_PIN_back, ECHO_PIN_back, MAX_DISTANCE);


// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 10;
int in3 = 5;
int in4 = 4;


// Control Values Motor
enum Control {forward=0,backward,AforwardBbackward,BforwardAbackward};
Control control_motor = forward;
uint16_t a_vel = 0; 
uint16_t b_vel = 0;


//Buzzer
int buzzerPin = 11;
uint16_t buzzer_active = 0;



//Functions 
void directionControl(uint16_t a_vel, uint16_t b_vel,Control control) {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, a_vel); //210 max here
  analogWrite(enB, b_vel); //255 max here

  switch(control){
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
/*
float distanceRead(int TRIGGER_PIN,int ECHO_PIN){
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(1000.0);//Best delay 1000
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  float distance = (duration/ 2.0) / 29.0;
  return distance;
  }
*/

void subscriberCallbackMotor(std_msgs::Int16MultiArray& actuation_msg) {
  a_vel = actuation_msg.data[0];
  b_vel = actuation_msg.data[1];
  control_motor = (Control) actuation_msg.data[2];
  directionControl(a_vel,b_vel,control_motor);
}


void subscriberCallbackServo(std_msgs::UInt16& servo_msg) {
  servo_pos = servo_msg.data;
  
  if(servo_pos > 2000){
    servo_pos = 2000; 
  }
  else if(servo_pos <700){
    servo_pos = 700;
  }
  
  servoRoll.write(servo_pos);
}


void subscriberCallbackBuzzer(std_msgs::UInt8&  buzzer_msg) {
  buzzer_active = buzzer_msg.data;
  if(buzzer_active == 1){
  digitalWrite(buzzerPin, HIGH);
  }
  else{digitalWrite(buzzerPin, LOW);}
}





ros::NodeHandle node_handle;

std_msgs::Float32 distanceReading_front_output;
std_msgs::Float32 distanceReading_back_output;
std_msgs::UInt16 servo_input;
std_msgs::Int16MultiArray motor_input[3];
std_msgs::UInt8 buzzer_input;



ros::Publisher distanceReading_front_publisher("distanceReading_front_topic", &distanceReading_front_output);
ros::Publisher distanceReading_back_publisher("distanceReading_back_topic", &distanceReading_back_output);
ros::Subscriber<std_msgs::Int16MultiArray> motor_subscriber("motor_topic", &subscriberCallbackMotor);
ros::Subscriber<std_msgs::UInt16> servo_subscriber("servo_topic", &subscriberCallbackServo);
ros::Subscriber<std_msgs::UInt8> buzzer_subscriber("buzzer_topic", &subscriberCallbackBuzzer);

void setup()
{
  node_handle.getHardware()->setBaud(9600);
  node_handle.initNode();
  node_handle.advertise(distanceReading_back_publisher);
  node_handle.advertise(distanceReading_front_publisher);
  node_handle.subscribe(motor_subscriber);
  node_handle.subscribe(servo_subscriber);
  node_handle.subscribe(buzzer_subscriber);
  

  //servo
  servoRoll.attach(servoPin); 

  //Ultrasonic sensors
  pinMode (TRIGGER_PIN_front, OUTPUT);
  pinMode (ECHO_PIN_front, INPUT);
  pinMode (TRIGGER_PIN_back, OUTPUT);
  pinMode (ECHO_PIN_back, INPUT);

  
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
  
}

void loop()
{ 
  distanceReading_front_output.data = sonar_front.ping_cm();
  distanceReading_back_output.data = sonar_back.ping_cm();
  distanceReading_front_publisher.publish(&distanceReading_front_output);
  distanceReading_back_publisher.publish(&distanceReading_back_output);

  node_handle.spinOnce();
  
}
