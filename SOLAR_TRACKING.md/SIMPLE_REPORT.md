**Title:** Solar Tracking System Using Arduino and Servo Motor

**Abstract:** The Solar Tracking System is designed to optimize the efficiency of solar panels by continuously adjusting their orientation based on the position of the sun. This system employs Light Dependent Resistors (LDRs) as light sensors and a servo motor controlled by an Arduino microcontroller to adjust the solar panel's position. By maximizing the panel’s exposure to sunlight, this project enhances energy harvesting efficiency compared to fixed solar panels.

---

**1. Introduction:** With the increasing demand for renewable energy, solar power has emerged as a viable solution. However, fixed-position solar panels do not always receive maximum sunlight throughout the day. The Solar Tracking System dynamically adjusts the panel’s position based on sunlight intensity, ensuring better energy absorption and improved efficiency.

**2. Objectives:**

- To develop an automatic solar tracking system using Arduino.
- To enhance solar panel efficiency by maximizing sunlight exposure.
- To implement cost-effective and energy-efficient automation for solar tracking.

**3. Components Used:**

1. **Arduino Uno** – Microcontroller for system control.
2. **Servo Motor (SG90/MG995)** – Rotates the solar panel.
3. **Light Dependent Resistors (LDRs)** – Detects light intensity.
4. **Resistors (10kΩ)** – Used for LDR circuit stabilization.
5. **Solar Panel (Prototype)** – Represents a real-world solar panel.
6. **Power Supply (5V/9V Battery or Adapter)** – Powers the system.
7. **Wires and Breadboard** – For circuit connections.

**4. Methodology:** The system consists of two LDRs positioned at opposite ends of the solar panel to sense light intensity differences. The Arduino continuously monitors the LDR readings. If the left LDR detects higher light intensity than the right, the servo motor moves the panel left, and vice versa. This movement ensures optimal solar exposure throughout the day.

**5. Circuit Diagram and Working:**

1. The LDRs are connected to the analog pins (A0 and A1) of the Arduino.
2. The servo motor is connected to PWM pin 9 of the Arduino.
3. The Arduino reads the LDR values and determines the direction of movement.
4. The servo motor adjusts the solar panel’s orientation accordingly.
5. The process repeats continuously for real-time tracking.

**6. Algorithm:**

1. Read values from LDR1 and LDR2.
2. Compare LDR readings.
3. If LDR1 > LDR2, move the servo motor left.
4. If LDR2 > LDR1, move the servo motor right.
5. Update the servo position and repeat the process.

**7. Arduino Code:**

```cpp
#include <Servo.h>
Servo solarServo;
#define LDR1 A0
#define LDR2 A1
int servoPin = 9;
int position = 90;

void setup() {
  solarServo.attach(servoPin);
  Serial.begin(9600);
  solarServo.write(position);
  delay(1000);
}

void loop() {
  int ldr1Value = analogRead(LDR1);
  int ldr2Value = analogRead(LDR2);

  Serial.print("LDR1: "); Serial.print(ldr1Value);
  Serial.print("  LDR2: "); Serial.println(ldr2Value);

  if (ldr1Value > ldr2Value + 50) {
    if (position > 0) position -= 2;
  }
  else if (ldr2Value > ldr1Value + 50) {
    if (position < 180) position += 2;
  }

  solarServo.write(position);
  delay(200);
}
```

**8. Results and Observations:**

- The system successfully tracks sunlight and adjusts the solar panel position accordingly.
- It provides increased energy output compared to fixed solar panels.
- The response time is fast, and the system is highly efficient.
- The tracking is limited to a single-axis movement.

**9. Advantages and Limitations:** **Advantages:**

- Increases solar panel efficiency.
- Simple and cost-effective design.
- Low power consumption.
- Fully automated system.

**Limitations:**

- Limited to single-axis tracking.
- Performance affected by shadows and environmental conditions.
- Requires periodic calibration for optimal performance.

**10. Future Enhancements:**

- Implementing dual-axis tracking for better efficiency.
- Integrating a battery storage system for continuous operation.
- Using a stepper motor instead of a servo motor for precise movements.
- Adding IoT-based remote monitoring for real-time tracking.

**11. Conclusion:** The Solar Tracking System using Arduino and a servo motor provides an efficient solution for maximizing solar energy absorption. By dynamically adjusting the solar panel’s orientation, this system significantly enhances energy generation. With further improvements, it can be deployed for large-scale solar power applications.

**References:**

1. Datasheets of Arduino, Servo Motor, and LDR Sensors.
2. Research papers on solar tracking mechanisms.
3. Open-source Arduino programming documentation.

