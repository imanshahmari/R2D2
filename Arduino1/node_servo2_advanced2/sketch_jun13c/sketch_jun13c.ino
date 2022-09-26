// this sketch cycles three servos at different rates 
#include <ros.h>
#include <std_msgs/Float32.h>


//Ultrasonic connections
const int TRIGGER_PIN_front = A1;
const int ECHO_PIN_front = A0;
//const int TRIGGER_PIN_back = A3;
//const int ECHO_PIN_back = A2;
float duration;



float distanceRead(int TRIGGER_PIN,int ECHO_PIN){
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(1000.0);//Best delay 1000
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  float distance = (duration/ 2.0) / 29.0;
  return distance;
  }



ros::NodeHandle node_handle;

std_msgs::Float32 distanceReading_front_output;
//std_msgs::Float32 distanceReading_back_output;



ros::Publisher distanceReading_front_publisher("distanceReading_front_topic", &distanceReading_front_output);
//ros::Publisher distanceReading_back_publisher("distanceReading_back_topic", &distanceReading_front_output);

void setup()
{
  node_handle.getHardware()->setBaud(115200);
  node_handle.initNode();
  //node_handle.advertise(distanceReading_back_publisher);
  node_handle.advertise(distanceReading_front_publisher);
}

void loop()
{ 
  distanceReading_front_output.data = distanceRead(TRIGGER_PIN_front,ECHO_PIN_front)+1000.0;
  //distanceReading_back_output.data = distanceRead(TRIGGER_PIN_back,ECHO_PIN_back)+2000.0;
  distanceReading_front_publisher.publish(&distanceReading_front_output);
  //distanceReading_front_publisher.publish(&distanceReading_back_output);
  

  node_handle.spinOnce();
  
}
