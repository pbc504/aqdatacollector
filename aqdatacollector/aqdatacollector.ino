// This program takes measurements every 2 seconds for NO, CO, Temperature and Relative Humidity and stores it on a SD card with a timestamp.

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include "RTClib.h"
#include <SD.h>


const int chipSelect = 10;          // SD location
Adafruit_ADS1115 ads1115NO(0x48);   // NO sensor memory location
Adafruit_ADS1115 ads1115CO(0x49);   // CO sensor memory location
int temperature_input = A0;         // Temperature input pin
int humidity_input = A1;            // Relative Humidity input pin
RTC_DS3231 rtc;                     // Define real time clock
int check = 1;

// Setup serial and SD interface. Initializes the ADCs.
void setup() {

  #ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
  #endif

  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
 

  Serial.println("Hello!");

  File dataFile = SD.open("logtest.txt", FILE_WRITE);
  dataFile.println("Unixtime, differential_01NO /mV, differential_23NO /mV, differential_01CO /mV, differential_23CO /mV, Temperature /mV, Humidity /mV");
  dataFile.close();
  
  delay(2000);
  ads1115NO.begin();
  ads1115CO.begin();
}


// Takes reading repeatedly every 2 seconds:
void loop() {
  
  static const unsigned long REFRESH_INTERVAL = 2000; // ms
  static unsigned long lastRefreshTime = 0;

  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
    lastRefreshTime += REFRESH_INTERVAL;
        takeReadings();
  }

}


// Records all the data points, stores them in a string. Then writes the string to a text file in the SD card
void takeReadings(){

  DateTime now = rtc.now(); //gets current time

  // Definning sensors output values
  int16_t differential_01NO, differential_23NO, differential_01CO, differential_23CO;
  // Reads the differential between the different 4 electrodes for both CO and NO sensors
  differential_01NO = ads1115NO.readADC_Differential_0_1();                            
  differential_23NO = ads1115NO.readADC_Differential_2_3();
  differential_01CO = ads1115CO.readADC_Differential_0_1();
  differential_23CO = ads1115CO.readADC_Differential_2_3();

  
  // Definning sensors output values
  uint16_t temperatureValue = analogRead(temperature_input) * (4000/1023);   // Takes in an analog voltage reading
  uint16_t humidityValue = analogRead(humidity_input) * (4000/1023); 

  float tempValue = temperatureValue; // Coverts from uint16_t to float (decimal points)
  float humidValue = humidityValue;

  // Creates the text file to log the data.
  File dataFile = SD.open("logtest.txt", FILE_WRITE);
  
  // Creates the string, readings, to be printed on the file reading1.txt on the SD card.
  String readings = "";
  

// 6144mV / 32767 = 0.1875, 2^15 = 32767 because the ADC is a 16 bit signed, first bit decides if negative or positive
// Appends each datapoint to the string

// 1000v/32767 = 0.3267

 readings.concat(now.unixtime()+ 3353);
 readings.concat(",");
 readings.concat(differential_01NO * 0.32767);
 readings.concat(",");
 readings.concat(differential_23NO * 0.1875);
 readings.concat(",");
 readings.concat(differential_01CO * 0.1875);
 readings.concat(",");
 readings.concat(differential_23CO * 0.1875);
 readings.concat(",");
 readings.concat(tempValue);
 readings.concat(",");
 readings.concat(humidValue);

// If the file has been creating, it writes the sting to the text file and prints to the serial aswell
 if (dataFile) {
  
    dataFile.println(readings);
    dataFile.close();
     //print to the serial port too:
    Serial.println(readings);
  }
  else{
    Serial.println("Error opening logtest.txt");
  }

 
  
}
