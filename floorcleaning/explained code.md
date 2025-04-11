🔧 Code Explanation for Floor Cleaning Robot with Blynk + ESP32
1. Blynk Template & Wi-Fi Setup
cpp
Copy
Edit
#define BLYNK_TEMPLATE_ID ...
#define BLYNK_TEMPLATE_NAME ...
#define BLYNK_AUTH_TOKEN ...
These define your Blynk project identity to sync with the Blynk IoT cloud.

Wi-Fi credentials (ssid and pass) are used to connect the ESP32 to the internet.

2. Library Inclusions
cpp
Copy
Edit
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
Includes necessary libraries for WiFi, Blynk, and DHT11 temperature/humidity sensor.

3. Pin Definitions
cpp
Copy
Edit
#define IN1, IN2, IN3, IN4     // DC motor direction pins
#define ENA, ENB               // Motor speed PWM pins
#define RELAY_PIN 23           // Controls vacuum or mop motor via relay
#define DHTPIN 13              // DHT11 data pin
#define mq3Pin 34              // Air quality analog input pin
#define TRIG_LEFT, ECHO_LEFT   // Ultrasonic sensor (left)
#define TRIG_RIGHT, ECHO_RIGHT // Ultrasonic sensor (right)
Pins for DC motor, sensors, and relay control are assigned clearly for hardware wiring.

4. Global Variables
cpp
Copy
Edit
int speedValue = 200;         // Default motor speed (PWM: 0–255)
bool movingForward = false;   // Flag to track forward motion
BlynkTimer timer;             // Timer for scheduled tasks
5. setup() Function
cpp
Copy
Edit
void setup() {
  Serial.begin(115200);
  ...
  Blynk.begin(...);
  ...
  timer.setInterval(...);
}
Initializes serial monitor, Wi-Fi, Blynk, sensor pins, and timer tasks:

DHT sensor every 2 sec

MQ-3 air quality every 3 sec

Obstacle check every 0.5 sec

6. loop() Function
cpp
Copy
Edit
void loop() {
  Blynk.run();
  timer.run();
}
Runs the Blynk service and periodic sensor tasks.

7. Sensor Handlers
DHT11 Handler:
cpp
Copy
Edit
void sendDHTData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Blynk.virtualWrite(V6, ...);
}
Sends temperature and humidity values to Virtual Pin V6 in Blynk app.

MQ-3 Handler:
cpp
Copy
Edit
void sendMQ3Data() {
  int sensorValue = analogRead(mq3Pin);
  ...
  Blynk.virtualWrite(V7, ...);
}
Reads gas concentration and labels it (Fresh, Moderate, Danger), sends to V7.

8. Relay Control (Vacuum or Mop Motor)
cpp
Copy
Edit
BLYNK_WRITE(V5) {
  int state = param.asInt();  // 1 = ON, 0 = OFF
  digitalWrite(RELAY_PIN, state ? LOW : HIGH);
}
Controls cleaning motor through relay (active LOW logic).

Controlled via Virtual Pin V5 from the Blynk app.

9. Movement Control via Blynk
cpp
Copy
Edit
BLYNK_WRITE(V1) { ... moveForward(); }
BLYNK_WRITE(V2) { ... moveBackward(); }
BLYNK_WRITE(V3) { ... turnLeft(); }
BLYNK_WRITE(V4) { ... turnRight(); }
Movement commands are linked to Virtual Pins V1–V4.

Each command activates corresponding motion functions.

Movement stops when the button is released or obstacle is detected.

10. Obstacle Detection
cpp
Copy
Edit
long getDistance(trig, echo) {
  ...
}
Returns the average distance from ultrasonic sensors.

cpp
Copy
Edit
void checkObstacles() {
  ...
  if (obstacleFront) { reverse } 
  else if (left) { turnRight } 
  else if (right) { turnLeft }
}
Checks distance values from left and right ultrasonic sensors.

If too close (<20 cm), executes:

Stop → Reverse (if front)

Turn (if side obstacles)

11. Movement Functions
cpp
Copy
Edit
void moveForward() { ... }
void moveBackward() { ... }
void turnLeft() { ... }
void turnRight() { ... }
void stopCar() { ... }
Sets motor directions via digital pins

Controls motor speed using analogWrite() on ENA and ENB

Ensures:

Smooth motion

Stops if obstacle is in front

✅ Summary of Features:
Feature	Blynk Virtual Pin	Controlled Via
Forward / Back / Turn	V1–V4	Manual buttons
Relay Motor (Vacuum/Mop)	V5	Blynk Switch
DHT Sensor (Temp & Humid)	V6	Auto Update
MQ3 Sensor (Air Quality)	V7	Auto Update
Obstacle Avoidance	Internal Logic	Ultrasonic sensors
