void gui() {

  cp5 = new ControlP5(this);


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
    .setFont(createFont("Georgia", 12))
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

  // Second tab
  DropdownList drop_l1 = cp5.addDropdownList("list_d1")
    .setPosition(50, 100)
    .setBackgroundColor(color(190))
    .setItemHeight(20)
    .setBarHeight(15)
    .setWidth(200)
    .setLabel("CSV files")               
    .moveTo("result")
    ;

  for (int i = 0; i < fileNames.length; i++) {
    drop_l1.addItem(fileNames[i], i);
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
  if (data.size() <= 800) {
    for (int i = 0; i <= data.size()-1; i++) {
      if (i == 0) {
        point(i, -data.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line(i-1, -data.get(i-1)-250, i, -data.get(i)-250);
      }
    }

    //Smooth Data
    for (int i = 0; i <= smoothData.size()-1; i++) {
      if (i == 0) {
        point(i, -smoothData.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line(i-1, -smoothData.get(i-1)-250, i, -smoothData.get(i)-250);
      }
    }
  } else {
    for (int i = 0; i <= 800; i++) {
      if (i == 0) {
        point(i, -data.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line(i-1, -data.get(i-1)-250, i, -data.get(i)-250);
      }
    }

    //Smooth Data
    for (int i = 0; i <= 800; i++) {
      if (i == 0) {
        point(i, -smoothData.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line(i-1, -smoothData.get(i-1)-250, i, -smoothData.get(i)-250);
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

void drawGraph_result() {
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
  if (dataLoad.size() <= 800) {
    for (int i = 0; i <= dataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -dataLoad.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line(i-1, -dataLoad.get(i-1)-250, i, -dataLoad.get(i)-250);
      }
    }

    //Smooth Data
    for (int i = 0; i <= smoothDataLoad.size()-1; i++) {
      if (i == 0) {
        point(i, -smoothDataLoad.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line(i-1, -smoothDataLoad.get(i-1)-250, i, -smoothDataLoad.get(i)-250);
      }
    }
  } else {
    for (int i = 0; i <= 800; i++) {
      if (i == 0) {
        point(i, -dataLoad.get(i)-250);
      } else {
        stroke(34, 122, 202);
        line(i-1, -dataLoad.get(i-1)-250, i, -dataLoad.get(i)-250);
      }
    }

    //Smooth Data
    for (int i = 0; i <= 800; i++) {
      if (i == 0) {
        point(i, -smoothDataLoad.get(i)-250);
      } else {
        stroke(245, 136, 6);
        line(i-1, -smoothDataLoad.get(i-1)-250, i, -smoothDataLoad.get(i)-250);
      }
    }
  }  


  popMatrix();
}