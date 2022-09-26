#include <ArduinoJson.h>

DynamicJsonDocument doc(1024);

void setup() {

  doc["sensor"] = "gps";
  doc["time"]   = 1351824120;
  doc["data"][0] = 48.756080;
  doc["data"][1] = 2.302038;

  
  Serial.begin(115200);
  
}
void loop() {
  serializeJson(doc, Serial);
  delay(10000);
}
