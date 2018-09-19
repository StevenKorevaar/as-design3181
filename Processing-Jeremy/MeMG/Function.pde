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

  saveTable(table, "data/EMG "+year()+"-"+month()+"-"+day()+"_"+hour()+minute()+".csv");
}

void loadData(String fileName) {
  dataLoad.clear();
  smoothDataLoad.clear();

  Table tableForLoad = loadTable(fileName, "header");
  int rowCount = 0;
  for (TableRow row : tableForLoad.rows()) {
    dataLoad.add(rowCount, row.getFloat("raw"));
    smoothDataLoad.add(rowCount, row.getFloat("smooth"));
    rowCount++;
  }
}
