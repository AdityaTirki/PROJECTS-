Here is the **full ESP8266 (VMOS) code** for **Auto Temperature Detection** using **DHT11**, **IR Sensor**, and **Blynk**.

---

## **🌟 Features:**
✔ Detects **human presence** using an **IR sensor**  
✔ Reads **body temperature** using a **DHT11 sensor**  
✔ **Blynk App** displays **live temperature** (°C)  
✔ If **temperature > 37.5°C**, **buzzer alerts & Blynk LED turns ON**  
✔ Works with **ESP8266 (NodeMCU/VMOS)**  

---

## **🛠 Components & Connections**
| **Component**  | **ESP8266 (NodeMCU/VMOS) Pin** |
|--------------|----------------------|
| **DHT11 VCC**  | **3.3V** |
| **DHT11 GND**  | **GND** |
| **DHT11 Data** | **D4 (GPIO2)** |
| **IR Sensor VCC** | **3.3V** |
| **IR Sensor GND** | **GND** |
| **IR Sensor OUT** | **D5 (GPIO14)** |
| **Buzzer VCC**  | **D6 (GPIO12)** |
| **Buzzer GND**  | **GND** |

---

## **📱 Blynk Setup**
1️⃣ **Install Blynk App (Android/iOS)**  
2️⃣ **Create a new project** → Select **ESP8266**  
3️⃣ **Copy the Blynk Auth Token**  
4️⃣ **Add Widgets:**
   - **Label (V0)** → Displays **Temperature**  
   - **LED (V1)** → **Turns ON if Fever Detected**  

---
#define BLYNK_TEMPLATE_ID "TMPL3AGuV_dpQ"
#define BLYNK_TEMPLATE_NAME "Auto Temperature Detector for Entrance for Covid"
#define BLYNK_AUTH_TOKEN "6jVByQwVus2y_bflTR2w9__9vwRScgbv"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

// **WiFi Credentials**
char ssid[] = "Hotspot";  // Your WiFi SSID
char pass[] = "car@1234"; // Your WiFi Password

// **Define sensor pins**
#define DHTPIN D4          // DHT11 connected to GPIO2 (D4)
#define DHTTYPE DHT11      // Define DHT sensor type
#define IR_SENSOR_PIN D5   // IR sensor connected to GPIO14 (D5)
#define BUZZER_PIN D6      // Buzzer connected to GPIO12 (D6)

// **Temperature Threshold**
const float TEMP_THRESHOLD = 37.5; // Fever threshold in Celsius

// **Initialize DHT sensor**
DHT dht(DHTPIN, DHTTYPE);

// **Initialize IR sensor**
IRrecv irrecv(IR_SENSOR_PIN);
decode_results results;

BlynkTimer timer; // Timer for periodic updates
float bodyTemperature;

void setup() {
    Serial.begin(115200);
    dht.begin();

    pinMode(BUZZER_PIN, OUTPUT);
    digitalWrite(BUZZER_PIN, LOW);

    irrecv.enableIRIn(); // Start the IR sensor

    // **Connect to WiFi and Blynk**
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    timer.setInterval(3000L, sendIRSensorDataToBlynk); // Send IR status every 3 sec
    timer.setInterval(5000L, sendTemperatureToBlynk); // Send temperature every 5 sec

    Serial.println("System Ready...");
}

// **Function to detect person using IR sensor**
bool detectPerson() {
    if (irrecv.decode(&results)) {
        Serial.println("✅ IR Sensor Detected Movement!");
        irrecv.resume(); // Receive the next IR signal
        return true;
    }
    return false;
}

// **Function to get accurate temperature**
float getAccurateTemperature() {
    float sum = 0;
    int validReadings = 0;

    // Take multiple readings for accuracy
    for (int i = 0; i < 7; i++) {  
        float temp = dht.readTemperature();
        
        // Ignore incorrect readings (DHT sometimes gives -40 or NaN)
        if (!isnan(temp) && temp > 0) {
            sum += temp;
            validReadings++;
        }
        delay(300);  
    }

    if (validReadings > 0) {
        return sum / validReadings;  // Return average temperature
    } else {
        return -1;  // Error reading temperature
    }
}

// **Function to send IR sensor data to Blynk**
void sendIRSensorDataToBlynk() {
    if (detectPerson()) {
        Serial.println("✅ Person Detected!");
        Blynk.virtualWrite(V2, "Person Detected");  // Update IR sensor status in Blynk
        Blynk.logEvent("person_detected"); // Trigger event notification
    } else {
        Serial.println("❌ No Person Detected...");
        Blynk.virtualWrite(V2, "No Person");  // Update IR sensor status in Blynk
    }
}

// **Function to send temperature data to Blynk**
void sendTemperatureToBlynk() {
    if (detectPerson()) {
        float bodyTemperature = getAccurateTemperature();

        if (bodyTemperature == -1) {
            Serial.println("⚠ Error Reading Temperature!");
            Blynk.virtualWrite(V0, "Error"); // Send error message to Blynk
        } else {
            Serial.print("Body Temperature: ");
            Serial.print(bodyTemperature);
            Serial.println(" °C");

            // Send temperature to Blynk
            Blynk.virtualWrite(V0, bodyTemperature);  

            if (bodyTemperature > TEMP_THRESHOLD) {
                Serial.println("⚠ ALERT: Fever Detected!");
                Blynk.virtualWrite(V1, 255);  // Turn ON Fever Alert LED in Blynk
                Blynk.logEvent("fever_detected"); // Trigger fever alert event

                digitalWrite(BUZZER_PIN, HIGH);
                delay(2000);
                digitalWrite(BUZZER_PIN, LOW);
            } else {
                Blynk.virtualWrite(V1, 0);  // Turn OFF Fever Alert LED in Blynk
                Serial.println("✅ Temperature Normal.");
            }
        }
    }
}

void loop() {
    Blynk.run();
    timer.run();
}

## **📜 Full Code (ESP8266 with Blynk, DHT11, IR Sensor)**
```cpp
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D4          // DHT11 connected to GPIO2 (D4)
#define DHTTYPE DHT11      // Define DHT sensor type
#define IR_SENSOR D5       // IR sensor connected to GPIO14 (D5)
#define BUZZER D6          // Buzzer connected to GPIO12 (D6)

DHT dht(DHTPIN, DHTTYPE);

// Replace with your Blynk credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN";  // Enter your Blynk Auth Token
char ssid[] = "YOUR_WIFI_SSID";         // Enter your WiFi SSID
char pass[] = "YOUR_WIFI_PASSWORD";     // Enter your WiFi Password

BlynkTimer timer;
float tempC;
const float tempThreshold = 37.5; // Fever threshold in Celsius

// Function to Read Temperature & Send to Blynk
void sendTemperature() {
    tempC = dht.readTemperature();

    Serial.print("Body Temperature: ");
    Serial.print(tempC);
    Serial.println(" °C");

    // Send data to Blynk
    Blynk.virtualWrite(V0, tempC);  // Update Temperature Label

    // Fever Alert
    if (tempC > tempThreshold) {
        Serial.println("⚠ ALERT: Fever Detected!");
        digitalWrite(BUZZER, HIGH);
        Blynk.virtualWrite(V1, 255); // LED ON in Blynk
        delay(2000);
        digitalWrite(BUZZER, LOW);
    } else {
        Blynk.virtualWrite(V1, 0); // LED OFF in Blynk
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();

    pinMode(IR_SENSOR, INPUT);
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    // Connect to WiFi & Blynk
    Blynk.begin(auth, ssid, pass);
    timer.setInterval(5000L, sendTemperature);  // Read temperature every 5 sec
}

void loop() {
    Blynk.run();
    timer.run();

    // Check if a person enters
    if (digitalRead(IR_SENSOR) == HIGH) {
        Serial.println("✅ Person Detected!");
        sendTemperature();
        delay(5000);
    }
}
```

---

## **🔹 How It Works**
1️⃣ **IR Sensor detects a person** entering  
2️⃣ **DHT11 reads body temperature**  
3️⃣ **Temperature is sent to Blynk App**  
4️⃣ If **temperature > 37.5°C**, **Buzzer Beeps & Blynk LED Turns ON**  
5️⃣ If **temperature is normal**, only the reading is updated  

---

## **🌐 Blynk Dashboard**
- **Open Blynk App** → Connect to ESP8266  
- **Check Temperature in Real-Time**  
- **Fever Alert LED Turns ON if Temp > 37.5°C**  

---

## **📌 Next Steps**
🚀 **Send Data to Google Sheets (for tracking fever history)**  
🚀 **Trigger SMS/Email Alert for High Temperature**  
🚀 **Control from Web Dashboard (Blynk/Thingspeak)**  

---

**✅ Ready to Upload & Test!**  
Let me know if you need **modifications or extra features**! 🚀🔥
