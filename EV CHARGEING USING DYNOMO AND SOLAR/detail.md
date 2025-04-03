#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD: 0x27, 20 columns, 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin Definitions
#define SENSOR1_PIN 34  // Solar Battery Monitor
#define SENSOR2_PIN 35  // Dynamo Battery Monitor
#define SENSOR3_PIN 32  // Dynamo Charging Line (Relay 1)
#define SENSOR4_PIN 33  // Solar Charging Line (Relay 2)

#define RELAY1_PIN 23   // Dynamo Relay
#define RELAY2_PIN 22   // Solar Relay
#define BUZZER_PIN 14   // D5 (Buzzer pin updated)

// Voltage Thresholds
const float maxSensorVoltage = 25.0;
const float correctionFactor = 0.6920;
const float lowVoltageWarning = 10.0;
const float thresholdRelay1 = 11.5;  // Dynamo full
const float thresholdRelay2 = 11.5;  // Solar full

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  Wire.begin(21, 18);  // SDA, SCL
  lcd.init();
  lcd.backlight();

  // Pin Modes
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(RELAY1_PIN, HIGH);  // Relays OFF initially
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  // Boot beep
  lcd.setCursor(0, 0);
  lcd.print("System Booting...");
  beepPattern(300, 300, 2);
  delay(2000);
  lcd.clear();
}

void loop() {
  float v1 = readVoltage(SENSOR1_PIN);  // Solar battery
  float v2 = readVoltage(SENSOR2_PIN);  // Dynamo battery
  float v3 = readVoltage(SENSOR3_PIN);  // Dynamo charge line
  float v4 = readVoltage(SENSOR4_PIN);  // Solar charge line

  // Line 0 – Battery Voltages
  lcd.setCursor(0, 0);
  lcd.print("S1:");
  lcd.print(v1, 1);
  lcd.print("V S2:");
  lcd.print(v2, 1);
  lcd.print("V");

  // Line 1 – Solar Battery Status (S1)
  lcd.setCursor(0, 1);
  if (v1 < lowVoltageWarning) {
    lcd.print("LOW VOLTAGE at SOLAR ");
    beepPattern(1000, 800, 1);
  } else {
    lcd.print("SOLAR BATTERY OK     ");
  }

  // Line 2 – Dynamo Charge Control (S3)
  lcd.setCursor(0, 2);
  if (v3 >= thresholdRelay1) {
    lcd.print("FULLY CHARGED at DYN ");
    digitalWrite(RELAY1_PIN, HIGH);  // Stop charging
    beepPattern(200, 200, 2);
  } else {
    lcd.print("CHARGING from DYNAMO ");
    digitalWrite(RELAY1_PIN, LOW);   // Allow charging
  }

  // Line 3 – Solar Charge Control (S4)
  lcd.setCursor(0, 3);
  if (v4 >= thresholdRelay2) {
    lcd.print("FULLY CHARGED at SOL ");
    digitalWrite(RELAY2_PIN, HIGH);  // Stop charging
    beepPattern(200, 200, 2);
  } else {
    lcd.print("CHARGING from SOLAR  ");
    digitalWrite(RELAY2_PIN, LOW);   // Allow charging
  }

  delay(2000);
}

// Reads and calibrates voltage
float readVoltage(int pin) {
  int adc = analogRead(pin);
  return (adc / 4095.0) * maxSensorVoltage * correctionFactor;
}

// Custom beep patterns
void beepPattern(int onTime, int offTime, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(onTime);
    digitalWrite(BUZZER_PIN, LOW);
    delay(offTime);
  }
}
