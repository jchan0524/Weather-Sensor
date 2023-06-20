// Includes libraries
#include "Seeed_BMP280.h"   // Library for interfacing with Seeed board's sensors
#include <Wire.h>           // Library for I2C communication
#include <DHT.h>            // Library for DHT humidity and temperature sensor
#include "DHT.h"            // Library for DHT humidity and temperature sensor
#include <Arduino.h>        // Main Arduino library
#include <U8x8lib.h>        // Library for SSD1306 OLED display
#include <math.h>           // Standard math library
#include <SoftwareSerial.h> // Allows use two GPIO pins for a second UART

// Create a software serial object to communicate with ESP8266
SoftwareSerial espSerial(10, 11);

String IO_USERNAME = "";
String IO_KEY = "";
String WIFI_SSID = "";
String WIFI_PASS = "";
float num = 1.0; // Counts up to show upload working

// Define constants for DHT sensor
#define DHTPIN 3
#define DHTTYPE DHT11

// Create DHT, BMP280 and U8X8 (OLED display) objects
DHT dht(DHTPIN, DHTTYPE);
BMP280 bmp280;
U8X8_SSD1306_128X64_ALT0_HW_I2C u8x8(U8X8_PIN_NONE);

// Define pins for LED, buzzer, rotary encoder, light sensor, and sound sensor
int ledPin = 4;
int BuzzerPin = 5;
int rotaryPin = A0;
int sensorpin = A6;
int soundPin = A2;

// Define variables to hold sensor values
int rotaryValue = 0;
int sensorValue = 0;
int outputvalue = 0;

void setup()
{
  // Initialize serial communication, DHT sensor, OLED display, LED and buzzer pins
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
  u8x8.begin();
  u8x8.setPowerSave(0);
  u8x8.setFlipMode(1);

  pinMode(ledPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);

  // Check if BMP280 sensor is connected
  if (!bmp280.init())
  {
    Serial.println("Device Not Connected or broken!!!");
  }

  // Initialize software serial communication with ESP8266
  espSerial.begin(9600); // set up software UART to ESP8266 @ 9600 baud rate
  Serial.println("setting up");

  // Send Wi-Fi and Adafruit IO credentials to ESP8266
  String resp = espData("get_macaddr", 2000, false);     // get MAC address of 8266
  resp = espData("wifi_ssid=" + WIFI_SSID, 2000, false); // send Wi-Fi SSID to connect to
  resp = espData("wifi_pass=" + WIFI_PASS, 2000, false); // send password for Wi-Fi network
  resp = espData("io_user=" + IO_USERNAME, 2000, false); // send Adafruit IO info
  resp = espData("io_key=" + IO_KEY, 2000, false);
  resp = espData("setup_io", 15000, true); // setup the IoT connection

  // Check if Adafruit IO connection is successful
  if (resp.indexOf("connected") < 0)
  {
    Serial.println("\nAdafruit IO Connection Failed");
    while (1)
      ;
  }

  // Start data feed to Adafruit IO
  resp = espData("setup_feed=1,CPEG-ELEG298", 2000, false); // start the data feed
  Serial.println("------ Setup Complete ----------");
}

void loop()
{
  // Read temperature, humidity, and pressure values

  float temp, humi, pressure;

  temp = dht.readTemperature();
  humi = dht.readHumidity();
  // convert pascals into psi
  pressure = bmp280.getPressure() * .000145;

  // Print these values to the serial console
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C ");
  Serial.print("Humidity: ");
  Serial.print(humi);
  Serial.println(" %");
  Serial.print("Pressure:");
  Serial.print(pressure);
  Serial.println(" psi");

  // Print these values to the OLED display
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Temp: ");
  u8x8.print(temp);
  u8x8.print(" C  ");
  u8x8.setCursor(0, 1);
  u8x8.print("Humidity: ");
  u8x8.print(humi);
  u8x8.print("%");
  u8x8.setCursor(0, 2);
  u8x8.print("Press:");
  u8x8.print(pressure);
  u8x8.print(" psi");

  // Read the value of the rotary encoder
  int rotaryValue, threshValue;
  rotaryValue = analogRead(rotaryPin);

  // Sets the threshold value to be the rotaryValue/20 to get 51 increments then if the threshold value is greater than 50 it sets it to 50
  threshValue = (rotaryValue / 20);
  if (threshValue > 50)
  {
    threshValue = 50;
  }

  // Prints the threshold value to the Serial Display
  Serial.print("Threshold Value: ");
  Serial.print(threshValue);
  Serial.println(" C ");

  // Prints the threshold value to the OLED
  u8x8.setCursor(0, 3);
  u8x8.print("Thresh:");
  u8x8.print(threshValue);
  u8x8.print(" C  ");

  // if the temperature is greater than threshold, turn on the buzzer and the LED.
  if (temp > threshValue)
  {
    digitalWrite(ledPin, HIGH);
    analogWrite(BuzzerPin, 191);
  }
  else
  {
    digitalWrite(ledPin, LOW);
    analogWrite(BuzzerPin, 0);
  }

  // Read the light sensor and print the value to the serial console
  sensorValue = analogRead(sensorpin);
  Serial.print("Light (0-1023): ");
  Serial.print(sensorValue);

  // Read the sound sensor, convert the value to decibels, and print it to the serial console
  double soundState = analogRead(soundPin); // Read sound sensor’s value
  double sound = 20 * log10(soundState);    // conversion to decibels
  Serial.print("Sound: ");
  Serial.print(sound);
  Serial.println(" DB");

  // Send sensor data to Adafruit IO
  String resp = espData("send_data=1," + String(sensorValue), 2000, false); // send feed to cloud
  String soundData = espData("send_data=2," + String(sound), 2000, false);
  String tempData = espData("send_data=3," + String(temp), 2000, false);
  String humiData = espData("send_data=4," + String(humi), 2000, false);
  String pressureData = espData("send_data=5," + String(pressure), 2000, false);

  // Refresh the OLED display
  u8x8.refreshDisplay();

  // delay for 1 second
  delay(1000);
}

// Function to send a command to ESP8266 and receive a response
// The function waits for the specified timeout period for a response
String espData(String command, const int timeout, boolean debug)
{
  String response = "";
  espSerial.println(command); // send data to ESP8266 using serial UART
  long int time = millis();
  while ((time + timeout) > millis())
  { // wait the timeout period sent with the command
    while (espSerial.available())
    { // look for response from ESP8266
      char c = espSerial.read();
      response += c;
      Serial.print(c); // print response on serial monitor
    }
  }

  // If debug is true, the response is printed to the serial console
  if (debug)
  {
    Serial.println("Resp: " + response);
  }
  response.trim();
  return response;
}
