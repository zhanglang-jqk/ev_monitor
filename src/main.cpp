/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include <Arduino.h>
#include "main.h"
#include "BLE_server.h"
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

  Serial2.begin(115200);
  Serial2.println("Starting BLE work!");

  BS_Init();
  LCD_Init();
}

void loop()
{
  // put your main code here, to run repeatedly:

  for (int i = 0; i < SMART_BUCKLE_NUM; i++)
  {
    if (smartBuckles[i].isNew == true)
    {
      smartBuckles[i].isNew = false;

      char *retStr = smartBuckles[i].toString();
      Serial2.printf("smart buckle %d refresh : %s \r\n", i + 1, retStr);

      char disStr[32] = {0};
      sprintf(disStr, "%d %s", i + 1, smartBuckles[i].toString());

      display.fillScreen(GxEPD_WHITE);
      display.setCursor(0, (i + 1) * 10);
      display.println(disStr);
      display.update();
    }
  }

  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}