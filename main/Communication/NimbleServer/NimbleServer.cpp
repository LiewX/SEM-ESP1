#include "NimbleServer.h"

// Log tag
static const char* TAG = "NIMBLE SERVER";

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
        "F00D",
        // NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
    );

    // Set initial value
    pCharacteristic->setValue("Hello Client");

    /** Start the services when finished creating all Characteristics and Descriptors */
    pService->start();

    /** Create an advertising instance and add the services to the advertised data */
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->setName("SEM");
    pAdvertising->addServiceUUID(pService->getUUID());
    /**
     *  If your device is battery powered you may consider setting scan response
     *  to false as it will extend battery life at the expense of less data sent.
     */
    pAdvertising->enableScanResponse(true);
    pAdvertising->start();

    ESP_LOGI(TAG, "Advertising Started");
}

void send_bluetooth_msg(const char* msg) {
    if (pCharacteristic != nullptr && pServer->getConnectedCount() > 0) {
        pCharacteristic->setValue(msg);
        pCharacteristic->notify();
    } else {
        ESP_LOGW(TAG, "No connected client, message not sent");
    }
}



/**  None of these are required as they will be handled by the library with defaults. **
 **                       Remove as you see fit for your needs                        */
void ServerCallbacks::onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) {
    ESP_LOGI(TAG, "Client address: %s", connInfo.getAddress().toString().c_str());
    /**
     *  We can use the connection handle here to ask for different connection parameters.
     *  Args: connection handle, min connection interval, max connection interval
     *  latency, supervision timeout.
     *  Units; Min/Max Intervals: 1.25 millisecond increments.
     *  Latency: number of intervals allowed to skip.
     *  Timeout: 10 millisecond increments.
     */
    pServer->updateConnParams(connInfo.getConnHandle(), 24, 48, 0, 180);
}

void ServerCallbacks::onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) {
    ESP_LOGI(TAG, "Client disconnected - start advertising");
    NimBLEDevice::startAdvertising();
}

void ServerCallbacks::onMTUChange(uint16_t MTU, NimBLEConnInfo& connInfo) {
    ESP_LOGV(TAG, "MTU updated: %u for connection ID: %u", MTU, connInfo.getConnHandle());
}

/********************* Security handled here *********************/
uint32_t ServerCallbacks::onPassKeyDisplay() {
    ESP_LOGW(TAG, "Server Passkey Display");
    /**
     * This should return a random 6 digit number for security
     *  or make your own static passkey as done here.
     */
    return 123456;
}

void ServerCallbacks::onConfirmPassKey(NimBLEConnInfo& connInfo, uint32_t pass_key) {
    ESP_LOGW(TAG, "The passkey YES/NO number: %" PRIu32 "", pass_key);
    /** Inject false if passkeys don't match. */
    NimBLEDevice::injectConfirmPasskey(connInfo, true);
}

void ServerCallbacks::onAuthenticationComplete(NimBLEConnInfo& connInfo) {
    /** Check that encryption was successful, if not we disconnect the client */
    if (!connInfo.isEncrypted()) {
        NimBLEDevice::getServer()->disconnect(connInfo.getConnHandle());
        ESP_LOGW(TAG, "Encrypt connection failed - disconnecting client");
        return;
    }
    ESP_LOGI(TAG, "Secured connection to: %s", connInfo.getAddress().toString().c_str());
}


/** Handler class for characteristic actions */
void CharacteristicCallbacks::onRead(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    ESP_LOGV(TAG, "%s : onRead(), value: %s",
             pCharacteristic->getUUID().toString().c_str(),
             pCharacteristic->getValue().c_str());
}

void CharacteristicCallbacks::onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) {
    ESP_LOGV(TAG, "%s : onWrite(), value: %s",
             pCharacteristic->getUUID().toString().c_str(),
             pCharacteristic->getValue().c_str());
}

/**
 *  The value returned in code is the NimBLE host return code.
 */
void CharacteristicCallbacks::onStatus(NimBLECharacteristic* pCharacteristic, int code) {
    ESP_LOGV(TAG, "Notification/Indication return code: %d, %s", code, NimBLEUtils::returnCodeToString(code));
}

/** Peer subscribed to notifications/indications */
void CharacteristicCallbacks::onSubscribe(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo, uint16_t subValue) {
    std::string str  = "Client ID: ";
    str             += connInfo.getConnHandle();
    str             += " Address: ";
    str             += connInfo.getAddress().toString();
    if (subValue == 0) {
        str += " Unsubscribed to ";
    } else if (subValue == 1) {
        str += " Subscribed to notifications for ";
    } else if (subValue == 2) {
        str += " Subscribed to indications for ";
    } else if (subValue == 3) {
        str += " Subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    ESP_LOGV(TAG, "%s", str.c_str());
}


/** Handler class for descriptor actions */
void DescriptorCallbacks::onWrite(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo) {
    std::string dscVal = pDescriptor->getValue();
    ESP_LOGV(TAG, "Descriptor written value: %s", dscVal.c_str());
}

void DescriptorCallbacks::onRead(NimBLEDescriptor* pDescriptor, NimBLEConnInfo& connInfo) {
    ESP_LOGV(TAG, "%s Descriptor read", pDescriptor->getUUID().toString().c_str());
}