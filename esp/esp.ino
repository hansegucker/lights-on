#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiAPPSK[] = "lights-on";

WiFiServer server(80);

void setup() 
{
  initHardware();
  setupWiFi();
  server.begin();
}

void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  logStatus(req);
  client.flush();

  // Match the request
  if (req.indexOf("/all/on") != -1)
    allOn();
  else if (req.indexOf("/all/off") != -1)
    allOff();
  
  client.flush();

  // Prepare the response. Start with the common header:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "It works";
  s += "</html>\n";

  // Send the response to the client
  client.print(s);
  delay(1);
  logStatus("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void setupWiFi()
{
  Serial.println("Setup WiFi");
  WiFi.mode(WIFI_AP);

  // Do a little work to get a unique-ish name. Append the
  // last two bytes of the MAC (HEX'd) to "Thing-":
  //uint8_t mac[WL_MAC_ADDR_LENGTH];
  //WiFi.softAPmacAddress(mac);
  //String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
  //               String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  //macID.toUpperCase();
  String AP_NameString = "lights-on";

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i=0; i<AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAP(AP_NameChar, WiFiAPPSK);
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

