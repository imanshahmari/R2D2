#include <ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <std_msgs/UInt16.h>
#include <std_msgs/Int16MultiArray.h>
#include <Servo.h>

//Ultrasonic connections
const int TRIGGER_PIN = A1;
const int ECHO_PIN = A0;

// Motor B connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor A connections
int enB = 10;
int in3 = 5;
int in4 = 4;


// Control Values Motor
enum Control {forward=0,backward,AforwardBbackward,BforwardAbackward};

Control control_motor = forward;
uint16_t a_vel = 0; 
uint16_t b_vel = 0;


//Servo
uint16_t servo_pos = 75; 
Servo myservo; 

//Buzzer
//int buzzerPin = 11;
//uint16_t buzzer_active = 0;



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
}


void subscriberCallbackServo(std_msgs::UInt16& servo_msg) {
  servo_pos = servo_msg.data;
  if(servo_pos > 145){
    servo_pos = 145; 
  }
  else if(servo_pos < 40){
    servo_pos = 40;
  }
  myservo.write(servo_pos);
  //buzzer_active = actuation_msg.data[4];
}









ros::NodeHandle node_handle;


std_msgs::Bool lineTracking_output;
std_msgs::Float32 distanceReading_output;
std_msgs::UInt16 servo_input;
std_msgs::Int16MultiArray actuation_input;



//ros::Publisher lineTracking_publisher("lineTracking_topic", &lineTracking_output);
ros::Publisher distanceReading_publisher("distanceReading_topic", &distanceReading_output);
ros::Subscriber<std_msgs::Int16MultiArray> actuation_subscriber("actuation_topic", &subscriberCallbackMotor);
ros::Subscriber<std_msgs::UInt16> servo_subscriber("servo_topic", &subscriberCallbackServo);

void setup()
{
  node_handle.initNode();
  //node_handle.advertise(lineTracking_publisher);
  node_handle.advertise(distanceReading_publisher);
  node_handle.subscribe(actuation_subscriber);
  node_handle.subscribe(servo_subscriber);

  myservo.attach(3);

  //pinMode(buzzerPin, OUTPUT);
  
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
  unsigned long duration;
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(100.0);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distanceReading_output.data = (duration / 2.0) / 29.0;
  //lineTracking_output.data = digitalRead(4);
  
  //lineTracking_publisher.publish(&lineTracking_output);
  distanceReading_publisher.publish(&distanceReading_output);

  //if(buzzer_active == 1){
  //  digitalWrite(buzzerPin, HIGH);
  //}
  //else{digitalWrite(buzzerPin, LOW);}
  
  node_handle.spinOnce();
  
}
