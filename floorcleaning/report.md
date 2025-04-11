BLYNK-CONTROLLED IOT-BASED SMART FLOOR CLEANING ROBOT
1. ABSTRACT
This project focuses on the development of a smart, IoT-enabled floor cleaning robot controlled via the Blynk IoT platform. It incorporates multiple subsystems, including directional movement through DC motors, relay-controlled vacuum and mop cleaning mechanisms, environmental condition monitoring via DHT11 and MQ-3 sensors, and real-time obstacle detection using ultrasonic sensors. The ESP32 microcontroller serves as the core of the system, executing control logic and communicating with the Blynk cloud for remote operation. The robot can be manually navigated and cleaned using a mobile app, making it suitable for smart homes, offices, and healthcare environments.

2. INTRODUCTION
In today’s technology-driven world, automation is increasingly applied to routine maintenance tasks like cleaning. Traditional manual cleaning is labor-intensive and inconsistent. The integration of IoT with embedded systems opens doors for smarter, remotely controllable robots capable of operating efficiently and autonomously. This project leverages ESP32 and Blynk IoT to create a modular, real-time controlled smart floor cleaning robot capable of manual operation, remote activation, and environmental feedback.

3. OBJECTIVES
Enable manual remote control of robot movement via the Blynk app

Operate cleaning mechanisms (vacuum and mop motors) independently

Monitor temperature, humidity, and air quality and send data to the Blynk dashboard

Perform real-time obstacle detection and reactive movement

Maintain modular design for future automation and path planning

4. METHODOLOGY
4.1 Environmental Monitoring System
Sensors Used: DHT11 (Temperature & Humidity), MQ-3 (Air Quality)

Data is read by ESP32 and displayed on Blynk virtual pins V6 and V7

These readings are for monitoring only and not linked to any action

4.2 Car Movement and Obstacle Avoidance
Controlled via 4 DC motors and an L298N driver

Commands issued via Blynk virtual pins V1–V4

Ultrasonic sensors detect obstacles (front-left and front-right)

ESP32 executes movement logic and obstacle reactions

4.3 Cleaning Mechanism Control (Manual)
775 motor (vacuum) and mop motor are connected via relays

Controlled using Blynk virtual pins V5 and V8

Operation is independent from sensor or movement states

4.4 System Integration
ESP32 handles motor controls, sensors, and Blynk communication

Timer-based execution avoids blocking

Powered via a 12V regulated battery

5. COMPONENTS REQUIRED
Component	Quantity
ESP32 Development Board	1
DC Gear Motors (movement)	4
Mop Cleaning DC Motor	1
775 Vacuum Motor	1
L298N Motor Driver Module	1
Relay Modules (1-Channel)	2
Ultrasonic Sensors	2
DHT11 Sensor	1
MQ-3 Gas Sensor	1
Blynk IoT App	1
Jumper Wires, PCB, Breadboard	-
12V Battery or Power Module	1
6. SOFTWARE IMPLEMENTATION: BLYNK IOT INTEGRATION
Virtual Pin	Function	Widget Type
V1	Move Forward	Button
V2	Move Backward	Button
V3	Turn Left	Button
V4	Turn Right	Button
V5	Vacuum Motor (Relay)	Switch
V6	Temperature & Humidity	LCD/Text
V7	Air Quality (MQ-3)	Value Display
V8	Mop Cleaning Motor (Relay)	Switch
ESP32 connects to Wi-Fi and communicates with Blynk Cloud. All commands and sensor data are handled via BlynkTimer, ensuring efficient operation.

7. RESULTS AND DISCUSSION
Movement commands (forward, backward, left, right) responded instantly via Blynk app

Obstacle detection worked effectively with real-time reaction

Environmental values (DHT11 and MQ-3) updated reliably on the app

Mop and vacuum relays switched ON/OFF accurately through the app

System ran without interference between subsystems, confirming stability

8. CONCLUSION
This project demonstrates how embedded systems combined with IoT technology can create intelligent, efficient cleaning robots. The modularity ensures ease of expansion, and real-time Blynk control provides a user-friendly experience. It’s suitable for applications in homes, hospitals, or industrial spaces and serves as a foundation for future smart automation projects.

9. FUTURE SCOPE
Add autonomous path planning (using SLAM, IR grids, or AI algorithms)

Implement automatic docking and battery recharging system

Enable voice assistant integration (e.g., Google Assistant or Alexa)

Add onboard camera for surveillance and dirt detection

Expand environmental monitoring to include CO₂, PM2.5, etc.
