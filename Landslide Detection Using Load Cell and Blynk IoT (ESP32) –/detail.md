### **Landslide Detection Using Load Cell and Blynk IoT (ESP32) – Code Explanation**

This program is designed for a **Landslide Detection System** using **ESP32, a load cell (HX711), soil moisture & rain sensors, OLED display, and Blynk IoT**. The system detects potential landslides based on **weight variations** measured by a **load cell**, while also monitoring **soil moisture and rain intensity**.

---

## **📌 Code Breakdown**
### **1. Libraries and Credentials**
```cpp
#define BLYNK_TEMPLATE_ID "TMPL32tHt9aBx"
#define BLYNK_TEMPLATE_NAME "Landslide Detection"
#define BLYNK_AUTH_TOKEN "IZWXg0SCjKjcMGSNO9BS0FdmGE8GQAtO"

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HX711.h>
```
- **Blynk IoT Authentication**: Used to send alerts & monitor data remotely.
- **`Wire.h`**: Required for **I2C communication** (used by the OLED display).
- **`Adafruit_SSD1306.h`**: Handles **OLED display** operations.
- **`ESP32Servo.h`**: Controls **servo motors** for gates.
- **`WiFi.h`**: Connects ESP32 to the **internet**.
- **`BlynkSimpleEsp32.h`**: Used to integrate ESP32 with **Blynk IoT**.
- **`HX711.h`**: Used to interface with the **Load Cell Sensor** (for weight measurement).

---

### **2. WiFi Configuration**
```cpp
char ssid[] = "Galaxy M111822";
char pass[] = "szpk4641";
```
- Stores **WiFi SSID and Password** to connect the ESP32 to the **internet**.

---

### **3. OLED Display Configuration**
```cpp
#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
```
- **OLED Address**: `0x3C` (standard for SSD1306 OLEDs).
- **Screen Resolution**: `128x64` pixels.
- **Display Object**: `display` is initialized for handling text and graphics.

---

### **4. Load Cell (Weight Sensor) Configuration**
```cpp
#define LOADCELL_DOUT 21
#define LOADCELL_SCK 22
HX711 scale;
```
- **HX711 Load Cell Module**:
  - **Data Output (DOUT)** connected to ESP32 **pin 21**.
  - **Clock Signal (SCK)** connected to ESP32 **pin 22**.
- `HX711 scale;` creates an instance of the **load cell sensor**.

---

### **5. Servo Motors (Gate Control)**
```cpp
#define SERVO_ENTRY_PIN 13
#define SERVO_EXIT_PIN 14
Servo entryServo;
Servo exitServo;
int openPosition = 90;
int closePosition = 0;
bool entryOpen = false;
bool exitOpen = false;
```
- **Servo motors** control **entry and exit gates**.
- **Pin 13 (Entry Gate)** and **Pin 14 (Exit Gate)** control **servo motors**.
- **Gates start in a closed position (0°)**.
- `entryOpen` and `exitOpen` track **gate states**.

---

### **6. Soil & Rain Sensor Configuration**
```cpp
#define SOIL_PIN 34
#define RAIN_PIN 35
```
- **Soil Moisture Sensor** connected to **Pin 34**.
- **Rain Sensor** connected to **Pin 35**.

---

### **7. LED Indicators for Landslide Alerts**
```cpp
#define GREEN_LED_ENTRY 25
#define RED_LED_ENTRY 26
#define GREEN_LED_EXIT 27
#define RED_LED_EXIT 32
```
- **Green LEDs** (Entry: Pin 25, Exit: Pin 27) - **Indicates Normal Condition**.
- **Red LEDs** (Entry: Pin 26, Exit: Pin 32) - **Indicates Landslide Alert**.

---

## **📌 OLED Display Function**
```cpp
void displayData(float weight, int soilMoisture, int rainLevel) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("LANDSLIDE MONITOR");

    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("Wt: ");
    display.print(weight);
    display.print(" kg");

    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Soil:");
    display.print(soilMoisture);
    display.print("%");

    int barWidth = map(soilMoisture, 0, 100, 0, 80);
    display.fillRect(40, 45, barWidth, 8, WHITE);
    display.drawRect(40, 45, 80, 8, WHITE);

    display.setCursor(0, 55);
    display.print("Rain:");
    display.print(rainLevel > 700 ? "Dry" : (rainLevel > 400 ? "Light" : "Heavy"));

    display.display();
}
```
- **Displays weight, soil moisture, and rain levels**.
- **Graphically represents soil moisture** using a bar.

---

## **📌 Setup Function**
```cpp
void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
```
- **Initializes Serial Monitor** (baud rate `115200`).
- **Connects ESP32 to Blynk IoT**.

```cpp
    pinMode(GREEN_LED_ENTRY, OUTPUT);
    pinMode(RED_LED_ENTRY, OUTPUT);
    pinMode(GREEN_LED_EXIT, OUTPUT);
    pinMode(RED_LED_EXIT, OUTPUT);
```
- **Configures LEDs as OUTPUT**.

```cpp
    entryServo.attach(SERVO_ENTRY_PIN);
    exitServo.attach(SERVO_EXIT_PIN);
    entryServo.write(closePosition);
    exitServo.write(closePosition);
```
- **Configures Servo Motors** and sets them to the **closed position**.

```cpp
    scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
    scale.set_scale(2280.0);  // Adjust based on calibration
    scale.tare();  // Reset the scale to zero
```
- **Initializes Load Cell Sensor** and sets calibration scale (`2280.0`).

---

## **📌 Loop Function (Main Execution)**
```cpp
void loop() {
    Blynk.run();
```
- **Runs Blynk to maintain IoT connectivity**.

```cpp
    float weight = scale.get_units(5); // Get weight in kg
    if (weight < 0) weight = 0;
```
- **Reads weight from Load Cell (5 samples for accuracy)**.

```cpp
    int soilMoisture = map(analogRead(SOIL_PIN), 4095, 1500, 0, 100);
    int rainLevel = analogRead(RAIN_PIN);
```
- **Reads Soil Moisture and Rain Levels**.

```cpp
    if (weight > 50.0) { 
        digitalWrite(RED_LED_ENTRY, HIGH);
        digitalWrite(RED_LED_EXIT, HIGH);
        digitalWrite(GREEN_LED_ENTRY, LOW);
        digitalWrite(GREEN_LED_EXIT, LOW);
        Serial.println("Warning: Landslide Detected!");
        Blynk.logEvent("landslide_alert", "Landslide Detected! Take Precautions.");
    } else {
        digitalWrite(GREEN_LED_ENTRY, HIGH);
        digitalWrite(GREEN_LED_EXIT, HIGH);
        digitalWrite(RED_LED_ENTRY, LOW);
        digitalWrite(RED_LED_EXIT, LOW);
    }
```
- **Triggers an alert if weight exceeds `50kg`** (indicating a landslide).
- **Red LEDs turn ON**, and **Blynk sends an alert**.

```cpp
    displayData(weight, soilMoisture, rainLevel);
    delay(1000);
}
```
- **Updates OLED display every second**.

---

## **🔹 Key Features**
✔ **Detects Landslides using Load Cell**  
✔ **Monitors Soil Moisture & Rain Conditions**  
✔ **Sends Real-time Alerts via Blynk IoT**  
✔ **Controls Entry & Exit Gates via Servo Motors**  
✔ **Displays Live Data on OLED**  

---

## **🚀 Future Enhancements**
- **Save Data to Cloud (Google Sheets, Firebase)**
- **SMS Alerts (Twilio, Blynk Notifications)**
- **Battery Backup for Field Use**

2.... Here's the updated code with the **ultrasonic sensors removed** and a **load cell sensor (HX711) added** to measure weight instead. The system now uses a **load cell to detect weight variations** (potential landslide indicators) instead of distance sensors.

---

### **Changes Made:**
1. **Removed Ultrasonic Sensors (`TRIG_ENTRY`, `ECHO_ENTRY`, `TRIG_EXIT`, `ECHO_EXIT`)**
2. **Added Load Cell (`HX711`) for weight measurement**
3. **Load Cell connected via `DOUT` (pin 21) and `SCK` (pin 22)**

---

### **Updated Code:**
```cpp
#define BLYNK_TEMPLATE_ID "TMPL32tHt9aBx"
#define BLYNK_TEMPLATE_NAME "Landslide Detection"
#define BLYNK_AUTH_TOKEN "IZWXg0SCjKjcMGSNO9BS0FdmGE8GQAtO"

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <HX711.h>

// WiFi Credentials
char ssid[] = "Galaxy M111822";
char pass[] = "szpk4641";

// OLED Display Configuration
#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Load Cell Pins (HX711)
#define LOADCELL_DOUT 21
#define LOADCELL_SCK 22
HX711 scale;

// Servo Motor Pins
#define SERVO_ENTRY_PIN 13
#define SERVO_EXIT_PIN 14

// Soil & Rain Sensor Pins
#define SOIL_PIN 34
#define RAIN_PIN 35

// LED Pins
#define GREEN_LED_ENTRY 25
#define RED_LED_ENTRY 26
#define GREEN_LED_EXIT 27
#define RED_LED_EXIT 32

// Servo Motors
Servo entryServo;
Servo exitServo;
int openPosition = 90;
int closePosition = 0;
bool entryOpen = false;
bool exitOpen = false;

// Function to Display Data on OLED
void displayData(float weight, int soilMoisture, int rainLevel) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(20, 0);
    display.print("LANDSLIDE MONITOR");

    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("Wt: ");
    display.print(weight);
    display.print(" kg");

    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Soil:");
    display.print(soilMoisture);
    display.print("%");

    int barWidth = map(soilMoisture, 0, 100, 0, 80);
    display.fillRect(40, 45, barWidth, 8, WHITE);
    display.drawRect(40, 45, 80, 8, WHITE);

    display.setCursor(0, 55);
    display.print("Rain:");
    display.print(rainLevel > 700 ? "Dry" : (rainLevel > 400 ? "Light" : "Heavy"));

    display.display();
}

void setup() {
    Serial.begin(115200);
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    pinMode(GREEN_LED_ENTRY, OUTPUT);
    pinMode(RED_LED_ENTRY, OUTPUT);
    pinMode(GREEN_LED_EXIT, OUTPUT);
    pinMode(RED_LED_EXIT, OUTPUT);

    digitalWrite(GREEN_LED_ENTRY, LOW);
    digitalWrite(RED_LED_ENTRY, LOW);
    digitalWrite(GREEN_LED_EXIT, LOW);
    digitalWrite(RED_LED_EXIT, LOW);

    entryServo.attach(SERVO_ENTRY_PIN);
    exitServo.attach(SERVO_EXIT_PIN);
    entryServo.write(closePosition);
    exitServo.write(closePosition);

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("OLED not found!");
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.println("System Init...");
    display.display();
    delay(2000);
    
    // Initialize Load Cell
    scale.begin(LOADCELL_DOUT, LOADCELL_SCK);
    scale.set_scale(2280.0);  // Adjust based on calibration
    scale.tare();  // Reset the scale to zero
    
    Serial.println("Landslide Detection System Ready");
}

void loop() {
    Blynk.run();

    // Read Load Cell Data
    float weight = scale.get_units(5); // Get weight in kg
    if (weight < 0) weight = 0;

    // Read Soil Moisture and Rain Sensor
    int soilMoisture = map(analogRead(SOIL_PIN), 4095, 1500, 0, 100);
    int rainLevel = analogRead(RAIN_PIN);

    // Landslide condition check
    if (weight > 50.0) { // If detected weight exceeds 50kg (adjustable threshold)
        digitalWrite(RED_LED_ENTRY, HIGH);
        digitalWrite(RED_LED_EXIT, HIGH);
        digitalWrite(GREEN_LED_ENTRY, LOW);
        digitalWrite(GREEN_LED_EXIT, LOW);
        Serial.println("Warning: Landslide Detected!");
        Blynk.logEvent("landslide_alert", "Landslide Detected! Take Precautions.");
    } else {
        digitalWrite(GREEN_LED_ENTRY, HIGH);
        digitalWrite(GREEN_LED_EXIT, HIGH);
        digitalWrite(RED_LED_ENTRY, LOW);
        digitalWrite(RED_LED_EXIT, LOW);
    }

    displayData(weight, soilMoisture, rainLevel);
    delay(1000);
}
```

---

## **Changes & Features in Updated Code**
### **1. Load Cell Integration (HX711)**
- **Replaces ultrasonic sensors** for landslide detection.
- Measures **weight variations** (e.g., soil displacement due to landslides).
- Uses **`HX711` sensor module** connected to **ESP32 pins 21 and 22**.
- `scale.get_units(5)` reads weight in **kilograms**.
- If weight **exceeds 50kg**, it **triggers an alert** and **turns on red LEDs**.

### **2. Blynk IoT Alerts**
- Sends a **"Landslide Detected" alert** if the weight crosses a defined threshold (`50kg`).

### **3. OLED Display Updates**
- Shows **measured weight** instead of entry/exit distances.
- Displays **soil moisture** and **rain status**.

---

## **Hardware Connections**
| **Component**       | **ESP32 Pin** |
|---------------------|--------------|
| Load Cell DOUT     | 21           |
| Load Cell SCK      | 22           |
| Soil Moisture      | 34           |
| Rain Sensor        | 35           |
| Servo Motor Entry  | 13           |
| Servo Motor Exit   | 14           |
| Green LED Entry    | 25           |
| Red LED Entry      | 26           |
| Green LED Exit     | 27           |
| Red LED Exit       | 32           |
| OLED SDA           | GPIO 21 (I2C) |
| OLED SCL           | GPIO 22 (I2C) |

---

### **How it Works**
1. **ESP32 reads weight from the load cell.**
2. If the **weight exceeds 50kg**, the system:
   - **Triggers an alert** via Blynk.
   - **Turns on red LEDs** to indicate a landslide.
3. **Soil moisture and rain levels** are displayed on the **OLED screen**.
4. **Blynk IoT integration** allows real-time monitoring.

---

### **Next Steps**
- **Calibrate Load Cell**: Adjust `scale.set_scale(2280.0)` for better accuracy.
- **Set Threshold Dynamically**: Instead of a fixed `50kg` threshold, allow **remote threshold updates** using **Blynk sliders**.
- **Data Logging**: Send weight, soil, and rain data to a **Google Sheet** or **cloud database** for further analysis.
- # **Landslide Detection System Using Load Cell, ESP32, and Blynk IoT**

### **📌 Project Overview**
This project **detects landslides** using a **load cell sensor (HX711)** to measure weight changes in the soil. The system is built using **ESP32**, and it also monitors **soil moisture and rain levels**. If a landslide is detected based on abnormal weight variations, the system:
- **Triggers an alert** using Blynk IoT.
- **Turns on LED indicators** (Red for danger, Green for normal conditions).
- **Displays live data on an OLED screen**.
- **Controls gates using servo motors** (if necessary).

This **real-time monitoring system** is useful for **landslide-prone areas**, helping authorities and individuals **take preventive actions before a disaster occurs**.

---

## **🎯 Objectives**
- **Monitor soil movement using a load cell**.
- **Detect landslide-prone conditions** based on weight fluctuations.
- **Monitor environmental factors** like soil moisture and rainfall.
- **Send real-time alerts to a cloud platform (Blynk IoT)**.
- **Control entry and exit gates using servo motors**.
- **Display real-time data on an OLED screen**.

---

## **🛠️ Hardware Components**
| **Component** | **Function** |
|--------------|-------------|
| **ESP32** | Microcontroller for data processing & IoT connectivity |
| **HX711 Load Cell** | Measures weight (used to detect soil displacement) |
| **Soil Moisture Sensor** | Measures soil wetness (helps predict landslides) |
| **Rain Sensor** | Detects rainfall intensity |
| **OLED Display (128x64)** | Displays sensor data and alerts |
| **Servo Motors (2x)** | Controls entry and exit gates |
| **LEDs (Red & Green)** | Indicates normal or landslide conditions |
| **Blynk IoT** | Sends alerts and allows remote monitoring |

---

## **🔌 System Working & Functionality**
### **1️⃣ Weight-Based Landslide Detection (Load Cell & HX711)**
- The **load cell sensor** continuously measures weight.
- If there’s a **sudden increase in weight (e.g., 50kg or more)**, it indicates soil movement (possible landslide).
- The system **triggers an alert and activates warning LEDs**.

### **2️⃣ Soil Moisture & Rainfall Monitoring**
- The **soil moisture sensor** measures how wet the soil is.
- The **rain sensor** detects the intensity of rainfall.
- Both factors contribute to **landslide risk assessment**.
- If the soil is too wet + rain is heavy + weight increases → **higher landslide risk**.

### **3️⃣ Alerts & Notifications (Blynk IoT)**
- If a landslide is detected, **Blynk sends an alert to a mobile device**.
- Users can **view live data remotely**.

### **4️⃣ Servo Motors for Gate Control**
- If a landslide is detected, **entry and exit gates** can be controlled.
- This prevents vehicles or people from **entering a dangerous area**.

### **5️⃣ OLED Display for Real-time Monitoring**
- **Shows live sensor values** (Weight, Soil Moisture, Rain Level).
- Provides a **graphical bar for soil moisture**.

---

## **📡 IoT Integration with Blynk**
- **Blynk is an IoT platform** that allows remote monitoring & control.
- The system:
  - **Sends landslide alerts** in real time.
  - **Logs sensor data** for analysis.
  - **Provides a dashboard** for users to monitor conditions.

---

## **⚡ Workflow**
1. **ESP32 collects data** from:
   - Load Cell (weight measurement).
   - Soil Moisture Sensor.
   - Rain Sensor.
2. **ESP32 processes the data** and **checks conditions**:
   - If weight is **>50kg**, **LEDs turn RED** (Landslide alert).
   - If weight is normal, **LEDs stay GREEN**.
3. **Blynk IoT sends alerts** if a landslide is detected.
4. **OLED display updates** with real-time values.
5. **Servo motors control gates** based on the situation.

---

## **🚦 System States**
| **Condition** | **Weight (Load Cell)** | **Soil Moisture** | **Rain Intensity** | **LED Status** | **Blynk Alert** |
|--------------|----------------|----------------|----------------|-----------|-------------|
| **Normal Condition** | < 50kg | Low or Moderate | Light Rain / Dry | Green ON | No Alert |
| **Possible Landslide** | 50kg - 100kg | High | Heavy Rain | Red ON | Landslide Warning Sent |
| **Severe Landslide** | > 100kg | Very High | Heavy Rain | Red ON | Emergency Alert Sent |

---

## **🚀 Applications**
1. **Landslide Prone Areas** – Monitors weight shifts and warns communities.
2. **Construction & Mining Sites** – Prevents structural failures.
3. **Highway & Road Safety** – Alerts authorities about unstable slopes.
4. **Agriculture** – Detects soil erosion in farms.

---

## **📌 Future Enhancements**
✅ **AI-based Prediction Model** – Analyze historical data to predict landslides.  
✅ **Google Sheets / Firebase Data Logging** – Store data for long-term analysis.  
✅ **SMS Alerts (via Twilio)** – Notify emergency responders.  
✅ **Remote Threshold Control** – Allow users to set weight limits dynamically.  
✅ **Battery Backup for Remote Areas** – Ensure continuous monitoring.  

---

## **📝 Conclusion**
This **Landslide Detection System** is a **real-time, IoT-based monitoring solution** that integrates **weight-based detection, environmental monitoring, and remote alerts**. It can help **save lives and prevent disasters** by providing **early warnings and automated safety mechanisms**. 🚀

Would you like me to add **data logging features** or **machine learning-based landslide prediction**? Let me know! 😊

