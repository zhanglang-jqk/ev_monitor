/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

/* define ---------------------------------------------------------------------------------------------------------------*/
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define CHARACTERISTIC_NUM 3 //设置得过大，可能会导致 client 端连接创建时堆栈溢出
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID1 "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define CHARACTERISTIC_UUID2 "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define CHARACTERISTIC_UUID3 "beb5483e-36e1-4688-b7f5-ea07361b26a3"
#define CHARACTERISTIC_UUID4 "beb5483e-36e1-4688-b7f5-ea07361b26a4"
#define CHARACTERISTIC_UUID5 "beb5483e-36e1-4688-b7f5-ea07361b26a5"
#define CHARACTERISTIC_UUID6 "beb5483e-36e1-4688-b7f5-ea07361b26a6"

#define LED 33

/* variable ---------------------------------------------------------------------------------------------------------------*/
BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristics[CHARACTERISTIC_NUM] = {NULL};
char *characteristics_uuid[6] = {CHARACTERISTIC_UUID1, CHARACTERISTIC_UUID2, CHARACTERISTIC_UUID3, CHARACTERISTIC_UUID4, CHARACTERISTIC_UUID5, CHARACTERISTIC_UUID6};

/* function ---------------------------------------------------------------------------------------------------------------*/
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
    Serial2.printf("characteristic onwrite %s \r\n", pCharacteristic->getValue().c_str());
  }
};

// EPD1in54B epd(26, 25, 5, 27); // reset, dc, cs, busy

void setup()
{
  pinMode(LED, OUTPUT);

  Serial2.begin(115200);
  Serial2.println("Starting BLE work!");

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

void loop()
{
  // put your main code here, to run repeatedly:

  // for (int i = 0; i < CHARACTERISTIC_NUM; i++)
  // {
  //   std::string str = pCharacteristics[i]->getValue();
    
  // }
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
}