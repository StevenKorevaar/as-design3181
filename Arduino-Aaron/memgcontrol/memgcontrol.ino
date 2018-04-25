#include <TM1637Display.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6 // Pin controlling NEOPIXELS
#define NUMPIXELS 2  // Number of NEOPIXELS used
const int CLK = 9; //Set the CLK pin connection to the display
const int DIO = 8; //Set the DIO pin connection to the display

//create neopixel object. 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int dataCount = 0;
int delayval = 500;

int readData() {  //generate fuzzy sinuciod
  ++dataCount;
  float ret = (900/2)*sin( dataCount/10.0 ) + (900/2) + random(0, 300);
  if(ret > 1024) {
    ret = 1024;
  }
  if(ret < 0 ) {
    ret = 0;
  }
  return abs( ret );
}

int NumStep = 0;  //Variable to interate

TM1637Display display(CLK, DIO);  //set up the 4-Digit Display.

void setup() {
  //serial init
  Serial.begin(115200);
  pixels.begin(); //pixel init
  randomSeed(analogRead(1)); //to randomize generated signal

  display.setBrightness(0x0a);  //set the diplay to maximum brightness
  
}

bool notChanged = true;
short thresholdemg = 0;
short lastThresholdemg = 0;
short thresholdflex = 0;
short lastThresholdflex = 0;
bool belowThreshFlex = false;
float emg = 0;
float flex = 0;
float smoothedEMG = 0;
int reps = 0;

#define timeCountMAX 1000

void loop() {
  // put your main code here, to run repeatedly:

  int timeCount = timeCountMAX;
  unsigned long time = millis();
  while(timeCount > 0) {
    flex = analogRead(0);
    --timeCount;
  }
  //Serial.print("Time Taken: ");
  Serial.println( (time-millis()) );
  
  while(notChanged) {
    emg = readData();
    
    flex = analogRead(0);
    
    //smoothedEMG = 0.95*smoothedEMG + 0.05*emg;
   
    //Serial.print( "EMG: " );
    //Serial.println( emg );
    //Serial.print( "FLEX: " );
    //Serial.println( flex );
    
    //EMG Thresholds
    if( emg >= 800 ) {
      pixels.setPixelColor(0, pixels.Color(0,100,0));   //GREEN
      thresholdemg = 2;
    }
    else if( emg >= 400 ) {
      pixels.setPixelColor(0, pixels.Color(75,75,0));   //YELLOW
      thresholdemg = 1;
    }
    else {
      pixels.setPixelColor(0, pixels.Color(100,0,0));   //RED
      thresholdemg = 0;
    }
    
    //Flex Thresholds
    if( flex >= 500 ) {
      pixels.setPixelColor(1, pixels.Color(0,100,0));   //GREEN
      thresholdflex = 2;
      belowThreshFlex = true;
    }
    else if( flex >= 300 ) {
      pixels.setPixelColor(1, pixels.Color(75,75,0));   //YELLOW
      thresholdflex = 1;
    }
    else {
      pixels.setPixelColor(1, pixels.Color(100,0,0));   //RED
      thresholdflex = 0;
      
    }

    if(thresholdemg != lastThresholdemg) {
      lastThresholdemg = thresholdemg;
      notChanged = false;
    }
    if(thresholdflex != lastThresholdflex) {
        if(thresholdflex == 0 && belowThreshFlex == true) {
          ++reps;
          belowThreshFlex = false;
        }
        
      lastThresholdflex = thresholdflex;
      notChanged = false;
    }

    
    display.showNumberDec(reps);
    //Serial.println(analogRead(0));
    delay(100); 
  }
  notChanged = true;
  
  pixels.show(); // This sends the updated pixel color to the hardware.
  
  //delay(delayval); 
}
