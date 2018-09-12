import controlP5.*; // import controlP5 library
ControlP5 cp5;
ControlTimer timer;

Accordion accordion;

color c = color(0, 160, 100);
float minimum = 0;
float maximum = 0;
float averageValue = 0;
float averageTotal = 0;
int count = 0;
ArrayList<Float> data = new ArrayList<Float>();
ArrayList<Float> smoothData = new ArrayList<Float>();


Textlabel heading;
Textlabel timerLabel;
Textlabel averageLabel;
boolean isStart;

void setup() {
  size(1366, 768);
  frameRate(30);
  noStroke();
  smooth();
  gui();
  cp5 = new ControlP5(this);
  timer = new ControlTimer();
  isStart = false;

  heading = cp5.addTextlabel("label")
    .setText("Me-MG User Interface")
    .setPosition(50, 50)
    .setColorValue(#154360)
    .setFont(createFont("Georgia", 20))
    ;

  timerLabel = cp5.addTextlabel("timer")
    .setPosition(1150, 125)
    .setColorValue(0)
    .setValue(timer.toString())
    .setFont(createFont("Georgia", 12))
    ;
  timer.setSpeedOfTime(1);
}

void gui() {

  cp5 = new ControlP5(this);

  heading = cp5.addTextlabel("label")
    .setText("Me-MG User Interface")
    .setPosition(50, 50)
    .setColorValue(#154360)
    .setFont(createFont("Georgia", 20))
    ;
  Group g1 = cp5.addGroup("Control")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;

  cp5.addBang("Start")
    .setPosition(40, 40)
    .setSize(60, 60)
    .moveTo(g1)
    //.plugTo(this,"shuffle");
    ;
  cp5.addBang("Reset")
    .setPosition(140, 40)
    .setSize(60, 60)
    .setColorForeground(#c0392b)
    .setColorActive(#f1948a)
    .moveTo(g1)
    //.plugTo(this,"shuffle");
    ;

  // Group 2
  Group g2 = cp5.addGroup("Threshold")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;

  cp5.addSlider("Minimum")
    .setPosition(40, 40)
    .setSize(10, 80)
    .setValue(150)
    .setRange(0, 500)
    .setSliderMode(Slider.FLEXIBLE)
    .moveTo(g2)
    ;
  cp5.addSlider("Maximum")
    .setPosition(140, 40)
    .setSize(10, 80)
    .setValue(300)
    .setRange(0, 500)
    .setSliderMode(Slider.FLEXIBLE)
    .moveTo(g2)
    ;

  // Group 3
  Group g3 = cp5.addGroup("Information")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;
  averageLabel = cp5.addTextlabel("average")
    .setText("Average : "+averageValue)
    .setPosition(40, 40)
    .setColorValue(255)
    .setFont(createFont("Georgia", 12))
    .moveTo(g3)
    ;

  // Accordion
  accordion = cp5.addAccordion("acc")
    .setPosition(50, 100)
    .setWidth(250)
    .addItem(g1)
    .addItem(g2)
    .addItem(g3)
    .moveTo("default")
    ;
  accordion.open(0, 1, 2);

  accordion.setCollapseMode(Accordion.MULTI);
}

void draw() {
  background(#aed6f1);
  if (isStart)
    randomGenerator();
  drawGraph();
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
  }
}

void drawGraph() {
  pushMatrix();
  translate(400, 650);
  stroke(0);
  // Background
  strokeWeight(0);
  rect(-50, -550, 900, 600);

  //Axis
  strokeWeight(3);
  line(0, 0, 800, 0);
  line(0, -500, 0, 0);
  fill(0);
  triangle(0, -500, -10, -480, 10, -480);
  triangle(800, 0, 780, 10, 780, -10);
  fill(255);

  //Plot
  //Raw Data
  strokeWeight(1);
  for (int i = 0; i <= data.size()-1; i++) {
    if (i == 0) {
      point(i, -data.get(i));
    } else {
      stroke(34, 122, 202);
      line(i-1, -data.get(i-1), i, -data.get(i));
    }
  }

  //Smooth Data
  for (int i = 0; i <= smoothData.size()-1; i++) {
    if (i == 0) {
      point(i, -smoothData.get(i));
    } else {
      stroke(245, 136, 6);
      line(i-1, -smoothData.get(i-1), i, -smoothData.get(i));
    }
  }

  //Threshold
  //Minimum
  stroke(255, 0, 0);
  strokeWeight(3);
  for (int i = 0; i <= 100; i++) {
    float x = lerp(0, 800, i/100.0);
    point(x, -minimum);
  }
  //Maximum
  stroke(0, 255, 0);
  for (int i = 0; i <= 100; i++) {
    float x = lerp(0, 800, i/100.0);
    point(x, -maximum);
  }

  if (data.size()>=800) {
    data.remove(799);
    averageTotal -= smoothData.get(799);
    smoothData.remove(799);
  }
  popMatrix();
}

void randomGenerator() {
  float r = random(0, 500);
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
}

void saveData(){
  Table table = new Table();
  
  table.addColumn("no");
  table.addColumn("raw");
  table.addColumn("smooth");
  
  for(int i = 0; i <= data.size()-1; i++){
    TableRow newRow = table.addRow();
    newRow.setInt("no", table.getRowCount()-1);
    newRow.setFloat("raw",data.get(i));
    newRow.setFloat("smooth",smoothData.get(i));
  }
  
  saveTable(table, "data/new.csv");
}
