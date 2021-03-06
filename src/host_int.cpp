/***********************************************************************
 * @file 	:	computer_int.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-12  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include "host_int.h"
#include "main.h"
#include "bme280.h"
#include "m702.h"
#include "ArduinoJson.h"
/* 宏定义 -----------------------------------------------------------------------*/
/* 类型定义 ---------------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------------------*/
StaticJsonDocument<1024> root;
StaticJsonDocument<1024> payload;
// JsonObject payload = root.createNestedObject("payload");
JsonArray smartBuckleJsonArray = payload.createNestedArray("smartBuckles");
StaticJsonDocument<200> smartBucklesDoc[SMART_BUCKLE_NUM];
JsonObject bme280Obj = payload.createNestedObject("bme280");
JsonObject m702Obj = payload.createNestedObject("m702");
// JsonInteger checksumInt = root.createNestedObject("checksum");
/* 扩展变量 ---------------------------------------------------------------------*/
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/
void CI_Init()
{
    CI_SERIAL.begin(9600);

    for (int i = 0; i < SMART_BUCKLE_NUM; i++)
    {
        smartBucklesDoc[i]["temperature"] = smartBuckles[i].temperature;
        smartBucklesDoc[i]["heartbeat"] = smartBuckles[i].heartbeat;
        smartBucklesDoc[i]["finger_f"] = smartBuckles[i].finger_f;

        smartBuckleJsonArray.add(smartBucklesDoc[i]);
    }
    payload.add(smartBuckleJsonArray);

    bme280Obj["temperature"] = bmeInfo.temperature;
    bme280Obj["humidity"] = bmeInfo.humidity;
    bme280Obj["altitude"] = bmeInfo.altitude;
    bme280Obj["pressure"] = bmeInfo.pressure;
    payload.add(bme280Obj);

    m702Obj["CO2"] = m702.CO2;
    m702Obj["CH2O"] = m702.CH2O;
    m702Obj["TVOC"] = m702.TVOC;
    m702Obj["PM25"] = m702.PM25;
    m702Obj["PM10"] = m702.PM10;
    m702Obj["temperature"] = m702.temperature;
    m702Obj["humidity"] = m702.humidity;
    payload.add(m702Obj);

    root["payload"] = payload;
    // root.add(payload);
}
void CI_Scan()
{
    for (int i = 0; i < SMART_BUCKLE_NUM; i++)
    {
        root["payload"]["smartBuckles"][i]["temperature"] = smartBuckles[i].temperature;
        root["payload"]["smartBuckles"][i]["heartbeat"] = smartBuckles[i].heartbeat;
        root["payload"]["smartBuckles"][i]["finger_f"] = smartBuckles[i].finger_f;
    }

    root["payload"]["bme280"]["temperature"] = bmeInfo.temperature;
    root["payload"]["bme280"]["humidity"] = bmeInfo.humidity;
    root["payload"]["bme280"]["altitude"] = bmeInfo.altitude;
    root["payload"]["bme280"]["pressure"] = bmeInfo.pressure;

    root["payload"]["m702"]["CO2"] = m702.CO2;
    root["payload"]["m702"]["CH2O"] = m702.CH2O;
    root["payload"]["m702"]["TVOC"] = m702.TVOC;
    root["payload"]["m702"]["PM25"] = m702.PM25;
    root["payload"]["m702"]["PM10"] = m702.PM10;
    root["payload"]["m702"]["temperature"] = m702.temperature;
    root["payload"]["m702"]["humidity"] = m702.humidity;
}

char jsonBuf[1024] = {0};
void CI_Transimit()
{
    serializeJson(root["payload"], jsonBuf, sizeof(jsonBuf));
    u16 checksum = 0;
    for (int i = 0; i < strlen(jsonBuf); i++)
    {
        checksum += jsonBuf[i];
    }

    root["checksum"] = checksum;
    memset(jsonBuf, 0, sizeof(jsonBuf));

    serializeJson(root, jsonBuf, sizeof(jsonBuf));
    CI_SERIAL.println(jsonBuf);
    Serial2.println(jsonBuf);

    // serializeJsonPretty(root,Serial2);
    // serializeJsonPretty(payload,Serial2);
}
//computer_int.cpp