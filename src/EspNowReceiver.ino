/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include "EspNowReceiver.h"
#define RECEIVER_DEBUG 0

// Create a esp_now_struct_received called receivedData
esp_now_struct_received receivedData;

extern TaskHandle_t mainLoopTaskHandle;
// callback function that will be executed when data is received
void on_data_received(const uint8_t *mac_addr, const uint8_t *incomingData, int len) {
    memcpy(&receivedData, incomingData, sizeof(receivedData));

    #if RECEIVER_DEBUG
    // Print MAC address of sender
    char macStr[18];
    snprintf(macStr, sizeof(macStr),
             "%02X:%02X:%02X:%02X:%02X:%02X",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

    Serial.print("Message from: ");
    Serial.println(macStr);
    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("Count: ");
    Serial.println(receivedData.timeCount);
    Serial.print("Power: ");
    Serial.println(receivedData.power);
    Serial.print("Speed: ");
    Serial.println(receivedData.speed);
    #endif

    xTaskNotifyGive(mainLoopTaskHandle);
}

// @brief Use this if you're using Arduino IDE. Rmb to change header file as well. Previous function may work with Arduino IDE
// void on_data_received(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
//     memcpy(&receivedData, incomingData, sizeof(receivedData));

//     #if TRANSMIT_DEBUG
//     // Print MAC address of sender
//     char macStr[18];
//     snprintf(macStr, sizeof(macStr),
//     "%02X:%02X:%02X:%02X:%02X:%02X",
//     info->src_addr[0], info->src_addr[1], info->src_addr[2],
//     info->src_addr[3], info->src_addr[4], info->src_addr[5]);
    
//     Serial.print("Message from: ");
//     Serial.println(macStr);
//     Serial.print("Bytes received: ");
//     Serial.println(len);
//     Serial.print("Char: ");
//     Serial.println(receivedData.a);
//     Serial.print("Int: ");
//     Serial.println(receivedData.b);
//     Serial.print("Float: ");
//     Serial.println(receivedData.c);
//     Serial.print("Bool: ");
//     Serial.println(receivedData.d);
//     Serial.println();
//     #endif
// }

void esp_now_receiver_setup() {
    // Set device as a Wi-Fi Station
    WiFi.mode(WIFI_STA);

    // Init ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Once ESPNow is successfully Init, we will register for receiver callback to
    // get receiver packer info
    esp_now_register_recv_cb(on_data_received);
}