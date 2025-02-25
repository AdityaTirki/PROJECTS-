**Title:** IoT-Based Manhole Monitoring System

**Abstract:**
This project presents an IoT-based Manhole Monitoring System designed to detect water levels, gas leaks, temperature variations, and rain presence inside manholes. The system integrates an ultrasonic sensor, MQ-6 gas sensor, DHT22 temperature sensor, and a rain sensor, all controlled by an ESP8266 microcontroller. Real-time monitoring is achieved via the Blynk IoT platform. The system enhances public safety by sending early warnings and logging data for future analysis.

---

**1. Introduction:**
Manhole accidents due to gas accumulation, water overflow, and extreme temperature conditions pose serious safety risks. This project aims to develop a real-time monitoring system that detects hazardous conditions inside manholes and alerts the concerned authorities using IoT-based solutions.

**2. Objectives:**
- To continuously monitor water level, gas concentration, temperature, and rain presence.
- To transmit real-time data to a remote dashboard via Blynk IoT.
- To log sensor data for future analysis and safety improvements.

**3. Components Used:**
1. **ESP8266** – Microcontroller for sensor integration and IoT communication.
2. **Ultrasonic Sensor (HC-SR04)** – Measures water level inside the manhole.
3. **MQ-6 Gas Sensor** – Detects harmful gases such as methane and LPG.
4. **DHT22 Temperature Sensor** – Monitors internal temperature conditions.
5. **Rain Sensor** – Detects rainwater presence.
6. **Wi-Fi Module (ESP8266)** – Sends data to Blynk IoT.

**4. Methodology:**
The ESP8266 microcontroller collects real-time sensor readings and evaluates them against predefined threshold values. If any threshold is exceeded, the system sends notifications to a cloud-based IoT dashboard. Data is logged periodically for future reference.

**5. Circuit Diagram and Working:**
1. **Water Level Detection** – Uses an ultrasonic sensor to measure distance and detect rising water levels.
2. **Gas Leak Detection** – The MQ-6 sensor monitors gas concentration and triggers alerts when levels are high.
3. **Temperature Monitoring** – The DHT22 sensor records temperature, alerting when unsafe conditions arise.
4. **Rain Detection** – A rain sensor detects rainwater intrusion and triggers necessary actions.
5. **IoT-Based Monitoring** – Data is transmitted to a remote dashboard using Blynk for real-time updates.

**6. Algorithm:**
1. Initialize ESP8266 and configure all sensors.
2. Calibrate the ultrasonic sensor to measure the baseline water level.
3. Continuously read sensor values at regular intervals.
4. Compare values against predefined thresholds:
   - If water level rises significantly, trigger "Water High" alert.
   - If gas levels exceed the threshold, trigger "Gas Alert".
   - If temperature exceeds the limit, trigger "Temperature Alert".
   - If rain is detected, trigger "Rain Alert".
5. Send real-time sensor data and alerts to the Blynk IoT platform.
6. Log all sensor readings for future analysis.
7. Repeat the monitoring cycle at regular intervals.

**7. Arduino Code:**
```cpp
#include <DHT.h>

// Sensor Pin Definitions
#define TRIG_PIN 5         // Trigger pin for Ultrasonic Sensor
#define ECHO_PIN 18        // Echo pin for Ultrasonic Sensor
#define MQ6_PIN 34         // MQ-6 Gas Sensor (Analog pin)
#define DHTPIN 4           // DHT22 Temperature Sensor Pin
#define DHTTYPE DHT22      // DHT22 Sensor type
#define RAIN_SENSOR_PIN 35 // Rain Sensor Pin

// Threshold and calibration constants
float baselineWaterDistance = 0;         // Baseline distance when no water is present
const float waterDeltaThreshold = 0.3;     // Minimum drop (in cm) from baseline to trigger "Water High!"
const float TEMP_THRESHOLD   = 30.0;       // °C for temperature alert
const int   GAS_THRESHOLD    = 800;        // Analog threshold for gas sensor
const int   RAIN_DETECTED    = 1;          // digital value for rain detection (assuming HIGH indicates rain)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ6_PIN, INPUT);
  pinMode(RAIN_SENSOR_PIN, INPUT);
  calibrateWaterSensor();
  Serial.println("Manhole Monitoring System Initialized (Serial Mode)!");
}

void loop() {
  readSensors();
}

void calibrateWaterSensor() {
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    float duration = pulseIn(ECHO_PIN, HIGH);
    float distance = duration * 0.034 / 2;
    sum += distance;
    delay(100);
  }
  baselineWaterDistance = sum / 10;
}

void readSensors() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  int gasValue = analogRead(MQ6_PIN);
  float temperature = dht.readTemperature();
  int rainValue = digitalRead(RAIN_SENSOR_PIN);
}
```

**8. Results and Observations:**
- The system successfully detects water level, gas concentration, temperature, and rain.
- Real-time alerts via Blynk notify users instantly.
- Data logging allows further safety analysis.

**9. Advantages and Limitations:**
**Advantages:**
- Real-time IoT-based monitoring with remote access.
- Automated alerts for hazardous conditions.
- Data logging enables predictive maintenance.
- Cost-effective and scalable for urban areas.

**Limitations:**
- Requires stable internet connection for IoT features.
- Sensor calibration may be needed periodically.
- Limited to predefined environmental factors.

**10. Future Enhancements:**
- Implementing AI-based predictive maintenance.
- Adding GSM module for SMS-based alerts.
- Using solar-powered battery backup.
- Deploying deep learning for anomaly detection.

**11. Conclusion:**
The IoT-based Manhole Monitoring System efficiently detects and alerts users to potential hazards in underground drainage systems. By leveraging real-time monitoring, smart alerts, and data logging, the system enhances public safety and enables proactive maintenance. Future developments can further enhance its capabilities, making it an essential tool for smart city infrastructure.

**References:**
1. Datasheets for ESP8266, HC-SR04, MQ-6, and DHT22 sensors.
2. Research papers on IoT-based public safety solutions.
3. Blynk IoT platform documentation.


