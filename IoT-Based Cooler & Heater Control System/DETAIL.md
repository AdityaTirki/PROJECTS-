# 🌡️ IoT-Based Temperature-Controlled Cooler and Heater System

This project automates the control of a **cooler** and a **heater** based on live temperature readings using a **DHT11 sensor** and an **ESP32**, and allows real-time monitoring through the **Blynk IoT platform**.

---

## 📌 Table of Contents

- [Overview](#overview)
- [Hardware Components](#hardware-components)
- [Working Principle](#working-principle)
- [Circuit Diagram](#circuit-diagram)
- [Blynk App Setup](#blynk-app-setup)
- [Virtual Pin Mapping](#virtual-pin-mapping)
- [Software Requirements](#software-requirements)
- [Installation & Setup](#installation--setup)
- [Code](#code)
- [License](#license)
- [Contributors](#contributors)

---

## 🧠 Overview

This smart system uses an ESP32 to read temperature and humidity from the DHT11 sensor. Depending on the temperature:
- It turns **ON the heater** if the temperature is **below 20°C**
- Turns **ON the cooler** if the temperature is **above 30°C**
- Keeps both devices **OFF** in the normal range (20–30°C)

The system updates **temperature**, **humidity**, and **relay status (ON/OFF)** to the **Blynk IoT mobile app** every 3 seconds.

---

## 🔧 Hardware Components

| Component                     | Quantity |
|------------------------------|----------|
| ESP32 DevKit V1              | 1        |
| DHT11 Temperature Sensor     | 1        |
| 2-Channel Relay Module       | 1        |
| Cooler/Fan (for demo)        | 1        |
| Heater/Bulb (for demo)       | 1        |
| Jumper Wires                 | As needed|
| Breadboard                   | 1        |
| 5V Power Supply              | 1        |

---

## ⚙️ Working Principle

- **Temp < 20°C** → Heater ON, Cooler OFF
- **Temp > 30°C** → Cooler ON, Heater OFF
- **Temp 20–30°C** → Both OFF

Relay states and environment data are displayed in the **Blynk app**.

---

## 🔌 Circuit Diagram

🧠 **Connections**:

| Component         | ESP32 GPIO |
|------------------|------------|
| DHT11 Data Pin   | GPIO 4     |
| Relay IN1 (Cooler) | GPIO 18  |
| Relay IN2 (Heater) | GPIO 19  |
| VCC (DHT & Relay) | 5V         |
| GND               | GND        |

_You can add a Fritzing or circuit image here if needed._

---

## 📱 Blynk App Setup

1. Create a Blynk project
2. Add these widgets:

| Widget Type     | Virtual Pin | Purpose               |
|------------------|-------------|------------------------|
| Gauge / Label    | V0          | Temperature (°C)       |
| Gauge / Label    | V1          | Humidity (%)           |
| Terminal / Label | V2          | Status Text (Heater/Cooler ON/OFF) |
| Value Display    | V3          | Heater Status ("ON"/"OFF") |
| Value Display    | V4          | Cooler Status ("ON"/"OFF") |

---

## 🧲 Virtual Pin Mapping

| Virtual Pin | Description                    |
|-------------|--------------------------------|
| `V0`        | Temperature from DHT11 (°C)     |
| `V1`        | Humidity from DHT11 (%)         |
| `V2`        | Combined system status message  |
| `V3`        | Heater status: "ON" / "OFF"     |
| `V4`        | Cooler status: "ON" / "OFF"     |

---

## 💻 Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Blynk Library](https://github.com/blynkkk/blynk-library)
- DHT Sensor Library by Adafruit
- Adafruit Unified Sensor Library
- Blynk App (iOS/Android)

---

## 🚀 Installation & Setup

1. Clone this repo or download the `.ino` file.
2. Open the `.ino` file in Arduino IDE.
3. Install required libraries:
   - `DHT sensor library` by Adafruit
   - `Adafruit Unified Sensor`
   - `Blynk` library for ESP32
4. Replace the following placeholders in the code:
   ```cpp
   #define WIFI_SSID "YourWiFiName"
   #define WIFI_PASS "YourWiFiPassword"
   #define BLYNK_AUTH_TOKEN "YourBlynkAuthToken"
