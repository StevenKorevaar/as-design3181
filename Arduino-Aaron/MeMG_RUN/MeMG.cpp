//
//  MeMG.cpp
//  
//  Created by Tim Scott, Laika on 26/04/2018.
//
//

#include "MeMG.h"


void MeMG::init(){
   
    commsif.begin();
    myAzure.init(&IoTDevice);
    EMGFilter.init(sampleRate, humFreq, true, true, true);
    
    // Initialise Visuals
    Screen.begin(); // OLED init
    Screen.fill_OLED(0x00,0x00,0x00); // Clear screen
    delay(10);
    Screen.fill_OLED(0xFF,0xFF,0xFF); // Draw White
    Screen.drawImage(logo, 50, 3, 28, 31); // Draw Teltra logo to screen
    
    pixels.begin(); //pixel init
    set_Pixels(0); // Default state (pink)
    RepCounter.setBrightness(0x0a);  //set the diplay to maximum brightness on 7 segment display
    

    //Print Battery Status
    Serial.print("Battery Status: ");
    Serial.println(IoTDevice.getBatteryStatus());
    Serial.print("Battery State of Charge: ");
    Serial.println(IoTDevice.getBatteryStateOfCharge());
  
}

void MeMG::uploadToAzure(const char* _host, const char* _path ,const int _port, short store[], int num_samples){

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

        for(int packet_num = 0; packet_num< NUM_PACKETS;packet_num++){

            myAzure.setPostContent(makeJsonData(encode(&store[PACKET_SIZE*packet_num],PACKET_SIZE)));
            WebIoT.post(myAzure.getPostPacket());
        }
        
        // Post to Azure
        
        conn.closeTCP();
        
    } else {
        Serial.println(" OpenTCP() failed.");
    }
}

short MeMG::read_EMG(int pin){

int data = analogRead(pin);

    // filter processing
    int dataAfterFilter = EMGFilter.update(data);

    // Get envelope by squaring the input
    int envelope = sq(dataAfterFilter);

    // Any value below the `baseline` value will be treated as zero
    if (envelope < BASELINE) {
        envelope = 0;
    }

    // scale down values
    envelope = envelope / SCALING;  

    if(envelope > 1023){envelope = 1023;}
  
  return (short)envelope;
}

short MeMG::read_Flex(int pin){
  return analogRead(pin);
}

void MeMG::set_Pixels(int state){

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

void MeMG::display_Rep(int count){
  RepCounter.showNumberDec(count); // for the digital display 
}

String MeMG::makeJsonData(String send_data)
{ 
  String content = "";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();    // Create the root of the object tree.

  JsonArray& jsonAr = jsonOb.createNestedArray("EMG");
  jsonAr.add(send_data);  
  jsonOb.printTo(content);    //create JSON

return content;
}


String MeMG::encode(short data[],int num_elements){

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
    
    // grab the current data value
    short value = data[n];
    // create a result string
    String result = "";

    //Zero Handling
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

void MeMG::OLED_Write(String input){

  Screen.fill_OLED(0x00,0x00,0x00);
  Screen.drawString(input, 5, 5, 255, 255, 255,1);
  
}

