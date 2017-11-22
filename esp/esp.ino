#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char* wifi_name = "lights-on";
const char* wifi_pass = "lights-on";
WiFiServer server(80);

////////////////////////
// Server Definitions //
////////////////////////


void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  // Set header
  String header = "HTTP/1.1 200 OK\r\n";
  header += "Content-Type: text/html\r\n\r\n";
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  logStatus("Got following request:");
  logStatus(req);
  client.flush();

  // Prepare the response. Start with the common header:
  String s = header;

  // Match the request
  if (req.indexOf("/all/on") != -1) {
    allOn();
    s += "1";
  } else if (req.indexOf("/all/off") != -1) {
    allOff();
    s += "1";
  } else {
    s += "1";
  }
  
  client.flush();  

  // Send the response to the client
  client.print(s);
  delay(1);

  // Disconnect
  client.stop();
  logStatus("Client disonnected");
}

void setupWiFi()
{
  logStatus("Setup WiFi");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(wifi_name, wifi_pass);
}

void initHardware()
{
  Serial.begin(115200);
}

void logStatus(String msg) {
  Serial.print("[STATUS] ");
  Serial.println(msg);
}

void allOn ()
{
  logStatus("Switch all leds on.");
}

void allOff ()
{
  logStatus("Switch all leds off.");
}

