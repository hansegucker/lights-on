#include <Keypad.h>
#include <SD.h>
#include "driver/i2s.h"
#include "esp_wifi.h"
#include <WiFi.h>
#include "esp_task.h"

// Pin-Nummer der Power-LED
const int POWER_LED_PIN = 16;

// Name des vom ESP aufgespannten WLANs
#define AP_SSID "espsoundbox"

// Variablen für die Player-Task
String currentFile;
bool doPlay = false;
bool hasChanged = false;

// I2S-Konfiguration
const int I2S_PORT_NUM = 0;

static const i2s_config_t i2s_config = {
  .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),
  .sample_rate = 44100,
  .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
  .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
  .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
  .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
  .dma_buf_count = 6,
  .dma_buf_len = 60
};

i2s_pin_config_t pin_config = {
  .bck_io_num = 26, //BCK-Pin
  .ws_io_num = 25, // LRCK-Pin
  .data_out_num = 22, // Daten-Pin
  .data_in_num = I2S_PIN_NO_CHANGE // Wird nicht benötigt
};

// Konfiguration des Tastenfelds
const byte N_ROWS = 4; 
const byte N_COLS = 4; 
char KEY_MAPPINGS[N_ROWS][N_COLS] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'A', 'B'},
  {'C', 'D', 'E', 'F'}
};

byte ROW_PINS[N_ROWS] = {15, 27, 0, 4}; // Pins der Zeilen des Tastenfelds
byte COL_PINS[N_COLS] = {17, 32, 13, 14}; // Pins der Reihen des Tastenfelds

Keypad keypad = Keypad(makeKeymap(KEY_MAPPINGS), ROW_PINS, COL_PINS, N_ROWS, N_COLS);

// Webserver lauscht auf Port 80
WiFiServer server(80);

// Daten von der SD-Karte und schreibt sie in den I2S-Buffer
void playFile(void *) {
  uint8_t file_buf[60];
  int file_buf_len;
  int file_int;
  File wavefile;
  while (true) {
    // doPlay wird true, wenn jemand über das Webinterface oder das Keypad eine neue Wave-Datei gewählt hat
    if (doPlay) {
      // Öffnen der WAV-Datei
      Serial.println("doPlay: " + currentFile);
      wavefile = SD.open(currentFile);
      // Wenn die Datei geöffnet wurde, mit dem Lesen anfangen
      if (wavefile) {
        Serial.println("doPlay: " + currentFile);
        // Datei in Blöcken von 60 Bytes lesen, bis die Datei endet oder die Variable hasChanged wahr ist.
        while (wavefile.available() && hasChanged == false) {
          file_buf_len = sizeof(file_buf);
          wavefile.read(file_buf, file_buf_len);
          // gelesene Bytes in den I2S-Buffer schreiben. Wenn das Schreiben länger als 500 ms dauert, wird der Vorgang abgebrochen
          i2s_write_bytes((i2s_port_t)I2S_PORT_NUM, (const char *)&file_buf, file_buf_len, 500);
        }
        // Datei schließen und I2S-Buffer leeren
        wavefile.close();
        i2s_zero_dma_buffer((i2s_port_t)I2S_PORT_NUM);
        Serial.println("Fertig abgespielt.");
      }
      else {
        Serial.println(currentFile + " nicht gefunden");
      }
      if (hasChanged) {
        doPlay = true;
        hasChanged = false;
      }
      else {
        doPlay = false;
      }
    }
    else {
      // Strom sparen
      vTaskDelay(10);
    }
  }
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED: // Knopf gedrückt
      String keynum = (String)key;
      // Abzuspielende Wav-Datei festlegen
      currentFile = "/" + keynum + ".wav";
      Serial.println("Taste " + keynum + " gedrückt");
      // playFile() mitteilen, dass Datei abgespielt werden soll
      if (!doPlay) {
        hasChanged = false;
      }
      else {
        hasChanged = true;
      }
      doPlay = true;
  }
}


// Angefragte URL aus dem Request des Clients extrahieren
String get_location(String req) {
  // Position von 'GET /' suchen
  int addr_start = req.indexOf("GET /");
  // Position von ' HTTP' suchen
  int addr_end = req.indexOf(" HTTP", addr_start + 1);
  // Text dazwischen zurückgeben
  if (addr_start != -1 || addr_end != -1) {
    return req.substring(addr_start + 1, addr_end - 4);
  }
  return "";
}


// Webserver Task
void webServerTask(void *) {
  // HTML erzeugen
  String html =
    "<!DOCTYPE html><html><head>"
    "<title>c't-Soundbox</title>"
    "<script>function h(n){var e=new XMLHttpRequest;e.open(\"GET\",n,1),e.send(null)}</script>"
    "<style>p{width:100%;display:flex;}a,.a:hover{text-decoration:none}a{color:#fff;padding:10px;background:#3498db;flex-grow:1;text-align:center;margin:10px;}a:hover{background:#3cb0fd}</style>"
    "</head>"
    "<body>";
  for (int i = 0; i < N_ROWS; ++i) {
    html += "<p>";
    for (int j = 0; j < N_COLS; ++j) {
      html += "<a href=\"#\" onclick=\"h('/" + (String)KEY_MAPPINGS[i][j] + "')\" title=\"" + (String) KEY_MAPPINGS[i][j] + "\">&nbsp;</a>";
    }
    html += "</p>";
  }
  html += "</body></html>";
  while (true) {
    WiFiClient client = server.available(); // Warten auf Anfragen von Clients
    if (client) {
      String url;
      String c;
      Serial.println("New client");           
      if (client.connected()) {            
        if (client.available()) {             
          c = client.readString();
        }
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type: text/html");
        client.println();
        client.print(html);
        client.println();
        client.stop();
        url = get_location(c);          
        Serial.println(url);
      }
      if (url != "") {
        currentFile = "/" + url + ".wav";
        if (!doPlay) {
          hasChanged = false;
        }
        else {
          hasChanged = true;
        }
        doPlay = true;
      }
    }
    // Unterbricht den Task für 10ms, um Strom zu sparen
    vTaskDelay(10);
  }
}


void setup() {
  // Aktivieren der seriellen Schnittstelle
  Serial.begin(115200);
  // Aktivieren der Power-LED
  pinMode(POWER_LED_PIN, OUTPUT);
  digitalWrite(POWER_LED_PIN, HIGH);
  // Konfiguration des I2S-Chips. Die Einstellungen sind in i2s_config und i2s_pin_config festgelegt.
  i2s_driver_install((i2s_port_t)I2S_PORT_NUM, &i2s_config, 0, NULL);
  i2s_set_pin((i2s_port_t)I2S_PORT_NUM, &pin_config);
  i2s_zero_dma_buffer((i2s_port_t)I2S_PORT_NUM);
  // Initialisierung der SD-Karte
  bool sd_status = SD.begin();
  delay(500);
  if (!sd_status) {
    Serial.println("SD not found");
    esp_restart();
  }
  // Player als Task starten
  xTaskCreate(&playFile, "playFile", 10000, NULL, 1, NULL);
  // Event Listener erzeugen, der auf Tastendrücke reagiert
  keypad.addEventListener(keypadEvent);

  // WLAN Access Point aktivieren
  WiFi.softAP(AP_SSID);
  // Webserver auf Port 80 starten
  server.begin();
  // WLAN-Stromsparmodus aktivieren
  esp_wifi_set_ps((wifi_ps_type_t)1);
  Serial.println("Access Point active");
  // Webserver als Task starten
  xTaskCreate(&webServerTask, "webServerTask", 10000, NULL, 5, NULL);
}


void loop() {
  // Tastenfeld nach Tastendrücken abfragen
  keypad.getKey();
  // Schleife für 10ms unterbrechen, um Strom zu sparen
  vTaskDelay(10);
}

