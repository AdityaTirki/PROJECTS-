// Blynk Template Credentials
#define BLYNK_TEMPLATE_ID "TMPL3T58dQKhf"
#define BLYNK_TEMPLATE_NAME "Temperature"
#define BLYNK_AUTH_TOKEN "r_IBcaO0XDcTCmRkD3sc6hP3Hqo79kAL"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// WiFi credentials
#define WIFI_SSID "OPPO A55"
#define WIFI_PASS "11223344"

// DHT configuration
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Relay pins
#define RELAY_COOLER 18
#define RELAY_HEATER 19

// Blynk Virtual Pins
#define VPIN_TEMP V0
#define VPIN_HUM V1
#define VPIN_STATUS V2
#define VPIN_HEATER V3
#define VPIN_COOLER V4

BlynkTimer timer;

void sendSensorData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  // Log readings
  Serial.print("🌡️ Temp: ");
  Serial.print(temperature);
  Serial.print(" °C | 💧 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Send to Blynk
  Blynk.virtualWrite(VPIN_TEMP, temperature);
  Blynk.virtualWrite(VPIN_HUM, humidity);

  // Relay Logic
  if (temperature < 37) {
    digitalWrite(RELAY_COOLER, HIGH);  // Cooler OFF
    digitalWrite(RELAY_HEATER, LOW);   // Heater ON

    Blynk.virtualWrite(VPIN_STATUS, "🔥 Heater ON, Cooler OFF");
    Blynk.virtualWrite(VPIN_HEATER, "ON");
    Blynk.virtualWrite(VPIN_COOLER, "OFF");

    Serial.println("🔥 Heater ON, Cooler OFF");
  } else if (temperature > 38) {
    digitalWrite(RELAY_COOLER, LOW);   // Cooler ON
    digitalWrite(RELAY_HEATER, HIGH);  // Heater OFF

    Blynk.virtualWrite(VPIN_STATUS, "❄️ Cooler ON, Heater OFF");
    Blynk.virtualWrite(VPIN_HEATER, "OFF");
    Blynk.virtualWrite(VPIN_COOLER, "ON");

    Serial.println("❄️ Cooler ON, Heater OFF");
  } else {
    digitalWrite(RELAY_COOLER, HIGH);  // Cooler OFF
    digitalWrite(RELAY_HEATER, HIGH);  // Heater OFF

    Blynk.virtualWrite(VPIN_STATUS, "✅ Temp Normal – Both OFF");
    Blynk.virtualWrite(VPIN_HEATER, "OFF");
    Blynk.virtualWrite(VPIN_COOLER, "OFF");

    Serial.println("✅ Temp Normal – Both OFF");
  }

  Serial.println("------------------------------------------------");
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_COOLER, OUTPUT);
  pinMode(RELAY_HEATER, OUTPUT);

  digitalWrite(RELAY_COOLER, HIGH);  // OFF
  digitalWrite(RELAY_HEATER, HIGH);  // OFF

  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected");

  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);

  timer.setInterval(3000L, sendSensorData);  // Every 3 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
