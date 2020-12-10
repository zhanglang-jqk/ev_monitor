/***********************************************************************
 * @file 	:	BLE_server.cpp
 * @author	:	ch
 * @brief	:	
 * @version:	v1.0
 * @Copyright (C)  2020-12-10  .cdWFVCEL. all right reserved
***********************************************************************/

/* 包含头文件 -------------------------------------------------------------------*/
#include <Arduino.h>
#include "BLE_server.h"
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ArduinoJson.h>

/* 宏定义 -----------------------------------------------------------------------*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define CHARACTERISTIC_NUM SMART_BUCKLE_NUM //设置得过大，可能会导致 client 端连接创建时堆栈溢出

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID1 "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define CHARACTERISTIC_UUID2 "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define CHARACTERISTIC_UUID3 "beb5483e-36e1-4688-b7f5-ea07361b26a3"
#define CHARACTERISTIC_UUID4 "beb5483e-36e1-4688-b7f5-ea07361b26a4"
#define CHARACTERISTIC_UUID5 "beb5483e-36e1-4688-b7f5-ea07361b26a5"
#define CHARACTERISTIC_UUID6 "beb5483e-36e1-4688-b7f5-ea07361b26a6"
/* 类型定义 ---------------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------------------*/
BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristics[CHARACTERISTIC_NUM] = {NULL};
char *characteristics_uuid[6] = {CHARACTERISTIC_UUID1, CHARACTERISTIC_UUID2, CHARACTERISTIC_UUID3, CHARACTERISTIC_UUID4, CHARACTERISTIC_UUID5, CHARACTERISTIC_UUID6};

StaticJsonDocument<200> doc;
/* 扩展变量 ---------------------------------------------------------------------*/
/* 私有函数声明 -----------------------------------------------------------------*/
/* 函数声明 ---------------------------------------------------------------------*/
/* 函数实现 ---------------------------------------------------------------------*/

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        BLEDevice::startAdvertising();
        Serial2.println("onConnect");
    };

    void onDisconnect(BLEServer *pServer)
    {
        Serial2.println("onDisconnect");
    }
};

class MyBLECharacteristicCallbacks : public BLECharacteristicCallbacks
{
    void onRead(BLECharacteristic *pCharacteristic)
    {
        Serial2.printf("characteristic onread %s \r\n", pCharacteristic->getValue().c_str());
    }
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        char *tmp = (char *)pCharacteristic->getValue().c_str();
        char json[64] = {0};
        strcpy(json, tmp);
        Serial2.printf("characteristic onwrite %s \r\n", json);

        // char json[] = "{\"temp\":11.2,\"ABPM\":135,\"finger\":1}";
        // Serial2.printf("characteristic onwrite %s \r\n", json);

        for (int i = 0; i < CHARACTERISTIC_NUM; i++)
        {
            if (std::string(characteristics_uuid[i]) == pCharacteristic->getUUID().toString())
            {
                //..
                Serial2.printf("%s is refresh \r\n", characteristics_uuid[i]);

                // Deserialize the JSON document
                DeserializationError error = deserializeJson(doc, json);

                // Test if parsing succeeds.
                if (error)
                {
                    Serial2.print(F("deserializeJson() failed: "));
                    Serial2.println(error.f_str());
                }
                else
                {
                    // {'temp':22,'IR':6452,'BPM':0,'ABPM':0,'finger':0}
                    smartBuckles[i].temperature = doc["temp"];
                    smartBuckles[i].heartbeat = doc["ABPM"];
                    smartBuckles[i].finger_f = doc["finger"];
                    smartBuckles[i].isNew = true;
                }
            }
        }
    }
};

void BS_Init()
{
    BLEDevice::init("Long name works now");
    pServer = BLEDevice::createServer();
    pService = pServer->createService(SERVICE_UUID);

    pServer->setCallbacks(new MyServerCallbacks());

    for (int i = 0; i < CHARACTERISTIC_NUM; i++)
    {
        // Create a BLE Characteristic
        pCharacteristics[i] = pService->createCharacteristic(characteristics_uuid[i], BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
        pCharacteristics[i]->setCallbacks(new MyBLECharacteristicCallbacks());
    }

    // pCharacteristic->setValue("Hello World says Neil");
    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial2.println("Characteristic defined! Now you can read it in your phone!");
}
//BLE_server.cpp