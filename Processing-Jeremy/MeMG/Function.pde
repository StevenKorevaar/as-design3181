import java.io.FilenameFilter;

static final FilenameFilter FILTER = new FilenameFilter() {
  static final String NAME = "EMG", EXT = ".csv";

  @ Override boolean accept(File path, String name) {
    return name.startsWith(NAME) && name.endsWith(EXT);
  }
};

void saveData() {
  Table table = new Table();

  table.addColumn("no");
  table.addColumn("raw");
  table.addColumn("smooth");

  for (int i = 0; i <= data.size()-1; i++) {
    TableRow newRow = table.addRow();
    newRow.setInt("no", table.getRowCount()-1);
    newRow.setFloat("raw", data.get(i));
    newRow.setFloat("smooth", smoothData.get(i));
    if (i == 0) {
      newRow.setString("duration", timer.toString());
    }
  }

  saveTable(table, "data/EMG "+year()+"-"+month()+"-"+day()+"_"+hour()+"_"+minute()+".csv");
}

void loadData(String fileName) {
  dataLoad.clear();
  smoothDataLoad.clear();
  int temp = 0;
  Table tableForLoad = loadTable(fileName, "header");
  int rowCount = 0;
  for (TableRow row : tableForLoad.rows()) {
    dataLoad.add(rowCount, row.getFloat("raw"));
    temp += row.getFloat("raw");
    smoothDataLoad.add(rowCount, row.getFloat("smooth"));
    if (rowCount == 0) {
      resultDuration = row.getString("duration");
    }
    rowCount++;
  }
  average = temp/rowCount;
}

void analyzeNumberOfReps() {
  if (!data.isEmpty()) {

    if (!inRep) {

      if (!lowerThreshold && !upperThreshold) {
        if (flex > rep_low) {
          lowerThreshold = true;
        }
      }

      if (lowerThreshold & !upperThreshold) {
        if (flex < rep_low) {
          lowerThreshold = false;
        }
        if (flex > rep_high) {
          upperThreshold = true;
          inRep = true;
        }
      }
    }

    if (inRep) {

      if (lowerThreshold & upperThreshold) {
        if (flex < rep_high) {
          upperThreshold = false;
        }
      }
      if (lowerThreshold & !upperThreshold) {
        if (flex < rep_low) {
          lowerThreshold = false;
          inRep = false;
          repCount++;
        }
      }
    }
  }
}

void sendToArduino(String type) {
  if (isStart) {
    if (type == "base") {
      //print("a"+int(minimum)+"\n");
      myPort.write("a"+int(minimum));
    }
    if (type == "emg") {
      print("b" + int(maximum)+ "\n");
      myPort.write("b"+int(maximum));
    }
    if (type == "rep_low") {
      myPort.write("c"+int(rep_low));
    }
    if (type == "rep_high") {
      myPort.write("d"+int(rep_high));
    }
  }
}

void mouseReleased() {
  if (isBaselineChanged) {
    sendToArduino("base");
    isBaselineChanged = false;
  }
  if (isEMGChanged) {
    sendToArduino("emg");
    isEMGChanged = false;
  }
  if (isRepLowChanged) {
    sendToArduino("rep_low");
    isRepLowChanged = false;
  }
   if (isRepHighChanged) {
    sendToArduino("rep_high");
    isRepHighChanged = false;
  }
  
}
