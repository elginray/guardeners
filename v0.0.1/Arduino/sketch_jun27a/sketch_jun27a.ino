#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

// Firebase configuration
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// Light Intensity
double calcLightIntensity(int AnVal, int Resistor) {
  double Vout = AnVal * 0.0048828125;
  int lux = ((2500 / Vout - 500) / Resistor);
  return lux;
}

// Humidity and Temperature
#define DHTPIN 7     // Pin 7 will measure the humidity and temperature
#define DHTTYPE DHT22
DHT dhtObject(DHTPIN, DHTTYPE);

// Soil Temperature
#define ONE_WIRE_BUS 2 // Pin 2 will measure the soil temperature
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Soil Moisture
const int Sensor = A1; // Pin A1 (GPIO1) will measure the soil moisture
#define wet 210
#define dry 510

void setup() {
  Serial.begin(9600);

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected");
  Serial.print(WiFi.localIP());

  // Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Soil Temperature
  sensors.begin();

  // Humidity and Temperature
  dhtObject.begin();
}

void loop() {
  // Light Intensity
  int analogVal = analogRead(A0); // Read values from A0
  int Resistance = 10; // I am using a 10k ohms resistor

  Serial.print("Light Intensity: ");
  Serial.print(int(calcLightIntensity(analogVal, Resistance)));
  Serial.println(" lux");

  delay(500);

  // Humidity and Temperature
  float h = dhtObject.readHumidity();
  float t = dhtObject.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %, Temp: ");
  Serial.print(t);
  Serial.println(" ° Celsius");

  delay(500);

  // Soil Temperature
  sensors.requestTemperatures();
  Serial.print("Soil Temperature: ");
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println(" ° Celsius");

  delay(500);

  // Soil Moisture
  int value = analogRead(Sensor);
  Serial.println(value);
  int soilHumidity = map(value, wet, dry, 100, 0);
  Serial.print("Soil Humidity: ");
  Serial.print(soilHumidity);
  Serial.println(" %");

  delay(500);
}
