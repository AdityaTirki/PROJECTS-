// Include necessary libraries
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the I2C LCD: address 0x27, 20 columns and 4 rows
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Define sensor input pins
#define SENSOR1_PIN 34  // Solar battery monitor only
#define SENSOR2_PIN 35  // Dynamo battery monitor only
#define SENSOR3_PIN 32  // Dynamo battery voltage sensor (relay-controlled)
#define SENSOR4_PIN 33  // Solar battery voltage sensor (relay-controlled)

// Define output control pins
#define RELAY1_PIN 2     // Relay 1 for Dynamo charging control
#define RELAY2_PIN 4     // Relay 2 for Solar charging control
#define BUZZER_PIN 5     // Buzzer output pin

// Voltage reading calibration and thresholds
const float maxSensorVoltage = 25.0;     // Max voltage measurable by sensor
const float correctionFactor = 0.6920;   // Calibration factor (based on testing)
const float lowVoltageWarning = 10.0;    // Threshold for low battery warning
const float thresholdRelay1 = 11.5;      // Dynamo battery fully charged
const float thresholdRelay2 = 11.5;      // Solar battery fully charged

int screenState = 0;  // Toggle variable to switch between voltage and status screens

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C with specific SDA and SCL pins for ESP32
  Wire.begin(21, 18);
  lcd.init();
  lcd.backlight();

  // Set pin modes for relays and buzzer
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Ensure relays are off at startup
  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  // Boot message and beep
  lcd.setCursor(0, 0);
  lcd.print("System Booting...");
  beepPattern(300, 300, 2);
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read voltages from all sensors
  float v1 = readVoltage(SENSOR1_PIN);  // Solar battery (monitor only)
  float v2 = readVoltage(SENSOR2_PIN);  // Dynamo battery (monitor only)
  float v3 = readVoltage(SENSOR3_PIN);  // Dynamo battery (relay-controlled)
  float v4 = readVoltage(SENSOR4_PIN);  // Solar battery (relay-controlled)

  lcd.clear();

  if (screenState == 0) {
    // ---- Screen 1: Raw voltage readings ----
    lcd.setCursor(0, 0);
    lcd.print("Solar Batt:  ");
    lcd.print(v1, 1);
    lcd.print("V");

    lcd.setCursor(0, 1);
    lcd.print("Dynamo Batt: ");
    lcd.print(v2, 1);
    lcd.print("V");

    lcd.setCursor(0, 2);
    lcd.print("Dyn Batt RLY: ");
    lcd.print(v3, 1);
    lcd.print("V");

    lcd.setCursor(0, 3);
    lcd.print("Sol Batt RLY: ");
    lcd.print(v4, 1);
    lcd.print("V");
  }
  else {
    // ---- Screen 2: Battery status + relay logic ----

    // Line 0: Solar battery monitor only
    lcd.setCursor(0, 0);
    if (v1 < lowVoltageWarning) {
      lcd.print("LOW VOLTAGE at SOLAR");
      beepPattern(1000, 800, 1);
    } else {
      lcd.print("SOLAR BATTERY OK     ");
    }

    // Line 1: Dynamo battery monitor only
    lcd.setCursor(0, 1);
    if (v2 < lowVoltageWarning) {
      lcd.print("LOW VOLTAGE at DYNAMO");
      beepPattern(1000, 800, 1);
    } else {
      lcd.print("DYNAMO BATTERY OK    ");
    }

    // Line 2: Dynamo relay-controlled battery
    lcd.setCursor(0, 2);
    if (v3 >= thresholdRelay1) {
      lcd.print("FULLY CHARGED at DYN ");
      digitalWrite(RELAY1_PIN, HIGH);  // Cutoff charging
      beepPattern(200, 200, 2);
    } else {
      lcd.print("CHARGING from DYNAMO ");
      digitalWrite(RELAY1_PIN, LOW);   // Allow charging
    }

    // Line 3: Solar relay-controlled battery
    lcd.setCursor(0, 3);
    if (v4 >= thresholdRelay2) {
      lcd.print("FULLY CHARGED at SOL ");
      digitalWrite(RELAY2_PIN, HIGH);
      beepPattern(200, 200, 2);
    } else {
      lcd.print("CHARGING from SOLAR  ");
      digitalWrite(RELAY2_PIN, LOW);
    }
  }

  screenState = !screenState;  // Toggle between screen 0 and 1
  delay(3000);  // Wait 3 seconds before switching
}

// Function to read and calibrate analog voltage from sensor
float readVoltage(int pin) {
  int adc = analogRead(pin);  // Read analog value (0–4095)
  return (adc / 4095.0) * maxSensorVoltage * correctionFactor;
}

// Function to beep buzzer with pattern
void beepPattern(int onTime, int offTime, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(onTime);
    digitalWrite(BUZZER_PIN, LOW);
    delay(offTime);
  }
}
