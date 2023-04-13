// includes
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <stdlib.h>

#include "DHT.h"
#include "ikony.h"
#include "index.html.h"
#include "styles.css.h"
#include "scripts.js.h"
#include "timer.h"

// defines
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16

#define DHTPIN 5
#define DHTTYPE DHT11

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1  // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS \
  0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
 
Timer myTimer(5000);

// Initializing display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Initializing the DHT sensor
DHT dht(DHTPIN, DHTTYPE);
float h, t;

// Wifi creditendials
const char* ssid = "MITAI2"; // "WiFi_Mateas";
const char* password = "sstvsvl1"; // "11211121";

// Initializing the object of server
ESP8266WebServer server(80);

// delay without delay
unsigned long previousMillis = 0;

// Helping functions
void nastavDisplay() {
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void pripojSaNaWifi() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.print(".");
    display.display();
  }
  Serial.println(WiFi.localIP());
}

void ukazDetailyWifi() {
  display.clearDisplay();
  display.println("");
  display.println("Connected to:");
  display.println(ssid);
  display.println("");
  display.println("IP address:");
  display.println(WiFi.localIP());
  display.display();
}

void dajHodnoty() {
  if (myTimer.DoneYet()) {
    myTimer.Mark();

    h = dht.readHumidity();
    t = dht.readTemperature();
  }
}

void zobrazHodnoty() {
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  display.clearDisplay();

  display.drawRect(0, 0, display.width(), display.height(), WHITE);

  display.drawBitmap(5, 15, teplota_icon, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(25, 20);
  display.println("Teplota: " + String(t, 1) + String(char(247)) + "C");

  display.drawBitmap(5, 35, vlhkost_icon, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.setCursor(25, 40);
  display.println("Vlhkost: " + String(h, 0) + "%");

  display.display();
}

void defaultPoziadavka() {
  String fullhtml = String(mojeHtml);
  fullhtml.replace("{TEPLOTA}", String(t, 1));
  fullhtml.replace("{VLHKOST}", String(h, 1));
  fullhtml.replace("{IP_ADDRESS}", WiFi.localIP().toString().c_str());
  server.send(200, "text/html", fullhtml);
}

void cssPoziadavka() {
  String fullcss = String(mojeCss);
  server.send(200, "text/css", fullcss);
}

void javascriptPoziadavka(){
  String fulljs = String(mojeJs);
  server.send(200, "text/javascript", fulljs);
}

void dataPoziadavka() {
  if (isnan(h) || isnan(t)) {
    server.send(500, "text/plain", "Internal server error");
    return;
  }

  String units = server.arg("units");
  bool isFahrenheit = units.equalsIgnoreCase("f");

  // Construct the JSON object
  StaticJsonDocument<200> jsonDoc;

  if (isFahrenheit){
    jsonDoc["teplota"] = atof(String(t * 1.8 + 32, 1).c_str());
  } else {
    jsonDoc["teplota"] = atof(String(t, 1).c_str());;
  }
  jsonDoc["vlhkost"] = h;

  // Serialize the JSON object to a string
  String response;
  serializeJson(jsonDoc, response);

  server.send(200, "application/json", response);
}

void nenajdenaPoziadavka() {
  // Create a JSON object
  StaticJsonDocument<200> jsonDoc;

  // Set the properties of the JSON object
  jsonDoc["message"] = "File Not Found";
  jsonDoc["uri"] = server.uri();
  jsonDoc["method"] = (server.method() == HTTP_GET) ? "GET" : "POST";
  jsonDoc["arguments"] = server.args();

  // Add the arguments as properties to the JSON object
  JsonObject args = jsonDoc.createNestedObject("args");
  for (uint8_t i = 0; i < server.args(); i++) {
    args[server.argName(i)] = server.arg(i);
  }

  // Serialize the JSON object to a string
  String response;
  serializeJson(jsonDoc, response);

  // Send the JSON response
  server.send(404, "application/json", response);
}

void setup() {
  Wire.begin(14, 12);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  delay(200);
  
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }

  // Display setup
  nastavDisplay();

  // Wait for connection
  pripojSaNaWifi();

  // When we're connected show config details
  ukazDetailyWifi();

  // Bind the routes
  // NOTE: Self-executed function is done like this: [](){}
  server.on("/", defaultPoziadavka);
  server.on("/data", dataPoziadavka);
  server.on("/styles.css", cssPoziadavka);
  server.on("/script.js", javascriptPoziadavka);
  server.onNotFound(nenajdenaPoziadavka);

  // Start server
  server.begin();

  // We will 5 seconds in which we will see the ip and ssid of/from our wifi
  delay(5000);
}

void loop() {
  server.handleClient();
  dajHodnoty();
  zobrazHodnoty();
}
