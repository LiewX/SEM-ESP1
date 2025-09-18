#pragma once

#include <esp_now.h>
#include <WiFi.h>
#include <esp_log.h>
#include "RTOS/Tasks/T1_SendToBluetooth.h"
#include "Communication/Json_Formatter/JsonFormatter.h"

// Structure on receiving data from ESP32 No. 2. Must match transmitter msg struct
typedef struct esp_now_msg_struct_2 {
    float power;
    float efficiency;
    float batteryCapacity;
} esp_now_msg_struct_2;

// Structure on receiving data from ESP32 No. 3. Must match transmitter msg struct
typedef struct esp_now_msg_struct_3 {
    float posX;
    float posY;
} esp_now_msg_struct_3;

extern esp_now_msg_struct_2 receivedData2;
extern esp_now_msg_struct_3 receivedData3;

void on_data_received(const uint8_t *mac_addr, const uint8_t *incomingData, int len);

void esp_now_receiver_setup();