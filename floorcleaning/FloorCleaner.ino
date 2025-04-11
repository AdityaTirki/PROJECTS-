// Blynk Floor Cleaner Robot using ESP32

#define BLYNK_TEMPLATE_ID "TMPL3ai8Jdgdu"
#define BLYNK_TEMPLATE_NAME "floorcleaner"
#define BLYNK_AUTH_TOKEN "2e741ROH_toZHtfLueY626PrhJIIT-rU"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Motor Driver Pins for movement
#define IN1 5
#define IN2 18
#define IN3 21
#define IN4 19
#define ENA 4
#define ENB 15

// DHT11 Sensor for temperature & humidity
#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ-3 Gas sensor for air quality monitoring
const int mq3Pin = 34;

// Relay to control cleaning motor (vacuum/mop)
#define RELAY_PIN 23  // Active LOW logic

// Wi-Fi credentials
char ssid[] = "Net";
char pass[] = "12341234";

// Motor speed value
const int speedValue = 200;
BlynkTimer timer;

// Ultrasonic sensor pins
#define TRIG_LEFT 26
#define ECHO_LEFT 25
#define TRIG_RIGHT 33
#define ECHO_RIGHT 32

// Flags for obstacle detection and state
bool obstacleFront = false;
bool obstacleLeft = false;
bool obstacleRight = false;
bool movingForward = false;

void setup() {
  Serial.begin(115200);

  // Set initial state for relay and configure pin
  digitalWrite(RELAY_PIN, LOW);
  pinMode(RELAY_PIN, OUTPUT);
  delay(200);

  // Start Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Motor pin modes
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Sensor setup
  dht.begin();
  pinMode(TRIG_LEFT, OUTPUT); pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  // Timer tasks
  timer.setInterval(2000L, sendDHTData);
  timer.setInterval(3000L, sendMQ3Data);
  timer.setInterval(500L, checkObstacles);
}

void loop() {
  Blynk.run();
  timer.run();
}

// DHT11 sensor reading and Blynk update
void sendDHTData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) return;
  Blynk.virtualWrite(V6, "T: " + String(t, 1) + "°C | H: " + String(h, 1) + "%");
}

// MQ-3 sensor reading and air quality analysis
void sendMQ3Data() {
  int sensorValue = analogRead(mq3Pin);
  String level = (sensorValue < 2000) ? "Fresh" : (sensorValue < 2500 ? "Moderate" : "Danger");
  Blynk.virtualWrite(V7, "CO: " + String(sensorValue) + " | " + level);
}

// Relay control for cleaning motor
BLYNK_WRITE(V5) {
  int state = param.asInt();
  digitalWrite(RELAY_PIN, state ? LOW : HIGH);
  Serial.println(state ? "🔛 Relay ON" : "🔴 Relay OFF");
}

// Blynk button controls for movement
BLYNK_WRITE(V1) { if (param.asInt()) { movingForward = true; moveForward(); } else stopCar(); }
BLYNK_WRITE(V2) { if (param.asInt()) moveBackward(); else stopCar(); }
BLYNK_WRITE(V3) { if (param.asInt()) turnLeft(); else stopCar(); }
BLYNK_WRITE(V4) { if (param.asInt()) turnRight(); else stopCar(); }

// Ultrasonic distance measurement
long getDistance(int trig, int echo) {
  long total = 0;
  for (int i = 0; i < 3; i++) {
    digitalWrite(trig, LOW); delayMicroseconds(2);
    digitalWrite(trig, HIGH); delayMicroseconds(10);
    digitalWrite(trig, LOW);
    long duration = pulseIn(echo, HIGH, 30000);
    long dist = duration * 0.034 / 2;
    if (dist <= 0 || dist > 300) dist = 300;
    total += dist;
    delay(10);
  }
  return total / 3;
}

// Obstacle detection logic
void checkObstacles() {
  long distLeft = getDistance(TRIG_LEFT, ECHO_LEFT);
  long distRight = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  obstacleFront = (distLeft < 20 && distRight < 20);
  obstacleLeft = (distLeft < 20 && distRight >= 20);
  obstacleRight = (distRight < 20 && distLeft >= 20);

  Serial.print("Left: "); Serial.print(distLeft);
  Serial.print(" cm | Right: "); Serial.print(distRight); Serial.println(" cm");

  if (obstacleFront) {
    Serial.println("🛑 Obstacle in FRONT — Reversing");
    stopCar();
    delay(200);
    moveBackward();
    delay(700);
    stopCar();
  } else if (obstacleLeft && movingForward) {
    Serial.println("↩️ Obstacle LEFT — Turning Right");
    turnRight();
    delay(600);
    stopCar();
  } else if (obstacleRight && movingForward) {
    Serial.println("↪️ Obstacle RIGHT — Turning Left");
    turnLeft();
    delay(600);
    stopCar();
  }

  Serial.println("--------------------------------");
}

// Movement functions
void moveForward() {
  if (obstacleFront) {
    stopCar();
    return;
  }
  Serial.println("➡️ Moving Forward");
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue); analogWrite(ENB, speedValue);
}

void moveBackward() {
  Serial.println("⬅️ Moving Backward");
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue); analogWrite(ENB, speedValue);
}

void turnLeft() {
  Serial.println("↪️ Turning Left");
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speedValue / 2); analogWrite(ENB, speedValue);
}

void turnRight() {
  Serial.println("↩️ Turning Right");
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedValue); analogWrite(ENB, speedValue / 2);
}

void stopCar() {
  movingForward = false;
  Serial.println("⏹️ Stopping Car");
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}
