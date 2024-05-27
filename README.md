# Internet of Things (IoT) Miniature Smart Parking System

This Arduino program implements a smart parking system using an ESP8266 microcontroller. It connects to a WiFi network, interacts with an MQTT broker for communication, and controls a servo motor to manage a parking gate. The program also uses infrared sensors to monitor the availability of parking slots and publishes this information to an MQTT topic.

## Components Used

- ESP8266 microcontroller
- Servo motor
- Ultrasonic distance sensors
- LEDs
- MQTT broker
- WiFi network

## Pin Configuration
- `D0`: Infrared sensor for parking slot 1 (`park1`)
- `D1`: Infrared sensor for parking slot 2 (`park2`)
- `D2`: Infrared sensor for gate entry (entry)
- `D3`: Servo motor control
- `D4`: LED 2 (green)
- `D5`: LED 1 (red)
- `D6`: LED 3 (white)

## Library Needed
- `ESP8266WiFi.h`: To connect to the WiFi network.
- `PubSubClient.h`: To handle MQTT communication.
- `ArduinoJson.h`: For JSON parsing and creation.
- `Servo.h`: To control the servo motor.

## Usage
1. Edit the `Wifi Connection Details` to match with your Wifi SSID and Password.
2. Load the program onto the ESP8266 microcontroller.
3. Ensure the hardware components are correctly connected as per the pin configuration.
4. Connect to an MQTT client in the application to subscribe to the topic `iot_arraydata` to receive updates about the parking slot status.

## You can view how the IoT works in this video:
[![Smart Parking System Demo](https://img.youtube.com/vi/83VEg3D-3bI/0.jpg)](https://youtu.be/83VEg3D-3bI?si=NvuTHA3JaO2m4WhP)
