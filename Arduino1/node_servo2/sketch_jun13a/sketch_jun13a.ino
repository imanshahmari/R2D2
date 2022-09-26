// this sketch cycles three servos at different rates 

#include <ServoTimer2.h>  // the servo library
#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16MultiArray.h>
#include <std_msgs/UInt16.h>
#define rollPin  3



ServoTimer2 servoRoll;    // declare variables for up to eight servos
uint16_t servo_pos= 1350;


//Ultrasonic connections
const int TRIGGER_PIN = A1;
const int ECHO_PIN = A0;
unsigned long duration;

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
      digitalWrite(in4, LOW);;  
    break;
    
    case backward:
      // Turn on motor A 
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Turn on motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);;
    break;
    
    case AforwardBbackward:
      // Turn on motor A 
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      // Turn on motor B
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);;  
    break;
    
    case BforwardAbackward:
      // Turn on motor A 
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      // Turn on motor B
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);;  
    break;
  }
}



void subscriberCallbackMotor(std_msgs::Int16MultiArray& actuation_msg) {
  a_vel = actuation_msg.data[0];
  b_vel = actuation_msg.data[1];
  control_motor = (Control) actuation_msg.data[2];
  directionControl(a_vel,b_vel,control_motor);
  servo_pos = actuation_msg.data[2];
  servoRoll.write(servo_pos);
}






ros::NodeHandle node_handle;


std_msgs::Float32 distanceReading_output;
std_msgs::Int16MultiArray motor_input;



ros::Publisher distanceReading_publisher("distanceReading_topic", &distanceReading_output);
ros::Subscriber<std_msgs::Int16MultiArray> motor_subscriber("motor_topic", &subscriberCallbackMotor);

void setup()
{
  node_handle.initNode();
  
  node_handle.advertise(distanceReading_publisher);
  node_handle.subscribe(motor_subscriber);

  servoRoll.attach(rollPin); 
  
  pinMode (TRIGGER_PIN, OUTPUT);
  pinMode (ECHO_PIN, INPUT);

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
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(100.0);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distanceReading_output.data = (duration / 2.0) / 29.0;


  distanceReading_publisher.publish(&distanceReading_output);


  distanceReading_publisher.publish(&distanceReading_output);
  
  
  node_handle.spinOnce();
}
