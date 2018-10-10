import processing.serial.*;

Serial myPort;
JSONObject json;
String val;

void setup()
{
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  if ( myPort.available() >0)
  {
    val = myPort.readStringUntil( '\n' );
    if ( val != null)
    {
      println(val);
      try {
        json = JSONObject.parse(val);
        JSONArray i = (JSONArray)json.get("EMG");
        String j = i.getString(0);
        float k = float(j); 
        println(k);
      }
      catch (Exception e) {
        e.printStackTrace();
      }
    }
  }
}
