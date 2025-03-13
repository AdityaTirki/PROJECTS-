Yes, **Load Cell (HX711) and OLED Display (SSD1306) both use GPIO 21 and GPIO 22 in the previous connection.**  
Since **HX711 uses GPIO 21 (DOUT) and GPIO 22 (SCK)**, we need to **change the OLED I2C pins** to avoid conflicts.

---

## **📌 Updated Pin Assignments**
Since **GPIO 21 & 22 are used for HX711**, we will **change OLED I2C pins** to another set:

| **Component** | **Old GPIO Pins** | **New GPIO Pins** |
|--------------|-----------------|-----------------|
| **OLED SDA** | **GPIO 21** | **GPIO 23** ✅ |
| **OLED SCL** | **GPIO 22** | **GPIO 5** ✅ |

---
### **📡 Corrected Connection Table**
#### **1️⃣ Load Cells (HX711) - Keep the Same**
| **HX711 Pin** | **Load Cell 1 ESP32 Pin** | **Load Cell 2 ESP32 Pin** |
|--------------|-----------------|-----------------|
| **VCC** | **3.3V** | **3.3V** |
| **GND** | **GND** | **GND** |
| **DT (DOUT)** | **GPIO 21** | **GPIO 17** |
| **SCK (CLK)** | **GPIO 22** | **GPIO 16** |

---
#### **2️⃣ Updated OLED Display (SSD1306) - Change Pins**
| **OLED Pin** | **New ESP32 Pin (I2C)** |
|------------|-----------------|
| **SDA** | **GPIO 23** ✅ |
| **SCL** | **GPIO 5** ✅ |
| **VCC** | **3.3V** |
| **GND** | **GND** |

---

### **📝 Update in Code**
Modify the **OLED display initialization** to use **GPIO 23 (SDA) and GPIO 5 (SCL)**:

```cpp
#define OLED_SDA 23  // Changed from 21 to 23
#define OLED_SCL 5   // Changed from 22 to 5

TwoWire I2Cbus = TwoWire(0);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

void setup() {
    I2Cbus.begin(OLED_SDA, OLED_SCL);  // Initialize OLED with new I2C pins

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("⚠️ OLED display not found!");
        while (1);
    }
}
```

---

## **🚀 Final Fixes**
✅ **Move OLED I2C Pins from GPIO 21, 22 → GPIO 23 (SDA) & GPIO 5 (SCL)**  
✅ **Ensure HX711 and OLED work independently without pin conflicts**  
✅ **Update code to initialize OLED with new I2C pins**  

Would you like to test this setup and let me know if you need more refinements? 🚀😊
Here is the **updated Landslide Detection Code** with **OLED Display**, **2 Red LEDs (for warnings)**, and **2 Green LEDs (for normal conditions)**.  
This version:
✅ **Reads weight from two 5kg load cells (max 10kg)**  
✅ **Reads soil moisture & rain sensor data**  
✅ **Controls two servo motors (for gates or barriers)**  
✅ **Displays values on OLED (128x64 SSD1306)**  
✅ **Controls LED indicators (Red for Alert, Green for Normal)**  
✅ **Removes Blynk IoT**  

---

## **🔌 Wiring Connections**
### **🛠️ Load Cell Connections (HX711)**
| **HX711 Pin** | **ESP32 Pin** |
|--------------|--------------|
| **VCC** | 3.3V |
| **GND** | GND |
| **DT (DOUT) Load Cell 1** | 21 |
| **SCK (CLK) Load Cell 1** | 22 |
| **DT (DOUT) Load Cell 2** | 17 |
| **SCK (CLK) Load Cell 2** | 16 |

### **🌱 Soil & Rain Sensor Connections**
| **Sensor** | **ESP32 Pin** |
|--------------|--------------|
| **Soil Moisture Sensor** | 34 |
| **Rain Sensor** | 35 |

### **🖥 OLED Display (SSD1306)**
| **OLED Pin** | **ESP32 Pin (I2C)** |
|------------|-----------------|
| **SDA** | 21 |
| **SCL** | 22 |

### **🚦 LED Indicators**
| **LED** | **ESP32 Pin** | **Function** |
|--------|------------|------------|
| **Green LED (Entry)** | 25 | Normal Condition |
| **Red LED (Entry)** | 26 | Landslide Alert |
| **Green LED (Exit)** | 27 | Normal Condition |
| **Red LED (Exit)** | 32 | Landslide Alert |

### **⚙️ Servo Motors (Gate Control)**
| **Servo Motor** | **ESP32 Pin** |
|--------------|-------------|
| **Entry Gate Servo** | 18 |
| **Exit Gate Servo** | 19 |

---

## **📌 Updated Landslide Detection Code (ESP32, HX711, OLED, LEDs, Servo)**
```cpp
#include "HX711.h"
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// **Load Cell (HX711) Configuration**
#define LOADCELL1_DOUT 21  
#define LOADCELL1_SCK 22   
#define LOADCELL2_DOUT 17  
#define LOADCELL2_SCK 16   

HX711 scale1;
HX711 scale2;

// **Soil & Rain Sensor Configuration**
#define SOIL_SENSOR 34  // Soil Moisture Sensor connected to GPIO 34
#define RAIN_SENSOR 35  // Rain Sensor connected to GPIO 35

// **Servo Motor Configuration**
Servo servo1;  // First servo for entry gate
Servo servo2;  // Second servo for exit gate
int servoPin1 = 18;  // Entry gate servo connected to GPIO 18
int servoPin2 = 19;  // Exit gate servo connected to GPIO 19

// **LED Configuration**
#define GREEN_LED_ENTRY 25
#define RED_LED_ENTRY 26
#define GREEN_LED_EXIT 27
#define RED_LED_EXIT 32

// **OLED Display Configuration**
#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
    Serial.begin(115200);  // Start Serial Monitor

    // **Initialize Load Cells**
    scale1.begin(LOADCELL1_DOUT, LOADCELL1_SCK);
    scale2.begin(LOADCELL2_DOUT, LOADCELL2_SCK);
    Serial.println("Calibrating Load Cells...");
    
    scale1.set_scale(2280.0);  // Adjust after calibration
    scale2.set_scale(2280.0);
    scale1.tare();
    scale2.tare();

    // **Initialize Servo Motors**
    servo1.attach(servoPin1);
    servo2.attach(servoPin2);
    servo1.write(0);  // Start in closed position
    servo2.write(0);

    // **Initialize LEDs**
    pinMode(GREEN_LED_ENTRY, OUTPUT);
    pinMode(RED_LED_ENTRY, OUTPUT);
    pinMode(GREEN_LED_EXIT, OUTPUT);
    pinMode(RED_LED_EXIT, OUTPUT);

    digitalWrite(GREEN_LED_ENTRY, HIGH);
    digitalWrite(RED_LED_ENTRY, LOW);
    digitalWrite(GREEN_LED_EXIT, HIGH);
    digitalWrite(RED_LED_EXIT, LOW);

    // **Initialize OLED Display**
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
        Serial.println("⚠️ OLED display not found!");
        while (1);
    }

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10, 20);
    display.println("System Init...");
    display.display();
    delay(2000);

    Serial.println("Landslide Detection System Ready");
}

void loop() {
    // **Read Load Cells**
    float weight1 = scale1.get_units(5); 
    float weight2 = scale2.get_units(5);
    
    // Ensure no negative values and limit max reading
    weight1 = constrain(weight1, 0, 5.0); 
    weight2 = constrain(weight2, 0, 5.0);
    
    float totalWeight = weight1 + weight2;
    totalWeight = constrain(totalWeight, 0, 10.0); // Max total weight: 10kg

    // **Read Soil Moisture & Rain Sensor**
    int soilMoisture = analogRead(SOIL_SENSOR);
    int rainLevel = analogRead(RAIN_SENSOR);

    // Convert raw values to percentage
    int soilMoisturePercent = map(soilMoisture, 4095, 0, 0, 100);
    int rainLevelPercent = map(rainLevel, 4095, 0, 0, 100);

    // **Display Data on OLED**
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("LANDSLIDE MONITOR");

    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("W1: ");
    display.print(weight1);
    display.print("kg");

    display.setCursor(0, 30);
    display.print("W2: ");
    display.print(weight2);
    display.print("kg");

    display.setCursor(0, 45);
    display.print("Total: ");
    display.print(totalWeight);
    display.print("kg");

    display.setCursor(0, 60);
    display.print("Soil: ");
    display.print(soilMoisturePercent);
    display.print("% Rain: ");
    display.print(rainLevelPercent);
    display.print("%");

    display.display();

    // **Landslide Detection Logic**
    if (totalWeight > 8.0) { // Landslide Alert when weight > 8kg
        Serial.println("⚠️ WARNING: Landslide Detected!");
        
        // **Trigger Servo Motors to Open Gates**
        servo1.write(90); // Open Entry Gate
        servo2.write(90); // Open Exit Gate
        
        // **Turn ON Red LEDs (Alert Mode)**
        digitalWrite(RED_LED_ENTRY, HIGH);
        digitalWrite(RED_LED_EXIT, HIGH);
        digitalWrite(GREEN_LED_ENTRY, LOW);
        digitalWrite(GREEN_LED_EXIT, LOW);
    } else {
        // **Keep gates closed and Normal Mode**
        servo1.write(0);
        servo2.write(0);
        
        digitalWrite(GREEN_LED_ENTRY, HIGH);
        digitalWrite(GREEN_LED_EXIT, HIGH);
        digitalWrite(RED_LED_ENTRY, LOW);
        digitalWrite(RED_LED_EXIT, LOW);
    }

    delay(1000); // 1 second delay
}
```

---

### **🚀 Summary of Features**
✅ **OLED Display for Real-time Monitoring**  
✅ **2 Load Cells (5kg each) for Landslide Detection**  
✅ **Soil Moisture & Rain Sensors for Risk Assessment**  
✅ **Red & Green LEDs for Alert Indication**  
✅ **Servo Motors for Automatic Gate Control**  

Would you like to add **a buzzer for audible alerts**? Let me know! 🚀😊
