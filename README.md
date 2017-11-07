# lights-on
A light composition

## Install
1. Download Arduino IDE
2. Do 
http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/arduino-esp8266-lolin-nodemcu-getting-started/

Use the Arduino IDE to Program your Lolin V.3 NodeMCU V1.0
This tutorial will show you how to use the Arduino IDE to program your LoLin V.3 NodeMCU WIFI board.  The steps are pretty simple to follow and should get your board running in less than a half hour.

If you’re new to  this module,  its fundamentally a System On a Chip (SOC) that includes WIFI ability).   It represents a great starting point to the famliarize yourself with the technologies surrounding the Internet of Things (IoT).

At the end of the end of the tutorial,  you will scan for nearby WiFi networks and display the results on your serial monitor.

Required Parts
You will need a LoLin NodeMCU Module. 

Preparing the Arduino IDE to Work with the NodeMCU Module
Don’t get intimidated.  These are relatively painless steps.

Go To ‘Preferences’
Select “Preferences” via the File Menu.


Add A URL
Type “http://arduino.esp8266.com/stable/package_esp8266com_index.json” into the field for ‘Additional Boards Manager URL’.

addurl

Access ‘Board Manager’
Select “Boards Manager” via the Tool Menu.


Install the ESP8266 Files
From the boards manager pop up, scroll until you find the esp8266 board.   Select the latest version and install.


Select the NodeMCU V1.0 ESP8266-12E Board
The board is selected via the ‘Tools’ menu.



Select the CPU Frequency and Upload Speed
Match the picture below.


Connect the LoLin NodeMCU Module to your PC
First Time Connecting
If this is your first time connecting your development board, you may simply need to connect the ESP8266-12E module to your computer via a micro USB cable.

Otherwise, there may be a program running that consumes more current than the USB can provide.  In that case, you may want to use connect your own supply.

Second Time Connecting
If your computer USB cannot supply the required power,  you may have to try something else.   You have several options and the are covered HERE.

The configuration I generally use is shown below.

 

Copy, Paste and Upload the Tutorial Sketch
The sketch is one that comes as an example from ESP8266.COM.

#include "ESP8266WiFi.h"

void setup() {
  Serial.begin(115200);
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Setup done");
}

void loop() {
  Serial.println("scan start");

  
  int n = WiFi.scanNetworks();// WiFi.scanNetworks will return the number of networks found
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  // Wait a bit before scanning again
  delay(5000);
}

 

Open the Arduino Serial Monitor and Verify Operation
If operating properly,  the serial monitor will show the detected WiFi networks and their signal strength.

