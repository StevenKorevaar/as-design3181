/*
 * MeMG Sketch by Team Laika 
 * 
 * Team Laika - Aaron Duivenvoorden, Steven Korevaar, Timothy Scott, and Jeremy Siaw
 * 
 * Submission for TIC 2018 
 */

#include "MeMG.h"
#include "OLED.h"

//Input Pins
#define BUTTON 7
#define EMG_PIN A2
#define FLEX_PIN A1

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


// Exercise Program Flow Variables
short flex = 0;
short emg = 0; 
short replowpoint = 250;
short rephighpoint = 450;
short repmidpoint = 290;
bool withinrep = false;
bool extended = false;
bool pushinghard = false;
bool record = false;
short repcount = 0;
short EMGpush = 300; 


void setup() {

  // Initialise MeMG 
  MeMG.init();
  
  Serial.begin(115200);
  pinMode(BUTTON, INPUT_PULLUP); 

  delay(3000); // Logo Display Time

  // Wait for user begin
  MeMG.update_OLED(START);
  while(digitalRead(BUTTON));
  delay(500);
}

void loop() {
  
    MeMG.update_OLED(GO);
    
    for(int i = 0;i<ARRAY_SIZE;i++){
      // Timestamp for sample rate 
      unsigned long timestamp = micros();

      // Read Sensors
      emg = MeMG.read_EMG(EMG_PIN);
      flex = MeMG.read_Flex(FLEX_PIN);

      //  Store EMG Reading - Must send serial to advance program out of loop
      if(Serial.available()){record = true;}
      if(record) {store[i] = emg;}
      
      //  - Program will keep looping unless EMG recording occurs (by sending serial character),
      //  - Only ~8 seconds of EMG data is recorded (data from 1 for loop),
      //  - Expandable flash memory planned for subsequent prototypes. 


      // Exercise State determination ______________________________________
      
      if (flex > repmidpoint && !withinrep)                 // Rep Starting
      { 
        
        MeMG.update_OLED(WITHIN_REP);
        withinrep = true;
        
      }
      if (flex < replowpoint && withinrep && extended)      // Rep Finished
      {         
        
        MeMG.set_Pixels(REP_FINISHED);
        MeMG.display_Rep(++repcount);
        MeMG.update_OLED(REP_DONE);
        
        pushinghard = extended = withinrep = false;
      
      }
      if (withinrep && emg > EMGpush && !pushinghard)       // Pushing HARD!
      {
        
        MeMG.set_Pixels(EMG_PUSH);
        MeMG.update_OLED(ACT_GOOD);
        
        pushinghard = true; 
        
      }
      if (flex > rephighpoint && withinrep && !extended)    // Leg all the way up!
      { 
        
        MeMG.set_Pixels(FLEX_REACH);
        MeMG.update_OLED(EXT_GOOD);

        extended = true; 
        
      }
     
      //Set 500Hz sample rate by adjusted for loop cost time
      unsigned long interval = (micros()-timestamp);

      if(!record && i < ARRAY_SIZE - 1){i = 0;}             
      
      if(interval < 2000){ 
        //should always be true
        delayMicroseconds(2000-interval);
      }
    
    
    }
        

    // Pixels return to IDLE
    MeMG.set_Pixels(MEMG_IDLE);
   
    // Option to Save Data for debugging & Data verification
    // Send data to USB serial port > CoolTerm > Capture to file
   
    #if SAVE_DATA
  
      MeMG.update_OLED(SAVE);
      while(digitalRead(BUTTON));
      
      for(int i = 0;i<ARRAY_SIZE;i++){
        Serial.println(store[i]);
      }
      delay(500);
      
    #endif

    //Initiate Upload process
    MeMG.update_OLED(UPLOAD);    
    while(digitalRead(BUTTON));


    // Call class member function to handle 4G connection & upload
    MeMG.uploadToAzure(host,path,port,&store[0],ARRAY_SIZE);


    MeMG.update_OLED(FINISH);
    // Program End
    while(1);
}





