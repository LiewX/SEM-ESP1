#include "EspNowReceiver.h"
#define RECEIVER_DEBUG 0

// Log tag
static const char* TAG = "ESP-NOW";

/** Create structs for received data from ESP-NOW */
esp_now_msg_struct_2 receivedData2; // Struct matching ESP2 msg
esp_now_msg_struct_3 receivedData3; // Struct matching ESP3 msg

/** MAC Address of target ESP32s */
uint8_t targetEsp32Mac2[6] = {0x44, 0x1D, 0x64, 0xF8, 0x2B, 0xFC};
uint8_t targetEsp32Mac3[6] = {0x44, 0x1D, 0x64, 0xF9, 0x54, 0x3C};

// callback function that will be executed when data is received
void on_data_received(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
    // Get MAC address of sender
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    
    // If MAC address matches ESP32 target 2
    if (memcmp(mac_addr, targetEsp32Mac2, 6) == 0) {
        memcpy(&receivedData2, incomingData, sizeof(receivedData2)); // Copy to msg struct
        update_json_doc(EspID::ESP2);
        // Signal, through sempahore, to "Task 1 - Send to Bluetooth" that data is ready from esp target 2
        xSemaphoreGive(xSem_Msg2Ready);
        ESP_LOGV(TAG, "Message from ESP2. Bytes received: %d", len);
    }
    
    // If MAC address matches ESP32 target 3
    else if (memcmp(mac_addr, targetEsp32Mac3, 6) == 0) {
        memcpy(&receivedData3, incomingData, sizeof(receivedData3)); // Copy to msg struct
        update_json_doc(EspID::ESP3);
        // Signal, through sempahore, to "Task 1 - Send to Bluetooth" that data is ready from esp target 3
        xSemaphoreGive(xSem_Msg3Ready);
        ESP_LOGV(TAG, "Message from ESP3. Bytes received: %d", len);
    }

    else {
        ESP_LOGI(TAG, "Unregistered message from: %s\n", macStr);
    }

}

void esp_now_receiver_setup() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        ESP_LOGW(TAG, "Error initializing ESP-NOW");
        return;
    }

    // Once ESP-Now is successfully Init, we will register for receiver callback to
    // get receiver packer info
    esp_now_register_recv_cb(on_data_received);
}