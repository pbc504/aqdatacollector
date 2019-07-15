#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include "RTClib.h"


Adafruit_ADS1115 ads1115NO(0x48);
Adafruit_ADS1115 ads1115CO(0x49);


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

  

  
  int16_t differential1, differential2, differential3, differential4;
  
  differential1 = ads1115NO.readADC_Differential_0_1();
  differential2 = ads1115NO.readADC_Differential_2_3();
  differential3 = ads1115CO.readADC_Differential_0_1();
  differential4 = ads1115CO.readADC_Differential_2_3();



  
  // 6144mV / 32767 = 0.1875, 2^15 = 23767 because the ADC is a 16 bit signed, first bit decides if negative or positive
  Serial.print("NO Differential between 0 and 1: "); Serial.print(differential1 * 0.1875); Serial.println("mV");
  getTime();
  Serial.print("NO Differential between 2 and 3: "); Serial.print(differential2 * 0.1875); Serial.println("mV");
  getTime();
  Serial.print("CO Differential between 0 and 1: "); Serial.print(differential3 * 0.1875); Serial.println("mV");
  getTime();
  Serial.print("CO Differential between 2 and 3: "); Serial.print(differential4 * 0.1875); Serial.println("mV");
  getTime();
 
  delay(2000);
}

DateTime getTime(){
  DateTime now = rtc.now() + TimeSpan(0,0,56,6);

  
   Serial.print(now.year(), DEC);

    Serial.print('/');

    Serial.print(now.month(), DEC);

    Serial.print('/');

    Serial.print(now.day(), DEC);

    Serial.print(' ');

    Serial.print(now.hour(), DEC);

    Serial.print(':');

    Serial.print(now.minute(), DEC);

    Serial.print(':');

    Serial.print(now.second(), DEC);

    Serial.println();
    Serial.println();

}
