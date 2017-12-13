#include <ESP8266WiFi.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char* wifi_name = "lights-on";
const char* wifi_pass = "lights-on";
WiFiServer server(80);

/////////////////////
// LED Definitions //
/////////////////////
const int pinCount = 6;
const int allLedCount = 2;

int ledRight[] = {D5, D6, D7};

int ledLeft[] = {D2, D3, D4};

int rgbPins[pinCount] = {D2, D3, D4, D5, D6, D7};

int* allLeds[allLedCount] = {ledLeft, ledRight};


String split1(String to_split, String delimiter) {
  return to_split.substring(to_split.indexOf(delimiter));  
}

String split1plus(String to_split, String delimiter) {
  return to_split.substring(to_split.indexOf(delimiter)+1);  
}

String split0(String to_split, String delimiter) {
  return to_split.substring(0, to_split.indexOf(delimiter));  
}

void setup() 
{
  WiFi.persistent(false);
  initHardware();
  setupWiFi();
  server.begin();

  // Reset all leds
  setRGB(0, 0, 0, ledLeft);
  setRGB(0, 0, 0, ledRight);
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
  } else if (req.indexOf("/all/set/color/") != -1) {
    String color;
    color = split1(req, "color/");
    color = split1plus(color, "/");
    color = split0(color, " ");
    setColorOfAll(color);
    Serial.println(color);
    s += "1";
  } else {
    s += "0";
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

  // Init leds
  int i;
  for (i = 0; i < pinCount; i = i + 1) {
    int pin = rgbPins[i];
    pinMode(pin, OUTPUT);
    
    Serial.print("Set mode of pin no. ");
    Serial.print(pin);
    Serial.println(" to output.");
  }
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

void setColorOfAll (String color) {
  // Convert to Int
  int number = (int) strtol( &color[0], NULL, 16);
  
  // Split them up into r, g, b values
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);

  int i;
  for (i = 0; i < allLedCount; i = i + 1) {
    int* led = allLeds[i];

    logStatus("Change color of led no. " + (String) i);
    setRGB(r, g, b, led);
  }

  logStatus("Changed color of all leds.");

}

void setBrightness (int pin, int brightness) {
  analogWrite(pin, brightness);
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(brightness);
}

void setRGB (int r, int g, int b, int* pins) {
  Serial.println("Set LED");
  setBrightness(pins[0], r);
  setBrightness(pins[1], g);
  setBrightness(pins[2], b);
}



