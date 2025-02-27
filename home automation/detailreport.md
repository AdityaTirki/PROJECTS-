Since your **Home Automation System** involves:  
- **Two Servo Motors** (for Door & Parking Gate)  
- **One DC Motor** (for Lift) controlled via **L298N Motor Driver**  
- **LED Lighting System**  

Here’s the complete **ESP8266 NodeMCU & Blynk App-based solution** for remote control.

---

## **System Overview**
1. **Car Parking Gate** → **Servo Motor** (controlled via Blynk App)  
2. **Main Door** → **Servo Motor** (controlled via Blynk App)  
3. **Lift Control** → **DC Motor via L298N Driver** (Up/Down buttons on Blynk)  
4. **LED Lighting** → **Controlled via Blynk App**  

---

## **Components Required**
1. **ESP8266 NodeMCU** – Wi-Fi-enabled microcontroller  
2. **2x Servo Motors (SG90 / MG995)** – For **Door & Parking Gate**  
3. **DC Motor (12V)** – For **Lift Movement**  
4. **L298N Motor Driver** – To **control the lift motor**  
5. **LED Strip / LED Bulb** – For **lighting system**  
6. **Blynk App (Android/iOS)** – To **control everything remotely**  
7. **Power Supply (5V/12V, 2A)** – For **ESP8266, motors, and LEDs**  
8. **Jumper Wires & Breadboard**  

---

## **Circuit Connections**
### **1. Servo Motor Connections (Door & Parking Gate)**
| Component         | ESP8266 NodeMCU Pin |
|------------------|--------------------|
| **Door Servo**   | D1 (GPIO5) |
| **Parking Gate Servo** | D2 (GPIO4) |

### **2. DC Motor (Lift) with L298N Driver**
| L298N Motor Driver | ESP8266 NodeMCU Pin |
|--------------------|--------------------|
| **IN1 (Motor Direction 1)** | D3 (GPIO0) |
| **IN2 (Motor Direction 2)** | D4 (GPIO2) |
| **ENA (Motor Speed - PWM)** | D5 (GPIO14) |

- **L298N Driver Motor Outputs (OUT1 & OUT2) → DC Motor Terminals**
- **L298N Power Inputs (VCC & GND) → 12V Power Supply**

### **3. LED Lighting Control**
| Component   | ESP8266 Pin |
|------------|------------|
| **LED Lights** | D6 (GPIO12) |

---

## **ESP8266 Code for Blynk App**
### **1. Install Required Libraries**
- **Blynk Library**
- **ESP8266WiFi Library**
- **Servo Library**

### **2. Upload This Code to ESP8266**
```cpp
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

// Wi-Fi & Blynk Credentials
char auth[] = "Your_Blynk_Auth_Token";  // From Blynk App
char ssid[] = "Your_WiFi_Name";         // Your WiFi SSID
char pass[] = "Your_WiFi_Password";     // Your WiFi Password

// Servo Motors
Servo doorServo;
Servo parkingServo;

// Define Pins
#define DOOR_SERVO_PIN D1
#define PARKING_SERVO_PIN D2
#define LIFT_MOTOR_IN1 D3
#define LIFT_MOTOR_IN2 D4
#define LIFT_MOTOR_ENA D5
#define LED_LIGHT_PIN D6

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  // Attach Servo Motors
  doorServo.attach(DOOR_SERVO_PIN);
  parkingServo.attach(PARKING_SERVO_PIN);

  // Set Initial Servo Positions
  doorServo.write(0);   // Door closed
  parkingServo.write(0); // Parking gate closed

  // Set Motor Driver Pins as Output
  pinMode(LIFT_MOTOR_IN1, OUTPUT);
  pinMode(LIFT_MOTOR_IN2, OUTPUT);
  pinMode(LIFT_MOTOR_ENA, OUTPUT);
  pinMode(LED_LIGHT_PIN, OUTPUT);

  // Stop Lift Motor Initially
  digitalWrite(LIFT_MOTOR_IN1, LOW);
  digitalWrite(LIFT_MOTOR_IN2, LOW);
  analogWrite(LIFT_MOTOR_ENA, 0);

  // Turn Off LED Initially
  digitalWrite(LED_LIGHT_PIN, LOW);
}

// **Blynk Button for Door Control**
BLYNK_WRITE(V1) {
  int state = param.asInt(); 
  if (state == 1) {
    doorServo.write(90);  // Open Door
  } else {
    doorServo.write(0);   // Close Door
  }
}

// **Blynk Button for Parking Gate Control**
BLYNK_WRITE(V2) {
  int state = param.asInt();
  if (state == 1) {
    parkingServo.write(90);  // Open Parking Gate
  } else {
    parkingServo.write(0);   // Close Parking Gate
  }
}

// **Blynk Buttons for Lift Control**
BLYNK_WRITE(V3) {  // Lift Up
  int state = param.asInt();
  if (state == 1) {
    digitalWrite(LIFT_MOTOR_IN1, HIGH);
    digitalWrite(LIFT_MOTOR_IN2, LOW);
    analogWrite(LIFT_MOTOR_ENA, 200); // Adjust Speed (0-255)
  } else {
    digitalWrite(LIFT_MOTOR_IN1, LOW);
    digitalWrite(LIFT_MOTOR_IN2, LOW);
    analogWrite(LIFT_MOTOR_ENA, 0);   // Stop Lift
  }
}

BLYNK_WRITE(V4) {  // Lift Down
  int state = param.asInt();
  if (state == 1) {
    digitalWrite(LIFT_MOTOR_IN1, LOW);
    digitalWrite(LIFT_MOTOR_IN2, HIGH);
    analogWrite(LIFT_MOTOR_ENA, 200);
  } else {
    digitalWrite(LIFT_MOTOR_IN1, LOW);
    digitalWrite(LIFT_MOTOR_IN2, LOW);
    analogWrite(LIFT_MOTOR_ENA, 0);
  }
}

// **Blynk Button for LED Lighting Control**
BLYNK_WRITE(V5) {
  int state = param.asInt();
  digitalWrite(LED_LIGHT_PIN, state);
}

void loop() {
  Blynk.run();
}
```

---

## **Blynk App Setup**
1. **Install the Blynk App** from Play Store/App Store.
2. **Create a New Project** and select:
   - **Device:** ESP8266
   - **Connection Type:** Wi-Fi
3. **Add Widgets**:
   - **Button (V1) → "Door OPEN/CLOSE" (Switch)**
   - **Button (V2) → "Parking Gate OPEN/CLOSE" (Switch)**
   - **Button (V3) → "Lift UP" (Push Button)**
   - **Button (V4) → "Lift DOWN" (Push Button)**
   - **Button (V5) → "Lights ON/OFF" (Switch)**
4. **Link the Buttons** to Virtual Pins **V1, V2, V3, V4, V5**.

---

## **How It Works**
- Open **Blynk App** and control:
  - **Door** with a button (Servo motor moves 0° to 90°).
  - **Parking Gate** with a button (Servo motor moves 0° to 90°).
  - **Lift** with UP and DOWN buttons (DC Motor runs via L298N).
  - **LED Lights** with a switch button (Turns ON/OFF).

---

## **Future Enhancements**
- Add **Timers** in Blynk for **automatic operations**.
- Use **Google Assistant / Alexa** for **Voice Control**.
- Add **MQTT** for real-time multi-device communication.

Would you like any **extra features**? 🚀
