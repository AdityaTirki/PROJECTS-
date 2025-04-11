// -------- BLYNK CONFIG MUST BE ON TOP --------
#define BLYNK_TEMPLATE_ID "TMPL3DiknMaIT"
#define BLYNK_TEMPLATE_NAME "dynomoEV"
#define BLYNK_AUTH_TOKEN "9vALNkyZ5Gdj_i3Aa1DDDPWO3nhZQu3M"

// -------- Library Includes --------
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// -------- WiFi Credentials --------
char ssid[] = "Major Project";
char pass[] = "Project@123";

// -------- LCD Setup --------
LiquidCrystal_I2C lcd(0x27, 20, 4);  // I2C address, 20 cols, 4 rows

// -------- Pin Definitions --------
#define SENSOR2_PIN 35  // Dynamo monitor
#define SENSOR3_PIN 32  // Dynamo relay
#define SENSOR4_PIN 33  // Solar relay

#define RELAY1_PIN 4
#define RELAY2_PIN 2
#define BUZZER_PIN 5

#define TEMP_SENSOR_DYN_PIN 16
#define TEMP_SENSOR_SOL_PIN 17

// -------- Sensor Objects --------
OneWire oneWireDyn(TEMP_SENSOR_DYN_PIN);
OneWire oneWireSol(TEMP_SENSOR_SOL_PIN);
DallasTemperature dynTempSensor(&oneWireDyn);
DallasTemperature solTempSensor(&oneWireSol);

// -------- Calibration & Thresholds --------
const float maxSensorVoltage = 25.0;
const float correctionFactor = 0.6920;
const float lowVoltageWarning = 7.0;
const float thresholdRelay1 = 13.0;
const float thresholdRelay2 = 13.0;

// -------- Global Variables --------
float dynTemp = 0.0;
float solTemp = 0.0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 18);
  lcd.init();
  lcd.backlight();

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN, HIGH);
  digitalWrite(RELAY2_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  dynTempSensor.begin();
  solTempSensor.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // -------- Startup Project Title --------
  lcd.setCursor(0, 0);
  lcd.print(" IoT Based Self ");
  lcd.setCursor(0, 1);
  lcd.print(" Charging EV     ");
  lcd.setCursor(0, 2);
  lcd.print(" Using Dynamo &  ");
  lcd.setCursor(0, 3);
  lcd.print(" Solar Power     ");
  beepPattern(300, 300, 2);
  delay(3000);
  lcd.clear();
}

void loop() {
  Blynk.run();

  // -------- Read Voltages --------
  float v2 = readVoltage(SENSOR2_PIN); // Dynamo monitor
  float v3 = readVoltage(SENSOR3_PIN); // Dynamo relay
  float v4 = readVoltage(SENSOR4_PIN); // Solar relay

  // -------- Read Temperatures --------
  dynTempSensor.requestTemperatures();
  solTempSensor.requestTemperatures();
  dynTemp = dynTempSensor.getTempCByIndex(0);
  solTemp = solTempSensor.getTempCByIndex(0);

  // -------- Send to Blynk --------
  Blynk.virtualWrite(V1, v2);
  Blynk.virtualWrite(V2, v3);
  Blynk.virtualWrite(V3, v4);
  Blynk.virtualWrite(V4, dynTemp);
  Blynk.virtualWrite(V5, solTemp);

  // -------- Relay Control --------
  if (v3 >= thresholdRelay1) digitalWrite(RELAY1_PIN, HIGH);
  else digitalWrite(RELAY1_PIN, LOW);

  if (v4 >= thresholdRelay2) digitalWrite(RELAY2_PIN, HIGH);
  else digitalWrite(RELAY2_PIN, LOW);

  // -------- Low Voltage Alert --------
  bool lowVolt = false;
  if (v2 < lowVoltageWarning) {
    Blynk.logEvent("low_voltage_alert", "Dynamo V LOW");
    beepPattern(500, 400, 1);
    lowVolt = true;
  }

  // -------- LCD OUTPUT (Single Screen) --------
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("DynV:");
  lcd.print(v2, 1);
  lcd.print("V ");
  lcd.print("T:");
  lcd.print(dynTemp, 0);
  lcd.print((char)223);  // Degree symbol

  lcd.setCursor(0, 1);
  lcd.print("SolT:");
  lcd.print(solTemp, 0);
  lcd.print((char)223);
  lcd.print(" DR:");
  lcd.print(v3, 1);
  lcd.print("V");

  lcd.setCursor(0, 2);
  lcd.print("SR:");
  lcd.print(v4, 1);
  lcd.print("V ");

  if (v3 >= thresholdRelay1) lcd.print("D:Full");
  else lcd.print("D:Chg ");

  if (v4 >= thresholdRelay2) lcd.print(" S:Full");
  else lcd.print(" S:Chg ");

  lcd.setCursor(0, 3);
  if (lowVolt) {
    lcd.print("ALERT: Low Dyn Volt ");
  } else {
    lcd.print("System OK           ");
  }

  delay(3000); // Update every 3 seconds
}

// -------- Read Voltage Sensor --------
float readVoltage(int pin) {
  int adc = analogRead(pin);
  return (adc / 4095.0) * maxSensorVoltage * correctionFactor;
}

// -------- Buzzer Pattern --------
void beepPattern(int onTime, int offTime, int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(onTime);
    digitalWrite(BUZZER_PIN, LOW);
    delay(offTime);
  }
}
