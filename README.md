# IoT Sensor Condition Data Collection and Alert System

## Overview

This project is an IoT device that reads values from a DHT humidity and temperature sensor, a pressure sensor, a rotary potentiometer, a light sensor, and a sound sensor. It displays the sensor values on an OLED display and sends them to Adafruit IO over Wi-Fi using an ESP8266 module. If the temperature exceeds a threshold set by the rotary potentiometer, it turns on an LED and a buzzer.

## Technologies Used

- Arduino Uno
- C++
- Adafruit IO (for IoT data handling)
- ESP8266 Wi-Fi Module
- DHT Humidity and Temperature Sensor
- Pressure sensor
- Rotary Potentiometer
- Light Sensor
- Sound Sensor
- OLED Display

## Files

- `main.ino`: This is the main Arduino sketch file where the setup and loop functions are defined. It contains the logic for reading sensor data, displaying it on the OLED, and sending it to Adafruit IO.

## How to Run

1. Install the Arduino IDE on your computer
2. Install the necessary libraries in the Arduino IDE. You can do this by going to Sketch -> Include Library -> Manage Libraries, then search for and install the following libraries: DHT sensory library, Adafruit BMP280 library, U8g2 library, and SoftwareSerial library.
3. Open the `main.ino` file in the Arduino IDE.
4. Connect your Arduino board to your computer via USB
5. Select the correct board and port in the Arduino IDE
6. Click the "Upload" button to compile and upload the sketch to your Arduino board.
7. Open the Serial Monitor to view the sensor data.

## Key Accomplishments

Through this project, I gained hands-on experience with various aspects of IoT development. Here are some key points:

1. **Sensor Integration**: Implemented how to interface and collect data from various sensors like DHT, BMP280, a rotary potentiometer, a light sensor, and a sound sensor using Arduino.
2. **Data Display**: Implemented the use an OLED display to show real-time sensor data
3. **IoT Communication**: I learned how to use the ESP8266 Wi-Fi module to send sensor data to Adafruit IO, an IoT platform.
4. **Threshold Alert System**: I implemented a system that triggers an LED and a buzzer when the temperature exceeds a certain threshold.
5. **Software Serial Communication**: I implemented using the software serial communication to interact with the ESP8266 module.
6. **Cloud-based IoT Platform**: Learned how to use Adafruit IO for storing and visualizing IoT data.
