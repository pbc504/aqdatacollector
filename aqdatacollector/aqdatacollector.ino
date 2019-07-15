#include <Wire.h>
#include <SPI.h>
#include <Adafruit_ADS1015.h>
#include "RTClib.h"


Adafruit_ADS1115 ads1115NO(0x48);
Adafruit_ADS1115 ads1115CO(0x49);

int temperature_input = A0; // temperature
int humidity_input = A1; // humidity

RTC_DS3231 rtc;


void setup() {
  // put your setup code here, to run once:

  
  #ifndef ESP8266

  while (!Serial); // for Leonardo/Micro/Zero

  #endif

  Serial.begin(9600);
  Serial.println("Hello!");
  delay(3000);
  ads1115NO.begin();
  ads1115CO.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
DateTime now = rtc.now();
  
  int16_t differential_01NO, differential_23NO, differential_01CO, differential_23CO;
  differential_01NO = ads1115NO.readADC_Differential_0_1();
  differential_23NO = ads1115NO.readADC_Differential_2_3();
  differential_01CO = ads1115CO.readADC_Differential_0_1();
  differential_23CO = ads1115CO.readADC_Differential_2_3();

  uint16_t temperatureValue = analogRead(temperature_input);
  uint16_t humidityValue = analogRead(humidity_input);

  float tempValue = temperatureValue;
  float humidValue = humidityValue;

  
  // 6144mV / 32767 = 0.1875, 2^15 = 23767 because the ADC is a 16 bit signed, first bit decides if negative or positive  
 Serial.print(now.unixtime()+ 3353);
 Serial.print(",");
 Serial.print(differential_01NO * 0.1875);
 Serial.print(",");
 Serial.print(differential_23NO * 0.1875);
 Serial.print(",");
 Serial.print(differential_01CO * 0.1875);
 Serial.print(",");
 Serial.print(differential_23CO * 0.1875);
 Serial.print(",");
 Serial.print(tempValue);
 Serial.print(",");
 Serial.println(humidValue);
 

}
