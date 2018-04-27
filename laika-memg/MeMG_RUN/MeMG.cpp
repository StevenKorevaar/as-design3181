//
//  MeMG.cpp
//  
//
//  Created by Tim Scott, Laika on 26/04/2018.
//
//

#include "MeMG.h"


void MeMG::init(){
   
    commsif.begin();
    myAzure.init(&IoTDevice);
    //myFilter.init(sampleRate, humFreq, true, true, true);
    
}

void MeMG::uploadToAzure(const char* _host, const char* _path ,const int _port){

    host = _host;
    path = _path;
    port = _port;
    
    Serial.println("Waiting until cellular system has finished booting...");
    IoTDevice.waitUntilCellularSystemIsReady();
    delay(3000);
    Serial.println(" Opening TCP connection!");

    WebIoT.setHost(host,path,port);
    
    if(conn.openTCP(host,port)==CONNECTION4G_STATUS_OK)
    {
        Serial.println(" Success!");
        
        myAzure.setPostContent(makeJsonData(0));

        Serial.println(myAzure.getPostPacket());
        
        // Post to Azure
        WebIoT.post(myAzure.getPostPacket());
        
        conn.closeTCP();
        
    } else {
        Serial.println(" OpenTCP() failed.");
    }
    
}


String MeMG::makeJsonData(int packet_number)
{ 
  String content = "";
  String temp = " ";
  String temp2 = " ";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();    // Create the root of the object tree.

  JsonArray& jsonAr = jsonOb.createNestedArray("Class");
  
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
