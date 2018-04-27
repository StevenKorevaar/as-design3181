//
//  MeMG.h
//  Library to control MeMG Device Functions 
//  by Laika, Telstra Innovation Challenge
//
//  Created by Tim Scott, Laika on 26/04/2018.
//


#ifndef MeMG_h
#define MeMG_h

#include "TelstraWeb.h"
#include "connection4g.h"
#include "TelstraM1Device.h"
#include "TelstraM1Interface.h"
#include "azureIoT.h"
#include "jsonGenerator.h"
//#include "OLED.h"
//#include "EMGFilters.h"

#define SECURE_CONNECTION true
#define INSECURE_CONNECTION false
#define PACKET_SIZE 200



class MeMG 
{
    public:
    
    MeMG() : 
      commsif(),
      IoTDevice(&commsif),
      conn(SECURE_CONNECTION,&commsif),
      WebIoT(&conn,&IoTDevice),
      myAzure("laika-hub.azure-devices.net", "memg","nwcgBl1BfWyODhQ9Kt30v3cBJdYXEcUDxsIzgH9w1KQ=",1000){}
    
    void init();
    void uploadToAzure(const char* _host, const char* _path ,const int _port); 
    void read_EMG();
    
    
    private:
    
    TelstraM1Interface commsif;
    TelstraM1Device IoTDevice;
    Connection4G conn;
    TelstraWeb WebIoT;
    AzureIoT myAzure;

    int store [PACKET_SIZE];
    const char* host;
    const char* path;
    int port;

    String makeJsonData(int packet_number);

    //OLED OLED;
    //EMGFilters myFilter;

    
    
};




#endif
