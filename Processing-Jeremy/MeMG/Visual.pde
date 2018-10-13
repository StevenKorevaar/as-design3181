void gui() {

  cp5 = new ControlP5(this);

  PFont pfont = createFont("Georgia", 20, true);
  ControlFont font = new ControlFont(pfont,12);
  ControlFont font2 = new ControlFont(pfont,12);
  

  cp5.addTab("result")
    .setColorBackground(color(0, 160, 100))
    .setColorLabel(color(255))
    .setColorActive(color(255, 128, 0))
    ;

  cp5.getTab("default")
    .activateEvent(true)
    .setLabel("Default Tab")
    .setId(1)
    ;

  cp5.getTab("result")
    .activateEvent(true)
    .setId(2)
    ;

  // Timer
  timerLabel = cp5.addTextlabel("timer")
    .setPosition(1150, 125)
    .setColorValue(0)
    .setValue(timer.toString())
    .setFont(createFont("Georgia", 14))
    ;
    
  // Flex Slider
  cp5.addSlider("flex")
     .setPosition(1300,100)
     .setSize(20,600)
     .setRange(0,500)
     .setColorCaptionLabel(255)
     .setColorValueLabel(255)
     .setFont(font2)
     ;

  // Title
  heading = cp5.addTextlabel("title")
    .setText("Me-MG User Interface")
    .setPosition(50, 50)
    .setColorValue(#154360)
    .setFont(createFont("Georgia", 20))
    .moveTo("global")
    ;
  Group g1 = cp5.addGroup("Control")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;

  cp5.addBang("Start")
    .setPosition(40, 40)
    .setSize(60, 60)
    .moveTo(g1)
    .setFont(font2)
    //.plugTo(this,"shuffle");
    ;
  cp5.addBang("Reset")
    .setPosition(140, 40)
    .setSize(60, 60)
    .setColorForeground(#c0392b)
    .setColorActive(#f1948a)
    .setFont(font2)
    .moveTo(g1)
    //.plugTo(this,"shuffle");
    ;

  // Group 2
  Group g2 = cp5.addGroup("Threshold")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;

  cp5.addSlider("Min")
    .setPosition(20, 30)
    .setSize(10, 80)
    .setValue(150)
    .setRange(0, 500)
    .setSliderMode(Slider.FLEXIBLE)
    .moveTo(g2)
    ;
  cp5.addSlider("Max")
    .setPosition(80, 30)
    .setSize(10, 80)
    .setValue(300)
    .setRange(0, 500)
    .setSliderMode(Slider.FLEXIBLE)
    .moveTo(g2)
    ;
  cp5.addToggle("raw")
    .setPosition(150, 30)
    .setSize(50, 20)
    .setValue(true)
    .setMode(ControlP5.SWITCH)
    .moveTo(g2)
    ;

  cp5.addToggle("filter")
    .setPosition(150, 90)
    .setSize(50, 20)
    .setValue(true)
    .setMode(ControlP5.SWITCH)
    .moveTo(g2)
    ;

  // Group 3
  Group g3 = cp5.addGroup("Information")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;
  averageLabel = cp5.addTextlabel("average")
    .setText("Average : "+averageValue+
    "\nNumber of Reps: "+repCount)
    .setPosition(40, 40)
    .setColorValue(255)
    .setFont(createFont("Georgia", 14))
    .moveTo(g3)
    ;

  // Group 4
  Group g4 = cp5.addGroup("Result Information")
    .setBackgroundColor(color(0, 64))
    .setBackgroundHeight(150)
    ;
  resultInformation = cp5.addTextlabel("result")
    .setText("Duration    : "+ duration + " s" +
    "\nAverage      : "+ average)
    .setPosition(20, 20)
    .setColorValue(255)
    .setFont(createFont("Georgia", 14))
    .moveTo(g4)
    ;

  // Accordion
  accordionDefault = cp5.addAccordion("acc")
    .setPosition(50, 100)
    .setWidth(250)
    .addItem(g1)
    .addItem(g2)
    .addItem(g3)
    .moveTo("default")
    ;
  accordionDefault.open(0, 1, 2);

  accordionDefault.setCollapseMode(Accordion.MULTI);

  accordionResult = cp5.addAccordion("accResult")
    .setPosition(50, 300)
    .setWidth(250)
    .addItem(g4)
    .moveTo("result")
    ;
  accordionResult.open(0);

  // Second tab
  drop_l1 = cp5.addDropdownList("list_d1")
    .setPosition(50, 100)
    .setBackgroundColor(color(190))
    .setItemHeight(20)
    .setBarHeight(30)
    .setWidth(200)
    .setFont(font)
    .setLabel("CSV files")               
    .moveTo("result")
    ;

  for (int i = 0; i < fileNames.length; i++) {
    drop_l1.addItem(fileNames[i], i);
  }
}

void drawFeedback() {
  pushMatrix();
  translate(120, 650);
  stroke(0, 0, 0);
  if (!isStart)
    fill(32, 32, 32);
  else
    fill(51, 255, 51);
  ellipse(0, 0, 50, 50);
  fill(0, 102, 153);
  text("Status", -20, 50);
  if (!inRep)
    fill(255, 51, 51);
  else
    fill(102, 255, 102);
  ellipse(100, 0, 50, 50);
  fill(0, 102, 153);
  text("In Rep", 80, 50);
  popMatrix();
}

void drawGraph() {
  pushMatrix();
  translate(400, 650);
  stroke(0);
  // Background
  strokeWeight(0);
  fill(255);
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
  if (isStart) {
    strokeWeight(1);
    if (data.size() <= 800) {
      if (raw) {
        for (int i = 0; i <= data.size()-1; i++) {
          if (i == 0) {
            point(i, -data.get(i)-250);
          } else {
            stroke(34, 122, 202);
            line(i-1, -data.get(i-1), i, -data.get(i));
          }
        }
      }

      //Smooth Data
      if (filter) {
        for (int i = 0; i <= smoothData.size()-1; i++) {
          if (i == 0) {
            point(i, -smoothData.get(i)-250);
          } else {
            stroke(245, 136, 6);
            line(i-1, -smoothData.get(i-1), i, -smoothData.get(i));
          }
        }
      }
    } else {
      if (raw) {
        for (int i = 0; i <= 800; i++) {
          if (i == 0) {
            point(i, -data.get(i)-250);
          } else {
            stroke(34, 122, 202);
            line(i-1, -data.get(i-1), i, -data.get(i));
          }
        }
      }

      //Smooth Data
      if (filter) {
        for (int i = 0; i <= 800; i++) {
          if (i == 0) {
            point(i, -smoothData.get(i)-250);
          } else {
            stroke(245, 136, 6);
            line(i-1, -smoothData.get(i-1), i, -smoothData.get(i));
          }
        }
      }
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
  popMatrix();
}

// Plotting data on the result tab
void drawGraph_result() {
  pushMatrix();
  translate(400, 650);
  stroke(0);
  // Background
  strokeWeight(0);
  fill(255);
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
  if (dataLoad.size() <= 800) {
    for (int i = 0; i <= dataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -dataLoad.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line((i-1)*800/dataLoad.size(), -dataLoad.get(i-1), i*800/dataLoad.size(), -dataLoad.get(i));
      }
    }

    //Smooth Data
    for (int i = 0; i <= smoothDataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -smoothDataLoad.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line((i-1)*800/dataLoad.size(), -smoothDataLoad.get(i-1), i*800/dataLoad.size(), -smoothDataLoad.get(i));
      }
    }
  } else {
    for (int i = 0; i <= dataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -dataLoad.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line((i-1)*800/dataLoad.size(), -dataLoad.get(i-1), i*800/dataLoad.size(), -dataLoad.get(i));
      }
    }

    //Smooth Data
    for (int i = 0; i <= smoothDataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -smoothDataLoad.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line((i-1)*800/dataLoad.size(), -smoothDataLoad.get(i-1), i*800/dataLoad.size(), -smoothDataLoad.get(i));
      }
    }
  }  
  popMatrix();
}
