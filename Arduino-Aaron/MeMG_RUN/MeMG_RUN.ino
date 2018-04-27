#include "TelstraWeb.h"
#include "connection4g.h"
#include "TelstraM1Device.h"
#include "TelstraM1Interface.h"
#include "azureIoT.h"
#include "jsonGenerator.h"
#include "MeMG.h"

#define SECURE_CONNECTION true
#define INSECURE_CONNECTION false
#define BUTTON 7

const char host[] = "laika-hub.azure-devices.net";
const char path[] = "/devices/memg/messages/events?api-version=2016-02-03";
const int port = 443;

// Create MeMG Object from MeMG.h
MeMG MeMG;

void setup() {
  MeMG.init();
  Serial.begin(115200);
  digitalWrite(BUTTON, HIGH);
}

void loop() {
  while(digitalRead(BUTTON));

  MeMG.uploadToAzure(host,path,port);

  while(1);
}


void serialFlush(){
  while(Serial.available() > 0) {
    Serial.read();
  }
}




