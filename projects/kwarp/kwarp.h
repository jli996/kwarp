#pragma once

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#if ARDUINO_USB_MODE
#error This program should be used when USB is in OTG mode
#endif

#include "USB.h"
#include "USBHIDKeyboard.h"

namespace Kwarp {

static USBHIDKeyboard Keyboard;

enum class EDeviceState : uint8_t
{
    Initalizing,
    WaitingConnection,
    Ready,
    Disconnected,
    Error
}; 
constexpr int MaxPinLifeTime = 5 * 60 * 1000;

class ServerCallbacks;
class CharacteristicCallbacks;

class BLEContext {

public:
    BLEContext(std::string inDeviceName,
     std::string inServiceUUID,
      std::string inCharacteristicUUID);
    EDeviceState getDeviceState() const { return DeviceState; };
    std::string getDeviceName() const { return DeviceName; };
    void setNewPin(uint32_t inPin) { pin = inPin; pSecurity->setStaticPIN(inPin); lastPinUpdateTime = millis(); };
    uint32_t getCurrentPin() const {return pin;}
    unsigned long getPinRemainLifetimeSec() { return (maxPinLifeTime - (millis() - lastPinUpdateTime))/1000;  };
    void setDeviceState(EDeviceState inDeviceState) { DeviceState = inDeviceState; };
    void startAdvertising();
    void tick(unsigned long deltaTime);
private:
    std::string DeviceName;
    std::string ServiceUUID;
    std::string CharacteristicUUID;

    EDeviceState DeviceState = EDeviceState::Initalizing;
    BLEServer *pServer = nullptr;
    BLECharacteristic *pCharacteristic = nullptr;
    BLESecurity *pSecurity = nullptr;
    bool deviceConnected = false;
    bool oldDeviceConnected = false;
    uint32_t pin = 123456;
    unsigned long maxPinLifeTime = MaxPinLifeTime;
    unsigned long lastPinUpdateTime = 0l;
};

class ServerCallbacks : public BLEServerCallbacks {
public:
    ServerCallbacks(BLEContext* inContext): pContext(inContext){};

    void onConnect(BLEServer *pServer) override {
        pContext->setDeviceState(EDeviceState::Ready);
    };

    void onDisconnect(BLEServer *pServer) override {
        pContext->setDeviceState(EDeviceState::Disconnected);
    }
private:
    BLEContext* pContext = nullptr;
};


class CharacteristicCallbacks : public BLECharacteristicCallbacks {
public:
    CharacteristicCallbacks(BLEContext* inContext): pContext(inContext) {};
    void onWrite(BLECharacteristic *pCharacteristic) override {
        const std::string value = pCharacteristic->getValue();
        Serial.printf("value rec: %s", value.c_str());
        Serial.println(" ");
        if (!value.empty()) {
            for (int i = 0; i < value.length(); i++) {
                Keyboard.write(value[i]);
            }
        }
    }
private:
    BLEContext* pContext = nullptr;

};

uint32_t static generateNewPin() {
    return static_cast<uint32_t>(random(100000, 999999));
}



} // End Kwarp Namespace

