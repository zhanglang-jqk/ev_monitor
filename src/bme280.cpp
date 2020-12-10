/***********************************************************************
 * @file 	:	bmp280.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-11  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "bme280.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)
/* 私有变量 ---------------------------------------------------------------------*/
/* 扩展变量 ---------------------------------------------------------------------*/
BME_280Info bmeInfo;
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

void BME280_Init()
{
    u8 status = bme.begin();
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status)
    {
        Serial2.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial2.print("SensorID was: 0x");
        Serial2.println(bme.sensorID(), 16);
        Serial2.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial2.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial2.print("        ID of 0x60 represents a BME 280.\n");
        Serial2.print("        ID of 0x61 represents a BME 680.\n");
        while (1)
            delay(10);
    }
}

void BME280_Scan()
{
    bmeInfo.temperature = bme.readTemperature();
    bmeInfo.humidity = bme.readHumidity();
    bmeInfo.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
    bmeInfo.pressure = bme.readPressure() / 100.0F;
}
//bmp280.cpp