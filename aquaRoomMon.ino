/**************************************************************************************************************
* Aquarium Room Monitor System
* Written by John Waltz
* 
* LAST UPDATED: 2/9/2017
* 
* Description:
*
* Components:
* 
*   DHT22 AM2302 Digital Temperature and Humidity Sensor
*   https://www.adafruit.com/products/393
*   
*   Waterproof DS18B20 Digital temperature sensor 
*   https://www.adafruit.com/products/381
*   
*   Industry Park Blue Backlight LCD Module
*   https://www.amazon.com/gp/product/B01LC4Q056/ref=oh_aui_detailpage_o03_s01?ie=UTF8&psc=1
*   
*   8-channel 5V Relay Module Shield
*   http://www.sainsmart.com/8-channel-dc-5v-relay-module-for-arduino-pic-arm-dsp-avr-msp430-ttl-logic.html
*
* PINOUT:
* 
* 5V Rail : 4 Dallas temperature sensors, I2C LCD Display, DHT Sensor, Relay Panel
* Ground  : 4 Dallas temperature sensors, I2C LCD Display, DHT Sensor, Relay Panel
* A4      : SDA for I2C LCD
* A5      : SCL for I2C LCD
* 2       : One wire data line for Dallas temperature sensors
* 4       : DHT data line
* 7       : Flashing LED to show code loop is running
* 5       : Relay 1 (future)
* 6       : Relay 2 (future)
* 8       : Relay 3 (future)
* 9       : Relay 4 (future)
* 10      : Relay 5 (future)
* 11      : Relay 6 (future)
* 12      : Relay 7 (future)
* 13      : Relay 9 (future)
* Add button for reset on outer case
*
*                      ** Some code used from online examples, no plagiarism intended**
*
**************************************************************************************************************/
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>  // Comes with Arduino IDE
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>
#include <DigitalToggle.h>

#define PROBE 3
#define DHTPIN 4
#define DHTTYPE DHT22   // DHT 22 Model (AM2302), AM2321
#define RUNNING 8
#define RELAY_1 5
#define RELAY_2 6
#define RELAY_3 8
#define RELAY_4 9
#define RELAY_5 10
#define RELAY_6 11
#define RELAY_7 12
#define RELAY_8 13

// Set I2C address for LCD display to 0x3F
// I2C Address scanner: http://playground.arduino.cc/Main/I2cScanner
// Set the pins on the I2C chip used for LCD connections: addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(PROBE);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup(void)
{
  pinMode(RUNNING, OUTPUT);
  sensors.begin();
  dht.begin();
  lcd.begin(20,4); // initialize the lcd for 20 chars 4 lines, turn on backlight
  //lcd.backlight(); // Turn backlight on 
  lcd.setCursor(1,0); //Start at character 1 on line 0
  lcd.print("Aquarium Room Mon"); // Print startup message for 5 seconds
  delay(5000);
  lcd.clear();
}
 
float temp1, temp2, temp3, temp4;
 
void loop(void)
{
  delay(2000); // Wait a couple seconds for DHT
  
  sensors.requestTemperatures(); // Get temperatures

  float h = dht.readHumidity();
  float f = dht.readTemperature(true); // true argument returns temp in F instead of C
  
  if (isnan(h) || isnan(f)){
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Room temp
  lcd.setCursor(0,0);
  lcd.print("Room: ");
  lcd.print(f,1);
  lcd.print((char)223);
  lcd.print("F");
  
  // Room humidity
  lcd.setCursor(13,0);
  lcd.print(h,1);
  lcd.print("% H");
  
  // Temp1 
  temp1 = sensors.getTempCByIndex(0);
  temp1 = (temp1 * 1.8) + 32;
  lcd.setCursor(0,2);
  lcd.print("A1: ");
  lcd.print(temp1,1);
  lcd.print((char)223);
  //lcd.print("F");

   // Temp2 
  temp2 = sensors.getTempCByIndex(1);
  temp2 = (temp2 * 1.8) + 32;
  lcd.setCursor(11,2);
  lcd.print("A2: ");
  lcd.print(temp2,1);
  lcd.print((char)223);
  //lcd.print("F");
  
  // Temp3
  temp3 = sensors.getTempCByIndex(2);
  temp3 = (temp3 * 1.8) + 32;
  lcd.setCursor(0,3);
  lcd.print("A3: ");
  lcd.print(temp3,1);
  lcd.print((char)223);
  //lcd.print("F");
  
  // Temp4 
  temp4 = sensors.getTempCByIndex(3);
  temp4 = (temp4 * 1.8) + 32;
  lcd.setCursor(11,3);
  lcd.print("A4: ");
  lcd.print(temp4,1);
  lcd.print((char)223);
  //lcd.print("F");

  digitalToggle(RUNNING); // Toggle LED pin as loop iterates
}
