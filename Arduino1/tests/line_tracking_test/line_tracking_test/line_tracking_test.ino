void setup() 
{
Serial.begin(9600); // activates Serial Communication
}
 
void loop() 
{
Serial.print(digitalRead(7));
Serial.print("\n");// Line Tracking sensor is connected with pin 8 of the Arduino
delay(100);
}
