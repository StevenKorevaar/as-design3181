/*
 * MeMG Sketch by TIC2018 Team Laika 
 * 
 * 
 */


#include "MeMG.h"

//Input Pins
#define BUTTON 7
#define EMG_PIN A2
#define FLEX_PIN A1

//Pixel States
#define MEMG_IDLE 0
#define REP_FINISHED 1
#define EMG_PUSH 2
#define FLEX_REACH 3

// Set Record Enable - this for writing EMG data to a USB serial monitor 
#define SAVE_DATA 1

//Http config
const char host[] = "laika-hub.azure-devices.net";
const char path[] = "/devices/memg/messages/events?api-version=2016-02-03";
const int port = 443;

// Variable to hold EMG data 
short store[ARRAY_SIZE];

// Create MeMG Object see "MeMG.h"
MeMG MeMG;


// Exercise Phase Program Flow Variables
short flex = 0;
short emg = 0; 
short replowpoint = 250;
short rephighpoint = 450;
short repmidpoint = 290;
bool withinrep = false;
bool extended = false;
bool record = false;
short repcount = 0;
short EMGpush = 300; 


void setup() {
  // Initialise MeMG 
  MeMG.init();
  Serial.begin(115200);
  digitalWrite(BUTTON, HIGH);

  // Wait for user begin
  Serial.println("Push to Start");
  while(digitalRead(BUTTON));
  delay(500);
}

void loop() {

    for(int i = 0;i<ARRAY_SIZE;i++){
      // Timestamp for sample rate 
      unsigned long timestamp = micros();

      // Read Sensors
      emg = MeMG.read_EMG(EMG_PIN);
      flex = MeMG.read_Flex(FLEX_PIN);

      // Store EMG Reading
      if(Serial.available()){
        record = true;
      }
      
      if(record) {store[i] = emg;}

      // Exercise State determination ______________________________________
      
      if (flex > repmidpoint && withinrep == false) // rep starting
      { 
        
        withinrep = true;
        MeMG.OLED_Write("Started Rep");
        
      }
      if (flex < replowpoint && withinrep ==true && extended == true) // rep finished
      { 
        
        MeMG.set_Pixels(REP_FINISHED);
        MeMG.display_Rep(++repcount);
  
        withinrep = false;
        extended = false;
        MeMG.OLED_Write("Rep Finished");
        
      }
      if (withinrep == true && emg > EMGpush) // pushing HARD!
      {
        
        MeMG.set_Pixels(EMG_PUSH);
        MeMG.OLED_Write("PUSHING HARD!");
        
      }
      if (flex > rephighpoint && withinrep == true && extended == false) // Leg all the way up!
      { 
        
        MeMG.set_Pixels(FLEX_REACH);
        extended = true; 
        MeMG.OLED_Write("Leg extended");
        
      }
     
      //Set 500Hz sample rate by adjusted for loop cost time
      unsigned long interval = (micros()-timestamp);

      if(!record && i < ARRAY_SIZE - 1){i = 0;}
      
      if(interval < 2000){ 
        //should always be true
        delayMicroseconds(2000-interval);
      }
    }
   
     // Send data to CoolTerm > Capture to file
  #if SAVE_DATA

    Serial.println("Push to Save");
    while(digitalRead(BUTTON));
    
    for(int i = 0;i<ARRAY_SIZE;i++){
      Serial.println(store[i]);
    }
    
  #endif

  Serial.println("Push to Upload");
  while(digitalRead(BUTTON));
  
  MeMG.uploadToAzure(host,path,port,&store[0],ARRAY_SIZE);

  while(1);
}





