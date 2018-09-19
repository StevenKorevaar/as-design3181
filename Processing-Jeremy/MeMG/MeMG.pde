import controlP5.*; // import controlP5 library
import processing.serial.*;
ControlP5 cp5;
ControlTimer timer;

Accordion accordion;
Serial myPort;
JSONObject json;

color c = color(0, 160, 100);
float minimum = 0;
float maximum = 0;
float averageValue = 0;
float averageTotal = 0;
int count = 0;
ArrayList<Float> data = new ArrayList<Float>();
ArrayList<Float> smoothData = new ArrayList<Float>();

ArrayList<Float> dataLoad = new ArrayList<Float>();
ArrayList<Float> smoothDataLoad = new ArrayList<Float>();

Textlabel heading;
Textlabel timerLabel;
Textlabel averageLabel;
String[] fileNames;
String selectedFile;
boolean isStart;

void setup() {
  size(1366, 768);
  frameRate(30);
  noStroke();
  smooth();

  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
  timer = new ControlTimer();

  File f = dataFile(dataPath(""));
  fileNames = f.list(FILTER);

  gui();
  isStart = false;

  timer.setSpeedOfTime(1);
}

void draw() {
  background(#aed6f1);
  if (isStart) {
    readFromArduino();
    //randomGenerator();
  }
  if (cp5.getTab("default").isActive())
    drawGraph();
  if (cp5.getTab("result").isActive())
    drawGraph_result();
  if (isStart) {
    timerLabel.setValue(timer.toString());
    if (count >= 30) {
      averageLabel.setText("Average : "+averageValue);
      count = 0;
    }
  }
  count++;
}

void controlEvent(ControlEvent theEvent) {
  if (theEvent.isController()) {
    if (theEvent.getController().getName()=="Start") {
      if (!isStart) {
        isStart = true;
        timer.reset();
      }
    }

    if (theEvent.getController().getName()=="Reset") {
      isStart = false;
      saveData();
      timer.reset();
      timerLabel.setValue(timer.toString());
      data.clear();
      smoothData.clear();
      averageValue = 0;
      count = 0;
      averageLabel.setText("Average : "+averageValue);
    }

    if (theEvent.getController().getName()=="Minimum") {
      minimum = theEvent.getController().getValue();
    }

    if (theEvent.getController().getName()=="Maximum") {
      maximum = theEvent.getController().getValue();
    }

    if (theEvent.getController().getName()=="average") {
      theEvent.getController().setValue(averageValue);
    }
    
    if(theEvent.getController().getName()=="list_d1"){
      selectedFile = fileNames[(int)theEvent.getController().getValue()];
      loadData(selectedFile);
    }
  }
}

void readFromArduino() {
  if ( myPort.available() >0)
  {
    String val = myPort.readStringUntil( '\n' );
    if ( val != null)
    {
      try {
        json = JSONObject.parse(val);
        JSONArray i = (JSONArray)json.get("EMG");
        String j = i.getString(0);
        float EMGValue = float(j)*250; 
        data.add(0, EMGValue);
        if (smoothData.isEmpty()) {
          smoothData.add(0, EMGValue);
          averageTotal += EMGValue;
        } else {
          float newSmoothData = 0.9*smoothData.get(0)+0.1*data.get(0);
          smoothData.add(0, newSmoothData);
          averageTotal += newSmoothData;
        }
        averageValue = averageTotal / smoothData.size();
      }
      catch (Exception e) {
        e.printStackTrace();
      }
    }
  }
}

void randomGenerator() {
  float r = random(0, 500);
  data.add(0, r);
  if (smoothData.isEmpty()) {
    smoothData.add(0, r);
    averageTotal += r;
  } else {
    float newSmoothData = 0.98*smoothData.get(0)+0.1*data.get(0);
    smoothData.add(0, newSmoothData);
    averageTotal += newSmoothData;
  }
  averageValue = averageTotal / smoothData.size();
}
