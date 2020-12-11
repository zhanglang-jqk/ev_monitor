/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include <Arduino.h>
#include "main.h"
#include "ch/bsp.h"
#include "BLE_server.h"
#include "bme280.h"
#include "m702.h"
#include "lcd.h"
// #include "lcd.h"

/* define ---------------------------------------------------------------------------------------------------------------*/

#define LED 33

/* variable ---------------------------------------------------------------------------------------------------------------*/

SmartBuckle smartBuckles[SMART_BUCKLE_NUM];
/* function ---------------------------------------------------------------------------------------------------------------*/

char *SmartBuckle::toString()
{
  static char retStr[32] = {0};
  char tmpStr[16] = {0};
  dtostrf(temperature, 2, 1, tmpStr);
  sprintf(retStr, "T:%s H:%03d F:%d", tmpStr, heartbeat, finger_f);

  return retStr;
}

void setup()
{
  pinMode(LED, OUTPUT);

  Serial2.begin(9600);
  Serial2.println("Starting BLE work!");

  BME280_Init();
  // BS_Init();
  LCD_Init();
}

#define SENSOR_SCAN (60 * 1000)
u32 led_c = 0, disRefresh_c = 0;
bool led_f = false;
void loop()
{
  // put your main code here, to run repeatedly:

  // for (int i = 0; i < SMART_BUCKLE_NUM; i++)
  // {
  //   if (smartBuckles[i].isNew == true)
  //   {
  //     smartBuckles[i].isNew = false;

  //     char *retStr = smartBuckles[i].toString();
  //     Serial2.printf("smart buckle %d refresh : %s \r\n", i + 1, retStr);

  //     char disStr[32] = {0};
  //     sprintf(disStr, "%d %s", i + 1, smartBuckles[i].toString());

  //     display.fillScreen(GxEPD_WHITE);
  //     display.setCursor(0, (i + 1) * 10);
  //     display.println(disStr);
  //     display.update();
  //   }
  // }

  // if (millis() - disRefresh_c > SENSOR_SCAN)
  {
    BME280_Scan();
    M702_Scan();

    display.fillScreen(GxEPD_WHITE);
    display.setCursor(0, 11 * 10);

    display.printf("T:%3.1f  H:%3.1f \r\n", bmeInfo.temperature, bmeInfo.humidity);
    display.printf("A:%3.1f P:%3.1f", bmeInfo.altitude, bmeInfo.pressure);
    display.println();
    if (m702.isnew == true)
    {
      Serial2.printf("CO2:%d CH2O:%d TVOC:%d PM2.5:%d PM2.0:%d temperature:%.2f humidity:%.2f \r\n",
                     m702.CO2, m702.CH2O, m702.TVOC, m702.PM25, m702.PM10, m702.temperature, m702.humidity);

      display.printf("2:%d O:%d C:%d \r\n", m702.CO2, m702.CH2O, m702.TVOC);
      display.printf("PM25:%d PM20:%d \r\n", m702.PM25, m702.PM10);
      display.printf("T:%.1f H:%.1f", m702.temperature, m702.humidity);

      m702.isnew = false;
    }

    display.update();
    disRefresh_c = millis();
  }

  if (millis() - led_c > 100)
  {
    digitalRead(LED) == HIGH ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
    // led_f == false ? (digitalWrite(LED, LOW), led_f = true) : (digitalWrite(LED, HIGH), led_f = false);
    led_c = millis();
  }
}