#include "HX711.h"
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

// **Load Cell (HX711) Configuration**
#define LOADCELL1_DOUT 21  
#define LOADCELL1_SCK 22   
#define LOADCELL2_DOUT 17  
#define LOADCELL2_SCK 16   

HX711 scale1, scale2;

// **Soil & Rain Sensor Configuration**
#define SOIL_SENSOR 34  
#define RAIN_SENSOR 35  

// **LED Configuration (Entry & Exit Gates)**
#define GREEN_LED_ENTRY 25  
#define RED_LED_ENTRY 26   
#define GREEN_LED_EXIT 27  
#define RED_LED_EXIT 32  

// **Servo Motors (Entry & Exit Gates)**
Servo servoEntry;  
Servo servoExit;  
#define SERVO_ENTRY_PIN 18  
#define SERVO_EXIT_PIN 19  

// **OLED Display Configuration**
#define OLED_SDA 23  
#define OLED_SCL 5   
#define OLED_ADDRESS 0x3C

TwoWire I2Cbus = TwoWire(0);
Adafruit_SSD1306 display(128, 64, &I2Cbus, -1);

// **New Threshold for Landslide Detection (Reduced to 3kg)**
#define ENTRY_THRESHOLD 3.0  // Load Cell 1 (Entry)
#define EXIT_THRESHOLD 3.0   // Load Cell 2 (Exit)

void setup() {
    Serial.begin(115200);

    // **Initialize Load Cells**
    scale1.begin(LOADCELL1_DOUT, LOADCELL1_SCK);
    scale2.begin(LOADCELL2_DOUT, LOADCELL2_SCK);
    
    scale1.set_scale(2280.0);  
    scale2.set_scale(2280.0);
    scale1.tare();
    scale2.tare();

    // **Initialize Servo Motors**
    servoEntry.attach(SERVO_ENTRY_PIN);
    servoExit.attach(SERVO_EXIT_PIN);
    servoEntry.write(0);  // Closed position
    servoExit.write(0);

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
    I2Cbus.begin(OLED_SDA, OLED_SCL);
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
    float weightEntry = scale1.get_units(5);  
    float weightExit = scale2.get_units(5);   

    // Ensure values are within expected range
    weightEntry = constrain(weightEntry, 0, 5.0);
    weightExit = constrain(weightExit, 0, 5.0);

    // **Read Soil & Rain Sensors**
    int soilMoisture = analogRead(SOIL_SENSOR);
    int rainLevel = analogRead(RAIN_SENSOR);

    // Convert to percentage
    int soilMoisturePercent = map(soilMoisture, 4095, 0, 0, 100);
    int rainLevelPercent = map(rainLevel, 4095, 0, 0, 100);

    // **Display Data on OLED**
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("LANDSLIDE MONITOR");

    display.setTextSize(2);
    display.setCursor(0, 15);
    display.print("Entry: ");
    display.print(weightEntry);
    display.print("kg");

    display.setCursor(0, 30);
    display.print("Exit: ");
    display.print(weightExit);
    display.print("kg");

    display.setCursor(0, 45);
    display.print("Soil: ");
    display.print(soilMoisturePercent);
    display.print("%");

    display.setCursor(75, 45);
    display.print("Rain: ");
    display.print(rainLevelPercent);
    display.print("%");

    display.display();

    // **Landslide Detection Logic for Entry Gate**
    if (weightEntry > ENTRY_THRESHOLD) {  
        Serial.println("⚠️ WARNING: Landslide at Entry Gate!");
        servoEntry.write(180);  // Fully Open Entry Gate
        digitalWrite(RED_LED_ENTRY, HIGH);
        digitalWrite(GREEN_LED_ENTRY, LOW);
    } else {
        servoEntry.write(0);  // Close Entry Gate
        digitalWrite(GREEN_LED_ENTRY, HIGH);
        digitalWrite(RED_LED_ENTRY, LOW);
    }

    // **Landslide Detection Logic for Exit Gate**
    if (weightExit > EXIT_THRESHOLD) {  
        Serial.println("⚠️ WARNING: Landslide at Exit Gate!");
        servoExit.write(180);  // Fully Open Exit Gate
        digitalWrite(RED_LED_EXIT, HIGH);
        digitalWrite(GREEN_LED_EXIT, LOW);
    } else {
        servoExit.write(0);  // Close Exit Gate
        digitalWrite(GREEN_LED_EXIT, HIGH);
        digitalWrite(RED_LED_EXIT, LOW);
    }

    delay(1000);  // 1 second delay
}
