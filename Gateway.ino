#include <Arduino.h>

//Librarie for Wi-Fi
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <SPIFFS.h>

// Libraries to get time from NTP Server
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Librarie for MQTT
#include <PubSubClient.h>

//Pins used by the LoRa transceiver module
#define ss 18
#define rst 14
#define dio0 26

byte localAddress = 0xAA;
byte destinationAddress = 0xBB; //Endereço de destino a definir pelo utilizador
long lastSendTime = 0;
int interval = 2000;

// ESP32 AP
const char* ssid_esp = "ESP32_AP";
const char* password_esp = "1234567a";

//WiFi SSID/Password
const char* ssid = "";
const char* password = "";

//MQTT Broker IP Address
const char* mqtt_server = "";

//Libraries for the OLED display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RST -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//Logo
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x06, 0x60, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x30, 0x0c, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x60, 0x06, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x80, 0x01, 0x80, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x60, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x30, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x09, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x3b, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x73, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0xf3, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0xb3, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xd8, 0xe3, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x33, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc8, 0x03, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x83, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x83, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x83, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x83, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xc0, 0x83, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0x80, 0xc1, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0xe7, 0x3c, 0x3c, 0xfc, 0xf8, 0x78, 0x8c, 0xe3, 0xc0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x67, 0x66, 0x66, 0xec, 0xec, 0xcc, 0x8d, 0xb6, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0c, 0x26, 0x7f, 0xfe, 0xc6, 0xcd, 0x84, 0x8d, 0xe7, 0xf0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0c, 0x36, 0xfe, 0xfe, 0xc6, 0xcd, 0x84, 0x8c, 0x77, 0xf0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x06, 0x76, 0x66, 0xe4, 0xc6, 0xcc, 0xcc, 0xcd, 0xb6, 0x60, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xf6, 0x7c, 0x7c, 0xc6, 0xc4, 0xf8, 0xfd, 0xf3, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//IO38
#define OLED_BUTTON 38

//Button var
int buttonState = 0;

//JSON Librarie
#include <ArduinoJson.h>

//Initialize OLED display
void startOLED(){
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.drawBitmap(0, 0, myBitmap, 128, 64, WHITE); //Show Logo
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(40,10);
  display.print("LoRa");
  display.setCursor(20,30);
  display.print("Gateway");
  display.display();
  delay(2000);
}

WiFiClient espClient;
PubSubClient client(espClient);

// Initialize variables to get and save LoRa data
int recipient;
int sender;
int rssi;
String loRaMessage = "";

//Variables to save the LoRa data received
String readingID;
String temperature;
String humidity;
String luminosity;
String soilmoisture;
String battery_voltage;

//Variables to save the JSON data received through the MQTT topic
int param_esp_id;
float param_temp_ar;
float param_hum_ar;
float param_lumin;
float param_hum_solo;
int param_deepsleep_time;
int param_active_time;

//Variables to save the values of the AP Website
const char* PARAM_INPUT_1 = "SSID";
const char* PARAM_INPUT_2 = "Password";
const char* PARAM_INPUT_3 = "MQTT";

//Aux Variables to save the the WiFi credential and MQTT server's IP
String inputMessage = "No message sent";
String inputMessage1 = "No message sent";
String inputMessage2 = "No message sent";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    SSID: <input type="text" name="SSID">
  <p></p>
    Password: <input type="text" name="Password">
  <p></p>
    MQTT IP: <input type="text" name="MQTT">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

//Inicialize ESP32 Acces Point
void setup_AP() {
  delay(10);
  WiFi.softAP(ssid_esp, password_esp);

  Serial.println();
  Serial.print("AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(50,20);
  display.print("AP:");
  display.setCursor(30,30);
  display.print(WiFi.softAPIP());
  display.setCursor(30,40);
  display.print("Pass:" + String(password_esp));
  display.display();

  Serial.print("Waiting for WiFi credencials.");

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    //Save the information inserted through the AP Website
    if (request->hasParam(PARAM_INPUT_1) && (request->hasParam(PARAM_INPUT_2))) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      ssid = inputMessage.c_str();
    
      inputMessage1 = request->getParam(PARAM_INPUT_2)->value();
      password = inputMessage1.c_str();

      inputMessage2 = request->getParam(PARAM_INPUT_3)->value();
      mqtt_server = inputMessage2.c_str();
    }
    
    Serial.println(inputMessage);
    Serial.println(inputMessage1);
    Serial.println(inputMessage2);
    
    //Show the user a second Webpage with the data introduced
    request->send(200, "text/html", "Connect to the network named "+ inputMessage + " with the password: "
                                    + inputMessage1 + "MQTT Broker IP" + inputMessage2
                                    + ". If you want connect to another WiFi or MQTT server. Reset the ESP32"
                                    + "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

//Connect to the WiFi
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(35,10);
  display.print("WiFi Setup");
  display.setCursor(0,20);
  display.print("Connecting to: ");
  display.setCursor(0,30);
  display.print(ssid);
  display.display();
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(25,30);
    display.print("Connecting...");
    display.display();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20,10);
  display.print("WiFi connected!");
  display.setCursor(20,30);
  display.print(WiFi.localIP());
  display.display();
  delay(2000);
}

//Initialize LoRa module
void startLoRA(){
  //initialize Serial Monitor
  while (!Serial);
  Serial.println("LoRa Gateway");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }

  Serial.println("LoRa Initializing OK!");
}

//MQTT Subscriber
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print("\nMessage: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }

  Serial.println();

  if (String(topic) == "esp32_parameters")
  {   
    StaticJsonDocument<256> doc;
    deserializeJson(doc, message, length);
  
    param_esp_id = doc["param_esp_id"];
    
    if(param_esp_id == 187) {
      destinationAddress = 0xBB;
    }

    if(param_esp_id == 204) {
      destinationAddress = 0xCC;
    }
    
    param_temp_ar = doc["param_temp_ar"];
    param_hum_ar = doc["param_hum_ar"];
    param_lumin = doc["param_lumin"];
    param_hum_solo = doc["param_hum_solo"];
    param_deepsleep_time = doc["param_deepsleep_time"];
    param_active_time = doc["param_active_time"];
    Serial.println("Estufa ID:" + String(param_esp_id));
    Serial.print(" " + String(param_esp_id));
    Serial.print(" " + String(param_temp_ar));
    Serial.print(" " + String(param_hum_ar));
    Serial.print(" " + String(param_lumin));
    Serial.print(" " + String(param_hum_solo));
    Serial.print(" " + String(param_deepsleep_time));
    Serial.print(" " + String(param_active_time));  
  }
}

//MQTT Reconnecter
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection... ");
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20,20);
    display.print("Attempting MQTT");
    display.setCursor(20,30);
    display.print("connection...");
    display.display();
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("Connected!");
      // Subscribe
      client.subscribe("esp32_parameters");
      
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20,20);
      display.print("MQTT Connected!");
      display.setCursor(40,30);
      display.print("Server PI: ");
      display.setCursor(20,40);
      display.print(mqtt_server);
      display.display();
      delay(1000);
    }
    
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(20,20);
      display.print("Attempting MQTT");
      display.setCursor(20,30);
      display.print("connection...");
      display.display();
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//Sende LoRa Messages
void sendMessage(String outgoing) {
  LoRa.beginPacket();
  LoRa.write(destinationAddress);
  LoRa.write(localAddress);
  LoRa.write(outgoing.length());
  LoRa.print(outgoing);
  LoRa.endPacket();
}

//Receive LoRa Packages
void receiveMessage(int packetSize) {
  if (packetSize == 0) return;

  recipient = LoRa.read();
  sender = LoRa.read();
  byte incomingLength = LoRa.read();

  if (recipient != localAddress) {
    Serial.println("Error: Recipient address does not match local address");
    return;
  }

  String incoming = "";

  //Decode LoRa message
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
    int pos1 = incoming.indexOf('/');
    int pos2 = incoming.indexOf('&');
    int pos3 = incoming.indexOf('!');
    int pos4 = incoming.indexOf('#');
    int pos5 = incoming.indexOf('|');
    readingID = incoming.substring(0, pos1);
    temperature = incoming.substring(pos1 +1, pos2);
    humidity = incoming.substring(pos2+1, pos3);
    luminosity = incoming.substring(pos3+1, pos4);
    soilmoisture = incoming.substring(pos4+1, pos5);
    battery_voltage = incoming.substring(pos5+1, incoming.length());
  }
  
  if (incomingLength != incoming.length()) {
      Serial.println("Error: Message length does not match length");
      return;
  }

  Serial.print("Received data " + incoming);
  Serial.print(" from 0x" + String(sender, HEX));
  Serial.print(" to 0x" + String(recipient, HEX));
  
  // Get RSSI
  rssi = LoRa.packetRssi();
  Serial.print(" with RSSI ");
  Serial.println(rssi);

  Serial.println("Temp.: "+ temperature);
  Serial.println("Humd.: " + humidity);
  Serial.println("Lumin.: " + luminosity);
  Serial.println("Soil Moisture.: " + soilmoisture);
  Serial.println("Battery: " + battery_voltage);
}

void setup() {
  Serial.println("LoRa Receiver");
  Serial.begin(115200);
  startOLED();
  startLoRA();
  setup_AP();
  
  while(inputMessage == "No message sent" && inputMessage1 == "No message sent" && inputMessage2 == "No message sent") {
    Serial.print(".");
    delay(500);
  }

  setup_wifi();
  
  //Connecting to the MQTT server
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  
  delay(300);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Package Received:");
  
  if(sender == 187) {
    display.setCursor(0,10);
    display.print("#" + String(sender) + " ReadingID:" + String(readingID));
  }
  if(sender == 204) {
    display.setCursor(0,20);
    display.print("#" + String(sender) + " ReadingID:" + String(readingID));
  }
  display.display();
  
  if (millis() - lastSendTime > interval) {
    String LoRaData = String(param_temp_ar) + "?" + String(param_hum_ar) + "%" + String(param_lumin) + "." + String(param_hum_solo) + "=" + String(param_deepsleep_time) + "+" + String(param_active_time);
    
    sendMessage(LoRaData);

    Serial.print("Sending data " + LoRaData);
    Serial.print(" from source 0x" + String(localAddress, HEX));
    Serial.println(" to destination 0x" + String(destinationAddress, HEX));

    lastSendTime = millis();
    interval = random(2000) + 1000;
  }

  receiveMessage(LoRa.parsePacket());

  //JSON payload
  String payload = "{\"id\":" + String(sender) +
                   ",\"battery\":" + String(battery_voltage) +
                   ",\"temperature\":" + String(temperature) +
                   ",\"humidity\":" + String(humidity) +
                   ",\"luminosity\":" + String(luminosity) +
                   ",\"soilmoisture\":" + String(soilmoisture) +
                   "} ";

  //Convert JSON to char
  static char json_payload[160];
  payload.toCharArray(json_payload, (payload.length() + 1));
  client.publish("esp32_data", json_payload);

  if(!client.loop())
    client.connect("ESP32Client");
}
