import controlP5.*; // import controlP5 library
import processing.serial.*;
ControlP5 cp5;
ControlTimer timer;

Accordion accordionDefault;
Accordion accordionResult;
Serial myPort;
JSONObject json;

color c = color(0, 160, 100);
float minimum = 0;
float maximum = 0;
float averageValue = 0;
float averageTotal = 0;
int repCount = 0;
int count = 0;

ArrayList<Float> data = new ArrayList<Float>();
ArrayList<Float> smoothData = new ArrayList<Float>();
float flex = 0;

ArrayList<Float> dataLoad = new ArrayList<Float>();
ArrayList<Float> smoothDataLoad = new ArrayList<Float>();

// Variables for result tab
float duration = 0;
float average = 0;
DropdownList drop_l1; 

// Variables for debugging
float index = 0;

// Variables for real-time analytics
boolean lowerThreshold = false;
boolean upperThreshold = false;
boolean inRep = false;

Textlabel heading;
Textlabel timerLabel;
Textlabel averageLabel;
Textlabel resultInformation;
String[] fileNames;
String selectedFile;
boolean isStart;
boolean raw;
boolean filter;
File f;

void setup() {
  size(1366, 768);
  frameRate(120);
  noStroke();
  smooth();

  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  timer = new ControlTimer();

  f = dataFile(dataPath(""));
  fileNames = f.list(FILTER);

  gui();
  isStart = false;

  timer.setSpeedOfTime(1);
}

void draw() {
  background(#aed6f1);
  if (isStart) {
    generateSin();
    //randomGenerator();
    analyzeNumberOfReps();
  }
  if (cp5.getTab("default").isActive()) {
    drawGraph();
    drawFeedback();
    cp5.getController("flex").setValue(flex);
  }
  if (cp5.getTab("result").isActive())
    drawGraph_result();
  if (isStart) {
    timerLabel.setValue(timer.toString());
    if (count >= 30) {
      averageLabel.setText("Average : "+averageValue+
        "\nNumber of Reps: "+repCount);
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
        index = 0;
        data.clear();
        smoothData.clear();
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
      repCount = 0;
      lowerThreshold = false;
      upperThreshold = false;
      inRep = false;
      count = 0;
      averageLabel.setText("Average : "+averageValue+
        "\nNumber of Reps: "+repCount);
      fileNames = f.list(FILTER);
      drop_l1.clear();
      for (int i = 0; i < fileNames.length; i++) {
        drop_l1.addItem(fileNames[i], i);
      }
    }

    if (theEvent.getController().getName()=="Min") {
      minimum = theEvent.getController().getValue();
    }

    if (theEvent.getController().getName()=="Max") {
      maximum = theEvent.getController().getValue();
    }

    if (theEvent.getController().getName()=="average") {
      theEvent.getController().setValue(averageValue);
    }

    if (theEvent.getController().getName()=="list_d1") {
      selectedFile = fileNames[(int)theEvent.getController().getValue()];
      loadData(selectedFile);
      resultInformation.setText("Duration    : "+ duration + " s" +
        "\nAverage      : "+ average);
    }
  }
}

/*
void serialEvent(Serial p) {
 if (isStart) {
 if ( p.available() >0)
 {
 String val = p.readStringUntil( '\n' );
 if ( val != null)
 {
 try {
 json = JSONObject.parse(val);
 JSONArray i = (JSONArray)json.get("DATA");
 print(i+"\n");
 String j = i.getString(0);
 String k = i.getString(1);
 print(j+"\n");
 float EMGValue = float(j);
 flex = float(k);
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
 }
 */

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

void generateSin() {
  float r = 200*sin(0.05*index+200)+200;
  flex = r;
  data.add(0, r);
  if (smoothData.isEmpty()) {
    smoothData.add(0, r);
    averageTotal += r;
  } else {
    float newSmoothData = 0.9*smoothData.get(0)+0.1*data.get(0);
    smoothData.add(0, newSmoothData);
    averageTotal += newSmoothData;
  }
  averageValue = averageTotal / smoothData.size();
  index++;
}
