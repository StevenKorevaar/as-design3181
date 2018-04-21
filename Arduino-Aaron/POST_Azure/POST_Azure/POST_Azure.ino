#include "TelstraWeb.h"
#include "connection4g.h"
#include "TelstraM1Device.h"
#include "TelstraM1Interface.h"
#include "azureIoT.h"
#include "jsonGenerator.h"


#define SECURE_CONNECTION true
#define INSECURE_CONNECTION false


char host[] = "laika.azure-devices.net";
char path[] = "/devices/memg/messages/events?api-version=2016-02-03";
int port = 443;
//--Unsecure---

TelstraM1Interface commsif;
TelstraM1Device IoTDevice(&commsif);
Connection4G conn(SECURE_CONNECTION,&commsif);
TelstraWeb WebIoT(&conn,&IoTDevice);
AzureIoT myAzure("laika.azure-devices.net", "memg","n+IgWUvlHKlhJ+o+La1quOqsgdCG60hgCcQWLK2OgS0=",604800);
JsonGenerator myJson((BATTERY_STATUS | BATTERY_STATE_OF_CHARGE | TEMPARATURE | LIGHT_LEVEL), 1); // Use Timer1 library to interrupt every time interval

void setup() {
  //delay(2000);
  commsif.begin(); // Must include for proper SPI communication
  conn.closeTCP();
  myAzure.init(&IoTDevice);
  Serial.begin(115200);
  
}

void loop() {
while(Serial.available()==0);
  serialFlush();

  Serial.println("Waiting until cellular system has finished booting...");
  IoTDevice.waitUntilCellularSystemIsReady();

  Serial.println(" Opening TCP connection!");

  WebIoT.setHost(host,path,port);
 if(conn.openTCP(host,port)==CONNECTION4G_STATUS_OK)
  {
     Serial.println(" Success!");
  
    delay(1000);
    

 ///////// JSON Generator /////////

 myJson.collectSensorData(&IoTDevice);

 myAzure.setPostContent(myJson.printJson());
 

    // Build HTTPS request.
    
    Serial.println(myAzure.getPostPacket());
    //Serial.println();
    WebIoT.post(myAzure.getPostPacket());
    
    delay(2000);
    conn.closeTCP();

    } else {
     Serial.println(" OpenTCP() failed.");
  }
}




void serialFlush(){
  while(Serial.available() > 0) {
    Serial.read();
  }
}
