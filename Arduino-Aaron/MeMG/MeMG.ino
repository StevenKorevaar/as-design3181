#include "TelstraWeb.h"
#include "connection4g.h"
#include "TelstraM1Device.h"
#include "TelstraM1Interface.h"
#include "azureIoT.h"
#include "jsonGenerator.h"
#include <elapsedMillis.h>
#include "OLED.h"
#include "EMGFilters.h"


#define SECURE_CONNECTION true
#define INSECURE_CONNECTION false
#define BUTTON 7
#define ARRAY_SIZE 4000
#define PACKET_SIZE 200
#define NUMBER_OF_PACKETS 20
#define SENSOR_PIN A0



// EMG Sensor
#define _DEBUG      0
#define CALIBRATE 0
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_1000HZ;

int scaling = 240;
int baseline = 700;


int sensorValue = 0;  // variable to store the value coming from the sensor
short store[ARRAY_SIZE];

char host[] = "laika-hub.azure-devices.net";
char path[] = "/devices/memg/messages/events?api-version=2016-02-03";
int port = 443;
//--Unsecure---

elapsedMillis timeElapsed;

TelstraM1Interface commsif;
TelstraM1Device IoTDevice(&commsif);
Connection4G conn(SECURE_CONNECTION,&commsif);
TelstraWeb WebIoT(&conn,&IoTDevice);
AzureIoT myAzure("laika-hub.azure-devices.net", "memg","nwcgBl1BfWyODhQ9Kt30v3cBJdYXEcUDxsIzgH9w1KQ=",1000);
OLED OLED;
EMGFilters myFilter;


void setup() {
  //delay(2000);
  commsif.begin(); // Must include for proper SPI communication
  //conn.closeTCP();
  myAzure.init(&IoTDevice);
  Serial.begin(115200);
  digitalWrite(BUTTON, HIGH);

  myFilter.init(sampleRate, humFreq, true, true, true);

  //Initialize the OLED controller
  OLED.begin();
  OLED.fill_OLED(0x00,0x00,0x00); // Clear screen
  delay(10);

  OLED.fill_OLED(0xFF,0xFF,0xFF); 
  OLED.drawImage(logo, 50, 3, 28, 31); // Draw Teltra logo to screen

  delay(5000);
  OLED.fill_OLED(0x00,0x00,0x00);
}

void loop() {
//  OLED.fill_OLED(0x00,0x00,0x00);
//  OLED.drawString("Press Button", 15, 15, 255, 255, 255,1);
  Serial.println("Exercise now!");
  while(digitalRead(BUTTON));
  
//  OLED.fill_OLED(0x00,0x00,0x00);
//  OLED.drawString("Collecting Data", 15, 15, 255, 255, 255,1);

  for(int i =0; i < ARRAY_SIZE;i++){
    store[i] = EMGread();
  }

//  OLED.fill_OLED(0x00,0x00,0x00);
//  OLED.drawString("Plot?", 15, 15, 255, 255, 255,1);

  Serial.println("View");

  while(digitalRead(BUTTON));
  
  for(int i =0; i < 4000;i++){
    delay(1);
    Serial.println(store[i]);
    Serial.print(" ");    
  }
//  OLED.fill_OLED(0x00,0x00,0x00);
//  OLED.drawString("Upload?", 15, 15, 255, 255, 255,1);


  Serial.println("Send?");
  while(digitalRead(BUTTON));

//  OLED.fill_OLED(0x00,0x00,0x00);
//  OLED.drawString("Connecting...", 15, 15, 255, 255, 255,1);
  
  Serial.println("Waiting until cellular system has finished booting...");
  IoTDevice.waitUntilCellularSystemIsReady();
  delay(3000);
  Serial.println(" Opening TCP connection!");

  WebIoT.setHost(host,path,port);
  
 if(conn.openTCP(host,port)==CONNECTION4G_STATUS_OK)
  {
    OLED.fill_OLED(0x00,0x00,0x00);
    OLED.drawString("Success!", 15, 15, 255, 255, 255,1);
    Serial.println(" Success!");
  
    delay(1000);
    
   ///////// JSON Generator /////////
   
   OLED.fill_OLED(0x00,0x00,0x00);
   OLED.drawString("Uploading...", 15, 15, 255, 255, 255,1);

   for(int i = 0; i<NUMBER_OF_PACKETS;i++){
   
   myAzure.setPostContent(makeJsonData(i)); 

   // Post to Azure
   WebIoT.post(myAzure.getPostPacket());
   Serial.print("Packet ");
   Serial.print(i);
   Serial.println(" Sent");                     
   }

   OLED.fill_OLED(0x00,0x00,0x00);
   OLED.drawString("Great Job :)", 15, 15, 255, 255, 255,1); 
   conn.closeTCP();

   } else {
     Serial.println(" OpenTCP() failed.");
   }

   while(1);
}


void serialFlush(){
  while(Serial.available() > 0) {
    Serial.read();
  }
}





String makeJsonData(int packet_number)
{
  String content = "";
  String temp = " ";
  String temp2 = " ";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();    // Create the root of the object tree.

  JsonArray& jsonAr = jsonOb.createNestedArray("Aaron");
  
  for(int i = 0;i < PACKET_SIZE;i++){

      temp2 = String(store[packet_number*PACKET_SIZE+i],HEX);
      
      if(temp2.length()< 3)
      {
        if(temp2.length()<2){
          temp2 = "00" + temp2;
        }
        else{
          temp2 = "0" + temp2; 
        }
      }
      
      if(i == 0){temp = temp2;}
      else {temp = temp + temp2;}
  }
  
  jsonAr.add(temp);  
  jsonOb.printTo(content);    //create JSON

return content;
}





short EMGread(){

  unsigned long timeStamp = micros();

    int data = analogRead(SENSOR_PIN);

    // filter processing
    int dataAfterFilter = myFilter.update(data);

    // Get envelope by squaring the input
    int envelope = sq(dataAfterFilter);

    if (CALIBRATE) {
        Serial.print("Squared Data: ");
        Serial.println(envelope);
    }
    else {
        // Any value below the `baseline` value will be treated as zero
        if (envelope < baseline) {
            dataAfterFilter = 0;
            envelope = 0;
        }
        // You may plot the data using Arduino SerialPlotter.

        envelope = envelope / scaling;
        
    }

    // Usually, you should still have (interval - timeElapsed) to do other work.
    // Otherwise, you would have to lower down the `sampleRate`.
    //unsigned long timeElapsed = micros() - timeStamp;
#if _DEBUG
    Serial.print("Filters cost time: ");
    //Serial.println(timeElapsed);
#else
    delayMicroseconds(410);
#endif

  if(envelope > 1023){envelope = 1023;}
  
  return (short) envelope;
}

