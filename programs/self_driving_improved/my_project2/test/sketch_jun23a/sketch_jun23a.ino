// this sketch cycles three servos at different rates 

String inputString = "";// a String to hold incoming data
String controlString = "";
bool stringComplete = false; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }

}

void servoWrite(String control){
  int servoValue = control.substring(1).toInt();
    Serial.println(servoValue);
}


void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
      controlString = inputString;
      controlString.remove(inputString.length()-1);
      if (inputString.charAt(0) == 'b') {
        servoWrite(controlString);
        controlString = "";
      }
    }
  }
}
