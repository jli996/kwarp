#include "kwarp.h"

Kwarp::BLEContext::BLEContext(std::string inDeviceName, std::string inServiceUUID, std::string inCharacteristicUUID)
: DeviceName(inDeviceName), ServiceUUID(inServiceUUID), CharacteristicUUID(inCharacteristicUUID)
{
    BLEDevice::init(DeviceName);
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);

    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new Kwarp::ServerCallbacks(this));

    BLEService *pService = pServer->createService(ServiceUUID);
    pCharacteristic = pService->createCharacteristic(
        CharacteristicUUID,
        BLECharacteristic::PROPERTY_WRITE
    );

    BLEDescriptor* pCharDesc = new BLEDescriptor(BLEUUID((uint16_t)0x2901));
    pCharDesc->setValue("Key Warp");
    pCharDesc->setAccessPermissions(ESP_GATT_PERM_READ);
    pCharacteristic->addDescriptor(pCharDesc);

    pCharacteristic->setCallbacks(new Kwarp::CharacteristicCallbacks(this));
    pCharacteristic->setAccessPermissions(ESP_GATT_PERM_WRITE_ENCRYPTED);

    pService->start();

    pSecurity = new BLESecurity();
    pin = Kwarp::generateNewPin();
    lastPinUpdateTime = millis(); 
    pSecurity->setStaticPIN(pin);

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

    pAdvertising->addServiceUUID(ServiceUUID);

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Keyboard.begin();
    USB.begin();
    DeviceState = Kwarp::EDeviceState::WaitingConnection;
}

void Kwarp::BLEContext::startAdvertising()
{
    BLEDevice::startAdvertising();
}

void Kwarp::BLEContext::tick(unsigned long deltaTime)
{
    if (getPinRemainLifetimeSec() < deltaTime)
    {
        setNewPin(generateNewPin());
    }
    if (getDeviceState() == EDeviceState::Disconnected)
    {
        delay(500);
        pServer->startAdvertising(); // restart advertising
        setDeviceState(EDeviceState::WaitingConnection);
    }
}
