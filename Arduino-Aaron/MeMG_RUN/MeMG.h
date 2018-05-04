//
//  MeMG.h
//  Library to control MeMG Device Functions 
//  by Laika, Telstra Innovation Challenge
//
//  Created by Tim Scott, Laika on 26/04/2018.
//

#ifndef MeMG_h
#define MeMG_h

#include <Adafruit_NeoPixel.h>
#include <TM1637Display.h>
#include "TelstraWeb.h"
#include "connection4g.h"
#include "TelstraM1Device.h"
#include "TelstraM1Interface.h"
#include "azureIoT.h"
#include "jsonGenerator.h"
#include "OLED.h"
#include "EMGFilters.h"

#define SECURE_CONNECTION true
#define INSECURE_CONNECTION false

#define PACKET_SIZE 300
#define ARRAY_SIZE 3900
#define NUM_PACKETS 13

#define BASELINE 1000
#define SCALING 120

//  Neopixel LED setup ______________________________________
//  Neopixel 0 : EMG threshold indicator
//  Neopixel 1 : Flex extension indicator

#define PIN 11 // Pin controlling NEOPIXELS
#define NUMPIXELS 2  // Number of NEOPIXELS used

// 7 segment Display Setup ___________________________________
#define CLK 9  //Set the CLK pin connection to the Rep Counter Display
#define DIO 8  //Set the DIO pin connection to the Rep Counter Display

class MeMG 
{
    public:

    // Constructor creates nested objects for device connectivity + EMG filter library
    MeMG() : 
      commsif(),
      IoTDevice(&commsif),
      conn(SECURE_CONNECTION,&commsif),
      WebIoT(&conn,&IoTDevice),
      myAzure("laika-hub.azure-devices.net", "memg","nwcgBl1BfWyODhQ9Kt30v3cBJdYXEcUDxsIzgH9w1KQ=",1000),
      Screen(),
      EMGFilter(),
      pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800),
      RepCounter(CLK,DIO){}

    
    void init();
    void uploadToAzure(const char* _host, const char* _path ,const int _port, short * store, int num_samples); 
    short read_EMG(int pin);
    short read_Flex(int pin);
    void set_Pixels(int state);
    void display_Rep(int count);
    void OLED_Write(String input);
    
    
    private:
    
    TelstraM1Interface commsif;
    TelstraM1Device IoTDevice;
    Connection4G conn;
    TelstraWeb WebIoT;
    AzureIoT myAzure;

    Adafruit_NeoPixel pixels;
    TM1637Display RepCounter;
    OLED Screen;
    
    EMGFilters EMGFilter;
    NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
    SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;

    const char* host;
    const char* path;
    int port;

    String makeJsonData(String send_data);
    String encode(short data[],int num_elements);
    
};




#endif
