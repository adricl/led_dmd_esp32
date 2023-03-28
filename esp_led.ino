/*
  Scrolling alphabet demo, displays characters one at a time into a scrolling box.
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>
#include <CircularBuffer.h>

#define ESP32


////////////////////////////////////////////////////////////////////
//// This might need to be changed in dm2 #define ESP32_PIN_NOE_PWM_CHANNEL 0

// WIFI code
const char* ssid = "cchs";
const char* password = "hackmelb";

const char* mqtt_server = "192.168.0.220";

WiFiClient espClient;
PubSubClient client(espClient);

// DM2 
// Set Width to the number of displays wide you have
const int WIDTH = 1;
// You can change to a smaller font (two lines) by commenting this line,
// and uncommenting the line after it:
const uint8_t *FONT = Arial14;
//const uint8_t *FONT = SystemFont5x7;

SPIDMD dmd(WIDTH,1);  // DMD controls the entire display
DMD_TextBox box(dmd);  // "box" provides a text box to automatically write to/scroll the display

//Message Buffer
CircularBuffer<char, 10000> messages;  
//const char *MESSAGE = "Hello World! ";

//Pin out for ESP32
// Arduino    Header   - ESP Pin    
// AREF       REF A    -    
// GND        GND      - GND
// D13        SCLK     - 18
// D12        D12      - 19
// D11        R        - 23
// D10        D10      - 
// D9         NOE      - 0      14
// D8         CLK      - 15     25
//
// D7         B        - 12     26
// D6         A        - 16     27 

// the setup routine runs once when you press reset:


void setup_wifi() {
  delay(10);
  //We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  if (String(topic) == "esp32/output") {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.println(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) {
      messages.push(message[i]);
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
    }
    Serial.println();
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

  Serial.begin(9600);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  

  setup_wifi();
  Serial.print("Setting Up Wifi");

  client.setServer(mqtt_server, 1883);
  Serial.print("Setting Up Mqtt Server");

  client.setCallback(callback);

  dmd.setBrightness(10);
  dmd.selectFont(FONT);
  dmd.begin(); 
  dmd.clearScreen();
}

// the loop routine runs over and over again forever:
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //get data from buffer and print it
  if (!messages.isEmpty()) {
    char letter = messages.shift();
    Serial.print(letter);
    box.print(letter);
  }
  delay(100);

}
