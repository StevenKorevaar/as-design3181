#include <ArduinoJson.h>

int i = 0;


String makeJsonData(String send_data){ 
  String content = "";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();      // Create the root of the object tree.

  JsonArray& jsonAr = jsonOb.createNestedArray("EMG"); // Create field for EMG data
  jsonAr.add(send_data);                               // Insert encoded string
  jsonOb.printTo(content);                             // Create JSON

return content;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
//Serial.println(sin(0.1*i));

Serial.println(makeJsonData((String(sin(0.1*i), 2))));
delay(100);
i++;

}



