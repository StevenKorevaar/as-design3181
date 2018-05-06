//
//  MeMG.cpp
//  
//  Created by Team Laika on 26/04/2018.
//
//

#include "MeMG.h"


void MeMG::init(){
    // OLED Start
    screen.begin();
    delay(10);
    update_OLED(LOGO);
    
    //  Pixels -> Pink
    pixels.begin(); //pixel init
    delay(10);
    set_Pixels(MEMG_IDLE);

    // SPI comm start
    commsif.begin();

    // Azure config 
    myAzure.init(&IoTDevice);

    //  Define EMG filter parameters
    EMGFilter.init(sampleRate, humFreq, true, true, true);  

    //  Set Rep counter brightness -> MAX
    RepCounter.setBrightness(0x0a);  
  
    //  Print Battery Status
    reportln(2,"Battery Status: ");
    reportln(2,IoTDevice.getBatteryStatus());
    reportln(2,"Battery State of Charge: ");
    reportln(2,IoTDevice.getBatteryStateOfCharge());
}

void MeMG::uploadToAzure(const char* _host, const char* _path ,const int _port, short store[], int num_samples){

    // This function handles cellular system boot
    // TCP connection to Azure
    // Encoding EMG and converting to JSON format
    // Forming HTTP post Packets
    // & batch upload to Azure
    
    host = _host;
    path = _path;
    port = _port;

    screen.fill_OLED(0,0,0); 
    screen.drawString("Connecting...",10, 10, R,G,B,1);
    
    reportln(1,"Waiting until cellular system has finished booting...");
    IoTDevice.waitUntilCellularSystemIsReady();
    delay(3000);
    reportln(1," Opening TCP connection!");

    WebIoT.setHost(host,path,port);
    
    if(conn.openTCP(host,port)==CONNECTION4G_STATUS_OK)
    {
        screen.drawString("Connecting...  OK",10, 10, R,G,B,1);
        reportln(1," Success!");
        delay(500);

        screen.fill_OLED(0,0,0);   
        screen.drawString("Uploading...",10, 10, R,G,B,1);
        int x = 0; // screen index

        for(int packet_num = 0; packet_num< NUM_PACKETS;packet_num++){

            //Loading Bar
            x = x + 128/NUM_PACKETS;
            screen.fillRect(x, 0, 128/NUM_PACKETS, 5, R,G,B);    

            myAzure.setPostContent(makeJsonData(encode(&store[PACKET_SIZE*packet_num],PACKET_SIZE)));
            WebIoT.post(myAzure.getPostPacket());
        }
        
        // Post to Azure
        
        screen.drawString("Complete",10, 20, R,G,B,1);
        conn.closeTCP();
        
    } else {
        screen.drawString("Connecting...  FAILED",10, 10, R,G,B,1);
        reportln(1," OpenTCP() failed.");
    }
}

short MeMG::read_EMG(int pin){

int data = analogRead(pin);

    // Filter EMG Data
    int dataAfterFilter = EMGFilter.update(data);

    // Get envelope by squaring the input
    int envelope = sq(dataAfterFilter);

    // Any value below the `baseline` value will be treated as zero
    if (envelope < BASELINE) {
        envelope = 0;
    }

    // Scale down values
    envelope = envelope / SCALING;  

    //Clip signal at ceiling
    if(envelope > 1023){envelope = 1023;}
  
  return (short)envelope;
}

short MeMG::read_Flex(int pin){
  return analogRead(pin);
}

void MeMG::set_Pixels(PIXEL_STATES state){

  // Switch statement to control different states of Neopixel indicator lights
  switch(state) {
    case 0:
      pixels.setPixelColor(0, pixels.Color(30,0,30));   // LED 1 to dull pink
      pixels.setPixelColor(1, pixels.Color(30,0,30));   // LED 2 to dull pink
      pixels.show();
      break;
    case 1:
      pixels.setPixelColor(0, pixels.Color(30,0,30));   // LED 1 to dull pink
      pixels.setPixelColor(1, pixels.Color(30,0,30));   // LED 2 to dull pink
      pixels.show();
      break;
    case 2:
      pixels.setPixelColor(0, pixels.Color(0,100,0));   // LED 1 to GREEN
      pixels.show();
      break;
    case 3:
      pixels.setPixelColor(1, pixels.Color(0,0,100));   // LED 2 to BLUE
      pixels.show();
      break;
  }
  
}

void MeMG::update_OLED(OLED_STATES state){

  // Switch statement to display different screens on the OLED
  switch(state) {
    case 0:
      // Draw MeMG Logo to Screen
      screen.fill_OLED(R,G,B);
      screen.drawImage(memg_logo, 15, 10, 29, 14); 
      screen.drawString("Me-MG", 55, 10,255,255,255,2,R,G,B);
      break;
    case 1:
      screen.fill_OLED(R,G,B); 
      screen.drawString("< Start", 5, 10,255,255,255,2,R,G,B);
      break;
    case 2:
      screen.fill_OLED(0,0,0); 
      screen.drawString("Begin Exercise",10, 10, R,G,B,1);
      break;
    case 3:
      screen.fill_OLED(0,0,0);
      screen.drawString("Within Rep _",10,5,R,G,B,1); 
      screen.drawString("Extension  : ",10, 15, R,G,B,1);
      screen.drawString("Activation : ",10, 25, R,G,B,1);  
      break;
    case 4:
      screen.drawString("GOOD!",85, 15, R,G,B,1);
      break;
    case 5:
      screen.drawString("GOOD!",85, 25, R,G,B,1);
      break;
    case 6:
      screen.fill_OLED(0,0,0); 
      screen.drawString("Rep Complete",10, 10, R,G,B,1);
      break;
    case 7:
      screen.fill_OLED(0,0,0);
      screen.drawString("< Push to Save",10, 10, R,G,B,1);
      break;
    case 8:
      screen.fill_OLED(0,0,0);
      screen.drawString("< Push to Upload",10, 10, R,G,B,1);
      break;
    case 9:
      screen.fill_OLED(0,0,0);
      screen.drawString("Nice.",10, 10, R,G,B,2);
      break;
  }
}

void MeMG::display_Rep(int count){
  RepCounter.showNumberDec(count); // rep count -> 7 segment display 
}

String MeMG::makeJsonData(String send_data)
{ 
  String content = "";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();      // Create the root of the object tree.

  JsonArray& jsonAr = jsonOb.createNestedArray("EMG"); // Create field for EMG data
  jsonAr.add(send_data);                               // Insert encoded string
  jsonOb.printTo(content);                             // Create JSON

return content;
}


String MeMG::encode(short data[],int num_elements){

  // Encodes data to 2 char string format

  String dataStr = "";

  // List of valid characters we can use for encoding
  char validChars[] = {
          '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 
          'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
          'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B',
          'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
          'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '#',  
          '%', '&', '(', ')', '*', '+', ',', '-', '.', '/', ':', ';', '<',  
          '=', '>', '?', '[', ']', '^', '_', '{', '|', '}', '~'};

  // loop through all data values in the data list
  for(int n = 0; n < num_elements; n++) {
    
    // get the current data value
    short value = data[n];
    // create a result string
    String result = "";

    // zero Handling
    if(value == 0) {
      result = "00";
    }
    
    // Loop while value is greater than 0
    while (value) {
      // find the value of the current digit
      short rem = value % sizeof(validChars);
      // find the value after this digit has been added
      value = floor(value / sizeof(validChars));
      // add current digit to the result
      result += validChars[rem];
    }

    // If 1 char pad, Else 2 chars swap for correct order
    if( result.length() == 1 ) {
          result = "0" + result;
    }
    else{
       String copy = result;
       result.setCharAt(0,copy.charAt(1));
       result.setCharAt(1,copy.charAt(0));
    }
    dataStr += result;      
    }
    return dataStr;
}




