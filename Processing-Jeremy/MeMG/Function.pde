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
    rowCount++;
  }
  duration = (float)rowCount/30;
  average = temp/rowCount;
}

void analyzeNumberOfReps() {
  float currentData = data.get(0);

  if (!inRep) {

    if (!lowerThreshold && !upperThreshold) {
      if (currentData > minimum) {
        lowerThreshold = true;
      }
    }

    if (lowerThreshold & !upperThreshold) {
      if (currentData < minimum) {
        lowerThreshold = false;
      }
      if (currentData > maximum) {
        upperThreshold = true;
        inRep = true;
      }
    }
  }

  if (inRep) {

    if (lowerThreshold & upperThreshold) {
      if (currentData < maximum) {
        upperThreshold = false;
      }
    }
    if (lowerThreshold & !upperThreshold) {
      if (currentData < minimum) {
        lowerThreshold = false;
        inRep = false;
        repCount++;
      }
    }
  }
}
