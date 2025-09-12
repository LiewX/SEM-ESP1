#include "NimbleServer.h"

// BLE global instances of the callbacks
ServerCallbacks serverCallbacks;
CharacteristicCallbacks chrCallbacks;
DescriptorCallbacks dscCallbacks;
NimBLEServer* pServer = nullptr;
NimBLECharacteristic *pCharacteristic = nullptr;

void ble_setup () {
    /** Initialize NimBLE and set the device name */
    NimBLEDevice::init("NimBLE");

    pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(&serverCallbacks);

    // Create service
    NimBLEService *pService = pServer->createService("efee514f-6606-4abf-964a-9cf32a86ffac");

    // Create characteristic (Notify enabled)
    pCharacteristic = pService->createCharacteristic(
        "27a15d46-f8ba-45ba-b474-f96222bcb945",
        // NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
    );

    // Set initial value
    pCharacteristic->setValue("Hello Client");

    /** Start the services when finished creating all Characteristics and Descriptors */
    pService->start();

    /** Create an advertising instance and add the services to the advertised data */
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName("NimBLE-Server");
    pAdvertising->addServiceUUID(pService->getUUID());
    /**
     *  If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
    pAdvertising->enableScanResponse(true);
    pAdvertising->start();

    Serial.printf("Advertising Started\n");
}

void send_bluetooth_msg(const char* msg) {
    if (pCharacteristic != nullptr && pServer->getConnectedCount() > 0) {
        pCharacteristic->setValue(msg);
        pCharacteristic->notify();
    } else {
        Serial.println("No connected client, message not sent");
    }
}