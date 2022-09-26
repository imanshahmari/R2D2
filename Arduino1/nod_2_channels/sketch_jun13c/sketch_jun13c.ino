// this sketch cycles three servos at different rates 

#include <ServoTimer2.h>  // the servo library
#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#define servoPin  3



ServoTimer2 servoRoll;    // declare variables for up to eight servos
uint16_t servo_pos= 1350;


//Ultrasonic connections
const int TRIGGER_PIN_front = A1;
const int ECHO_PIN_front = A0;
//const int TRIGGER_PIN_back = A3;
//const int ECHO_PIN_back = A2;
float duration;

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

float distanceRead(int TRIGGER_PIN,int ECHO_PIN){
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(1000.0);//Best delay 1000
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  float distance = (duration/ 2.0) / 29.0;
  return distance;
  }


void subscriberCallbackServo(std_msgs::Int16& actuation_msg) {

  if (0<=actuation_msg.data <= 1000){
    a_vel = actuation_msg.data;
    analogWrite(enA, a_vel); //210 max here
  }
  else if (1000 > actuation_msg.data && actuation_msg.data < 2000 ){
    b_vel = actuation_msg.data - 1000;
    analogWrite(enB, b_vel); //210 max here
  }
  else if (2000 >= actuation_msg.data && actuation_msg.data < 3000 ){
    servo_pos = actuation_msg.data - 2000;
    if(servo_pos > 2000){
      servo_pos = 2000; 
    }
    else if(servo_pos <700){
      servo_pos = 700;
    }
    servoRoll.write(servo_pos);
  }
  else if (actuation_msg.data == -1){
    // Turn on motor B
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Turn on motor A
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    
  }
  else if (actuation_msg.data == -2){
    // Turn on motor A 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Turn on motor B
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
  }
  else if (actuation_msg.data == -3){
    // Turn on motor A 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Turn on motor B
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    
  }
  else if (actuation_msg.data == -4){
    // Turn on motor A 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Turn on motor B
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);  
  }

  else if (actuation_msg.data == -5){
    buzzer_active = actuation_msg.data;
    if(buzzer_active == -5){
    digitalWrite(buzzerPin, HIGH);
    }
    else if(buzzer_active == -6){
      digitalWrite(buzzerPin, LOW);} 
  }
  
  
}

ros::NodeHandle node_handle;

std_msgs::Float32 distanceReading_front_output;
std_msgs::Float32 distanceReading_back_output;
std_msgs::Int16 actuation_input;



ros::Publisher distanceReading_publisher("distanceReading_topic", &distanceReading_front_output);
ros::Subscriber<std_msgs::Int16> actuation_subscriber("actuation_topic", &subscriberCallbackServo);

void setup()
{
  node_handle.getHardware()->setBaud(250000);
  node_handle.initNode();
  node_handle.advertise(distanceReading_publisher);
  node_handle.subscribe(actuation_subscriber);
  

  //servo
  servoRoll.attach(servoPin); 

  //Ultrasonic sensors
  pinMode (TRIGGER_PIN_front, OUTPUT);
  pinMode (ECHO_PIN_front, INPUT);
  //pinMode (TRIGGER_PIN_back, OUTPUT);
  //pinMode (ECHO_PIN_back, INPUT);

  
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
  distanceReading_front_output.data = distanceRead(TRIGGER_PIN_front,ECHO_PIN_front)+1000.0;
  //distanceReading_back_output.data = distanceRead(TRIGGER_PIN_back,ECHO_PIN_back)+2000.0;
  distanceReading_publisher.publish(&distanceReading_front_output);
  distanceReading_publisher.publish(&distanceReading_back_output);

  node_handle.spinOnce();
  
}
