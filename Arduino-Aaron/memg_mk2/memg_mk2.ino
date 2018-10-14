#include "EMGFilters.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#define EMG_PIN A2   //sensor input pin numbers
#define FLEX_PIN A1

//PIXEL STUFF___________________________________
#define NEO_PIN 11 // Pin controlling NEOPIXELS
#define NUMPIXELS 4  // Number of NEOPIXELS used
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

//EMG stuff __________
#define CALIBRATE 0
unsigned long threshold = 800;  // threshold: Relaxed baseline values.(threshold=0:in the calibration process)
unsigned long EMG_num = 0;      // EMG_num: The number of statistical signals
int scaling = 240;
int baseline = 1000;
EMGFilters myFilter;
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;

void setup() 
{
  myFilter.init(sampleRate, humFreq, true, true, true);
  Serial.begin(115200);
  Serial.flush();
  pixels.begin(); //pixel init
  pixels.setPixelColor(0, pixels.Color(30,0,30));   // LED 1 to dull pink
  pixels.setPixelColor(1, pixels.Color(30,0,30));   // LED 2 to dull pink
  pixels.setPixelColor(2, pixels.Color(30,0,30));   // LED 3 to dull pink
  pixels.setPixelColor(3, pixels.Color(30,0,30));   // LED 4 to dull pink
  pixels.show();
}

short flex = 0;
short emg = 0; 
short replowpoint = 240;
short rephighpoint = 380;
short repmidpoint = 300;
bool withinrep = false;
bool extended = false;
int repcount = 0;
int EMGpush = 300;

void loop() 
{   
    unsigned long timeStamp = micros();

    emg = EMGread();
    flex = analogRead(FLEX_PIN); 
    //emg = random(100);
    //flex = random(100);

    Serial.flush();
    Serial.println(makeJsonData(String(emg),String(flex)));
    
    if (flex > repmidpoint && withinrep == false) // rep starting
    { 
      withinrep = true;
    }
    if (flex < replowpoint && withinrep ==true && extended == true) // rep finished
    { 
      pixels.setPixelColor(0, pixels.Color(30,0,30));   //LED 1 to PINK
      pixels.setPixelColor(3, pixels.Color(30,0,30));   //LED 2 to PINK
      pixels.setPixelColor(2, pixels.Color(30,0,30));   //LED 3 to PINK
      pixels.setPixelColor(1, pixels.Color(30,0,30));   //LED 4 to PINK
      pixels.show();
      
      withinrep = false;
      extended = false;
    }
    if (withinrep == true && emg > EMGpush) // pushing HARD!
    { 
      pixels.setPixelColor(0, pixels.Color(0,100,0));   // LED 1 to GREEN
      pixels.setPixelColor(3, pixels.Color(0,100,0));   // LED 2 to GREEN
      pixels.show();
 
    }
    if (flex > rephighpoint && withinrep == true && extended == false) // Leg all the way up!
    { 
      pixels.setPixelColor(2, pixels.Color(0,0,100));   //LED 3 to BLUE
      pixels.setPixelColor(1, pixels.Color(0,0,100));   //LED 4 to BLUE
      pixels.show();

      extended = true; 
    }

    unsigned long interval = micros() - timeStamp;
    if(interval < 2000){
           delayMicroseconds(2000-interval);
    }

}

short EMGread(){
    
    //unsigned long timeStamp = micros();
    int data = analogRead(EMG_PIN);
    //Serial.print("Data: ");
    //Serial.println(data);
    // filter processing
    int dataAfterFilter = myFilter.update(data);
    //Serial.print("Data After Filter: ");
    //Serial.println(dataAfterFilter);
    int envelope = sq(dataAfterFilter);  // Get envelope by squaring the input
    if (CALIBRATE) {
        //Serial.print("Squared Data: ");
        Serial.println(envelope);
    }
    else {
        // Any value below the `baseline` value will be treated as zero
        if (envelope < baseline) {
            dataAfterFilter = 0;
            envelope = 0;
        }
        envelope = envelope / scaling;
    }
    // Usually, you should still have (interval - timeElapsed) to do other work.
    // Otherwise, you would have to lower down the `sampleRate`.
    //unsigned long timeElapsed = micros() - timeStamp;
#if _DEBUG
    Serial.print("Filters cost time: ");
    Serial.println(timeElapsed);
#else
   //delayMicroseconds(500);
#endif
  if(envelope > 1023){envelope = 1023;}
  
  return (short) envelope;
}

String makeJsonData(String emg_data,String flex_data){ 
  String content = "";
  DynamicJsonBuffer  jsonBuffer;
  JsonObject& jsonOb = jsonBuffer.createObject();      // Create the root of the object tree.

  JsonArray& jsonAr1 = jsonOb.createNestedArray("DATA"); // Create field for EMG data
  jsonAr1.add(emg_data);                               // Insert string  
  jsonAr1.add(flex_data);                               // Insert string
  
  jsonOb.printTo(content);                             // Create JSON

return content;
}
  
