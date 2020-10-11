#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal.h>

#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;

int  PhoResPin  =  A1;     // select the input  pin for  the potentiometer
int  PhoResLedPin  =  13;   // select the pin for  the LED
int  PhoResValue =  0;  // variable to  store  the value  coming  from  the sensor

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#define DHTPIN 3     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;


//Initialize the serial port.
void setup() {
  // put your setup code here, to run once:
  // initialize serial communication at 9600 bits per second:
  // pinMode(ledPin,OUTPUT);
  // set up the LCD's number of columns and rows:
  Serial.begin(9600);
  
  // Initialize DS3231
  Serial.println("Initialize DS3231");;
  clock.begin();

  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Plant Monitor v2.0");
  delay(500);
  lcd.clear();
  // Initialize device.
  dht.begin();
  Serial.println(F("DHT11 Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}


void loop() {
  // put your main code here, to run repeatedly:

  dt = clock.getDateTime();
  //We force temperature conversions, poniweaz by default it is weaned every 64 seconds
  clock. forceConversion();
 
  Serial. print(("Temp_Clock: "));
  Serial. println((clock. readTemperature()*9/5)+32);
  
  // For leading zero look to DS3231_dateformat example
  Serial.print(("Raw data: "));
  Serial.print(dt.year);   Serial.print("-");
  Serial.print(dt.month);  Serial.print("-");
  Serial.print(dt.day);    Serial.print(" ");
  Serial.print(dt.hour);   Serial.print(":");
  Serial.print(dt.minute); Serial.print(":");
  Serial.print(dt.second); Serial.println("");

  delay(delayMS/10);
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
//  lcd.setCursor(13, 1);
//  // print the number of seconds since reset:
//  lcd.print(millis() / 1000);
  lcd.setCursor(0, 0);
  lcd.print("T");
  lcd.setCursor(5, 0);
  lcd.print("H");
  lcd.setCursor(10, 0);
  lcd.print("W");
  


  // read the input on analog pin 0:
  int WaterLevelValue = analogRead(A0);
  // print out the value you read:
  Serial.println(WaterLevelValue);
  lcd.setCursor(11,0);
  lcd.print(WaterLevelValue);
  delay(delayMS);        // delay in between reads for stability

//  // read the input on analog pin 1:
//  int LightSensorValue = analogRead(A1);
//  // print out the value you read:
//  Serial.print(LightSensorValue);
//  Serial.println(" LUX");
//  lcd.setCursor(0,1);
//  lcd.print("L");
//  lcd.setCursor(1,1);
//  lcd.print(LightSensorValue);
//  delay(delayMS);        // delay in between reads for stability

  PhoResValue =  analogRead(PhoResPin);
  digitalWrite(PhoResPin,  HIGH);
  delay(delayMS);
  digitalWrite(PhoResLedPin,  LOW);
  delay(delayMS);
  Serial.println(PhoResValue,  DEC);
  lcd.setCursor(0,1);
  lcd.print("L");
  lcd.setCursor(1,1);
  lcd.print(PhoResValue);
  delay(delayMS);

  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    lcd.setCursor(1, 0);
    lcd.print("ERR!");
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(((event.temperature * 9 / 5) + 32));
    Serial.println(F("°F"));
    lcd.setCursor(1, 0);
    lcd.print(((event.temperature*9/5)+32));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    lcd.setCursor(6, 0);
    lcd.print(("ERR!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    lcd.setCursor(6, 0);
    lcd.print((event.relative_humidity));
  }
    Serial.print(("Avg Temp: "));
    Serial.println((((((event.temperature*9)/5)+32)*.5+ (((clock. readTemperature()*9)/5)+32))*.5));
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(8, 1);
  // print the number of seconds since reset:
  lcd.print(dt.hour);
  lcd.setCursor(10, 1);
  lcd.print(":");
  lcd.setCursor(11, 1);
  lcd.print(dt.minute);
  lcd.setCursor(13, 1);
  lcd.print(":");
  lcd.setCursor(14, 1);
  lcd.print(dt.second);
}
