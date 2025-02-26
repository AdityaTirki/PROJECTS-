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
