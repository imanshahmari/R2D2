
int Led = 12 ;// define LED Interface
int buttonpin = 7; // define D0 Sensor Interface
int val = 0;// define numeric variables val
void setup ()
{
 pinMode (Led, OUTPUT) ;// define LED as output interface
 pinMode (buttonpin, INPUT) ;// output interface D0 is defined sensor
}
void loop ()
{
 val = digitalRead(buttonpin);//
 if (val == HIGH) //
 {
 digitalWrite (Led, HIGH);
 }
 else
 {
 digitalWrite (Led, LOW);
 }
}
