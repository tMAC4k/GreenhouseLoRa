//Librarie to save values on the EEPROM
#include <EEPROM.h>
#define EEPROM_SIZE 1

//Librarie pthreads
#include <pthread.h>

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

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

//Librarie for the DHT11 Sensor
#include "DHT.h"

//Pins used by the LoRa transceiver module
#define ss 18
#define rst 14
#define dio0 26

byte localAddress = 0xBB;
byte destinationAddress = 0xAA;
long lastSendTime = 0;
int interval = 2000;

#define DHTTYPE DHT11   // DHT 11

const int DHTPin = 4;

DHT dht(DHTPin, DHTTYPE);

//LDR Pin
#define LDR 0

//Soil Moisture Sensor
#define SOILMOIST 23

//Battery Level Pin
#define VOLTAGE_PIN 33

//IO38
#define OLED_BUTTON 38

//Button var
int buttonState = 0;

// LED Pin
const int ledPin = 2;

//Waterpump Pin
const int ledPin1 = 13;

//Ventilation Pin
const int ledPin2 = 14;

//Variables
float t;
float h;
int d;
int s;
int ldr;
float baterry_voltage;
int count = 1; //Aux Variable to choose the disp. address
volatile boolean enablePump = 0;
volatile boolean enableVent = 0;
volatile boolean enableLED = 0;

// Initialize variables to get and save LoRa data
String LoRaMessage = "";
String param_esp_id;
String param_temp_ar;
String param_hum_ar;
String param_lumin;
String param_hum_solo;
String param_deepsleep_time;
String param_active_time;
int rssi;

//packet counter
int readingID = 0;

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

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

  //Display message: "LoRa Sender"
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(40,10);
  display.print("LoRa");
  display.setCursor(30,30);
  display.print("Sender");
  display.display();
  delay(2000);
}

//Inicialize LoRa
void startLoRA(){
  //initialize Serial Monitor
  while (!Serial);
  Serial.println("LoRa Sender");

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

//Send LoRa Package
void sendMessage(String outgoing) {
  LoRa.beginPacket();
  LoRa.write(destinationAddress);
  LoRa.write(localAddress);
  LoRa.write(outgoing.length());
  LoRa.print(outgoing);
  LoRa.endPacket();
}

//Recieve LoRa Packages
void receiveMessage(int packetSize) {
  if (packetSize == 0) return;

  int recipient = LoRa.read(); //Recipient of the LoRa message
  byte sender = LoRa.read(); //Sender of the LoRa message
  byte incomingLength = LoRa.read(); //Size of the message
  
  if (recipient != localAddress) {
    Serial.println("Error: Recipient address does not match local address");
    return;
  }
  
  String incoming = "";

  //Decode LoRa message
  while (LoRa.available()) {
    incoming += (char)LoRa.read();   
    int pos1 = incoming.indexOf('?');
    int pos2 = incoming.indexOf('%');
    int pos3 = incoming.indexOf('.');
    int pos4 = incoming.indexOf('=');
    int pos5 = incoming.indexOf('+');
    param_temp_ar = incoming.substring(0, pos1);
    param_hum_ar = incoming.substring(pos1+1, pos2);
    param_lumin = incoming.substring(pos2+1, pos3);
    param_hum_solo = incoming.substring(pos3+1, pos4);
    param_deepsleep_time = incoming.substring(pos4+1, pos5);
    param_active_time = incoming.substring(pos5+1, incoming.length());
  }

  if (incomingLength != incoming.length()) {
      Serial.println("Error: Message length does not match length");
      return;
  }
  
  Serial.print("Received data " + incoming);
  Serial.print(" from 0x" + String(sender, HEX));
  Serial.println(" to 0x" + String(recipient, HEX));
}

//Display the informations of the sensors
void displayinfo() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(20,0);
  display.print("Greenhouse#" + String(localAddress));
  
  display.setCursor(0,10);
  display.print("Temp:" + String(t, 1) + "C" + "  Hum:" + String(h, 1) + "%");
  
  display.setCursor(0,20);
  display.print("Lumin:" + String(ldr) + "%   SoilM:" + String(s) + "%");

  display.setCursor(0,30);
  display.print("   DeepSleep:" + String(param_deepsleep_time) + "sec");

  display.setCursor(0,40);
  rssi = LoRa.packetRssi();
  display.print("Batt:" + String(baterry_voltage, 1) + "V" + "   RSSI:" + String(rssi));

  display.setCursor(0,50);
  display.print("    ReadingID:" + String(readingID));
  readingID++;
  display.display();
}

//Read Battery Voltage
float readVoltage() {
  float voltage = (float)analogRead(36)/4095;
  return voltage * 5.9; //0-5V range
  //return voltage * 7.1; //0-6V range
  //return voltage * 14.6; //0-12V range
}

//Pthread Function
void *param_auto(void *arg){
  while(1){
    if(enableLED == 1) {
        Serial.println("It's getting dark!");
        Serial.println("Turning the lights on...");
        digitalWrite(ledPin, HIGH);
  
        delay(5000);
  
        digitalWrite(ledPin, LOW);
        Serial.println("Turning the light back off!");
        enableLED = 0;
    }
    
    if(enablePump == 1) {
        Serial.println("Dry Soil!");
        Serial.println("Turning on pump...");
        digitalWrite(ledPin1, HIGH);
  
        delay(5000);
  
        digitalWrite(ledPin1, LOW);
        Serial.println("Turning off pump!");
        enablePump = 0;
    }

    if(enableVent == 1) {
        Serial.println("Humidity levels high!");
        Serial.println("Truning on ventilation...");
        digitalWrite(ledPin2, HIGH);
  
        delay(5000);
  
        digitalWrite(ledPin2, LOW);
        Serial.println("Rega Terminada!");
        enableVent = 0;
    }

    delay(50); //Delay com função de yield para o ESP executar funções background
  }
}

void setup() {
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(OLED_BUTTON, INPUT);
  pinMode(SOILMOIST, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  Serial.begin(115200);
  startOLED();
  dht.begin();
  EEPROM.begin(EEPROM_SIZE);
  localAddress = EEPROM.read(0);
  startLoRA();
  
  //Initialize Thread
  pthread_t thread;
  pthread_create(&thread, NULL, param_auto, NULL);
}

void loop() {
  d = param_deepsleep_time.toInt();
  t = dht.readTemperature();
  h = dht.readHumidity();
  int ldr_aux = (4095 - analogRead(LDR));
  ldr = map(ldr_aux, 826, 2000, 0 ,100);
  
  //int aux;
  
  /*//Reading soil moisture sensor
  for(int i = 0; i<64; i++){
    s = analogRead(SOILMOIST);
    aux = aux + s;
  }
  s = (aux/64);
  s = map(s, 250, 1023, 100, 0);*/

  s = 70; //Representative Value
  
  //Check for error in the DHT Sensor
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
  }
   
  //Check if water is needed and activate waterpump
  if(s < (param_hum_solo.toInt()) && enablePump == 0){
     enablePump = 1;
  }

  //Check the air humidity
  if(h > (param_hum_ar.toInt()) && enableVent == 0){
      enableVent = 1;
  }

  //Check if it's getting dark outside to turn on the lights
  if(ldr < (param_lumin.toInt()) && enableLED == 0){
      enableLED = 1;
  }
  
  //Serial.println(readVoltage());
  //baterry_voltage = readVoltage();
  
  baterry_voltage = 3.3;  //Representative Value

  //Send the LoRa message at a random moment so the duplex system works
  if (millis() - lastSendTime > interval) {
    LoRaMessage = String(readingID) + "/" + String(t) + "&" + String(h) + "!" + String(ldr) + "#" + String(s) + "|" + String(baterry_voltage);
    sendMessage(LoRaMessage);

    displayinfo();

    Serial.print("Sending data " + LoRaMessage);
    Serial.print(" from 0x" + String(localAddress, HEX));
    Serial.println(" to 0x" + String(destinationAddress, HEX));

    lastSendTime = millis();
    interval = random(2000) + 1000;
  }

  //Receive the LoRa message
  receiveMessage(LoRa.parsePacket());

  // Change the local address of the ESP32
  buttonState = digitalRead(OLED_BUTTON);
  if(buttonState == LOW) {
    if(count == 1) {
      localAddress = 0xBB;
      EEPROM.put(0, localAddress);
      EEPROM.commit();
    }

    else if(count == 2) {
      localAddress = 0xCC;
      EEPROM.put(0, localAddress);
      EEPROM.commit();
    }
    
    if(count >= 2) {
      count = 0;
    }
    
    count++;

    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(1,1);
    display.print("  #");
    display.print(localAddress);
    display.display();
    delay(1000);
    display.clearDisplay();
  }
  
  //The ESP will sleep max 15h and with (atleast) over 10 mesages sent between deepsleeps 
  if(readingID > param_active_time.toInt() && param_active_time.toInt() > 10 && param_deepsleep_time.toInt() != 0 && param_deepsleep_time.toInt() < 54001) {
    if(enablePump == 0 && enableLED == 0) { //enableVent == 0 &&
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,20);
      display.print("  Going to sleep now");
      display.display();
      delay(500);
      display.clearDisplay();
      digitalWrite(OLED_RST, LOW);
      Serial.println("Going to sleep now");
      esp_sleep_enable_timer_wakeup(param_deepsleep_time.toInt() * 1000000);
      esp_deep_sleep_start();
    }

    else {
      readingID = 0;
      Serial.println("Deepsleep is not possible at the moment, task are still beign executed");
    }
  }
}