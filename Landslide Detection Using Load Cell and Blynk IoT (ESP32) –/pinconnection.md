### **🔌 Complete Wiring Guide for Landslide Detection System using ESP32**  
This guide provides **detailed pin connections** for **ESP32, Load Cells (HX711), Soil & Rain Sensors, OLED Display, LEDs, and Servo Motors**.

---

# **📡 Connection Table**
### **1️⃣ Load Cells (HX711) to ESP32**
| **HX711 Pin** | **Load Cell 1 ESP32 Pin** | **Load Cell 2 ESP32 Pin** |
|--------------|-----------------|-----------------|
| **VCC** | **3.3V** | **3.3V** |
| **GND** | **GND** | **GND** |
| **DT (DOUT)** | **GPIO 21** | **GPIO 17** |
| **SCK (CLK)** | **GPIO 22** | **GPIO 16** |

---

### **2️⃣ Soil & Rain Sensors to ESP32**
| **Sensor** | **ESP32 Pin** | **Function** |
|-----------|-------------|------------|
| **Soil Moisture Sensor** | **GPIO 34** | Measures soil wetness |
| **Rain Sensor** | **GPIO 35** | Detects rainfall intensity |

---

### **3️⃣ OLED Display (SSD1306) to ESP32 (Updated Pins)**
Since HX711 uses **GPIO 21 and 22**, we **change OLED I2C pins** to avoid conflicts:
| **OLED Pin** | **New ESP32 Pin (I2C)** |
|------------|-----------------|
| **SDA** | **GPIO 23** ✅ |
| **SCL** | **GPIO 5** ✅ |
| **VCC** | **3.3V** |
| **GND** | **GND** |

---

### **4️⃣ LED Indicators to ESP32**
| **LED** | **ESP32 Pin** | **Function** |
|--------|------------|------------|
| **Green LED (Entry Gate)** | **GPIO 25** | Indicates normal condition |
| **Red LED (Entry Gate)** | **GPIO 26** | Indicates landslide alert |
| **Green LED (Exit Gate)** | **GPIO 27** | Indicates normal condition |
| **Red LED (Exit Gate)** | **GPIO 32** | Indicates landslide alert |

✅ **Use a 220Ω resistor** in series with each LED to limit current.

---

### **5️⃣ Servo Motors to ESP32**
| **Servo Motor** | **ESP32 Pin** | **Function** |
|--------------|-------------|------------|
| **Entry Gate Servo** | **GPIO 18** | Controls Entry Gate |
| **Exit Gate Servo** | **GPIO 19** | Controls Exit Gate |

✅ **Servos need an external 5V power supply**, as ESP32 **cannot directly power them**.

---

# **🖥 Circuit Diagram Overview**
```
     +------------------------+
     |        ESP32           |
     |   (Microcontroller)     |
     +------------------------+
           |        |
           |        |
       [HX711]   [HX711]    
     (LoadCell1)  (LoadCell2)
         |        |
         |        |
     Soil Sensor  Rain Sensor
         |        |
     +------------------------+
     |       OLED Display     |
     +------------------------+
           |        |
      Green LED    Red LED
       (Normal)   (Warning)
           |        |
     +------------------------+
     |      Servo Motors      |
     +------------------------+
```

---

# **📌 Detailed Wiring for Each Component**
### **🔧 1. Load Cell Connection (HX711)**
1. **Connect Load Cell 1** to **HX711 Module 1**.
2. **Connect Load Cell 2** to **HX711 Module 2**.
3. Connect HX711 **VCC** → **ESP32 3.3V**.
4. Connect HX711 **GND** → **ESP32 GND**.
5. Connect **HX711 DOUT of Load Cell 1** → **ESP32 GPIO 21**.
6. Connect **HX711 SCK of Load Cell 1** → **ESP32 GPIO 22**.
7. Connect **HX711 DOUT of Load Cell 2** → **ESP32 GPIO 17**.
8. Connect **HX711 SCK of Load Cell 2** → **ESP32 GPIO 16**.

---

### **🔧 2. Soil Moisture & Rain Sensor Connection**
1. **Soil Sensor VCC** → **ESP32 3.3V**.
2. **Soil Sensor GND** → **ESP32 GND**.
3. **Soil Sensor Analog Output (A0)** → **ESP32 GPIO 34**.
4. **Rain Sensor VCC** → **ESP32 3.3V**.
5. **Rain Sensor GND** → **ESP32 GND**.
6. **Rain Sensor Analog Output (A0)** → **ESP32 GPIO 35**.

---

### **🔧 3. OLED Display Connection (SSD1306)**
1. **SDA (OLED) → ESP32 GPIO 23**.
2. **SCL (OLED) → ESP32 GPIO 5**.
3. **VCC (OLED) → ESP32 3.3V**.
4. **GND (OLED) → ESP32 GND**.

---

### **🔧 4. LED Indicator Connection**
1. **Green LED Entry → ESP32 GPIO 25** (with **220Ω resistor**).
2. **Red LED Entry → ESP32 GPIO 26** (with **220Ω resistor**).
3. **Green LED Exit → ESP32 GPIO 27** (with **220Ω resistor**).
4. **Red LED Exit → ESP32 GPIO 32** (with **220Ω resistor**).

---

### **🔧 5. Servo Motor Connection**
1. **Entry Gate Servo Signal (Yellow wire)** → **ESP32 GPIO 18**.
2. **Exit Gate Servo Signal (Yellow wire)** → **ESP32 GPIO 19**.
3. **Servo VCC (Red wire)** → **External 5V power supply**.
4. **Servo GND (Black wire)** → **ESP32 GND**.

✅ **Important:**  
- **Do NOT power servos directly from ESP32.**
- **Use an external 5V power source.**
- **Connect servo GND to ESP32 GND for common ground.**

---

# **📌 Final Notes**
- **If servos are not moving**, ensure you are using an **external 5V power supply**.
- **Check OLED connections** (must be GPIO 23 & 5, NOT 21 & 22).
- **Use a multimeter to test power connections.**
- **LEDs need a 220Ω resistor in series.**
- **Make sure HX711 modules share a common ground with ESP32.**

---

### **✅ Would you like to add a buzzer for alerts?**
Let me know if you need any refinements! 🚀😊
