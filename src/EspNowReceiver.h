#pragma once

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct esp_now_struct_received {
    uint32_t timeCount;
    float power;
    float speed;
} esp_now_struct_received;

extern esp_now_struct_received receivedData;

void on_data_received(const uint8_t *mac_addr, const uint8_t *incomingData, int len);
// void on_data_received(const esp_now_recv_info *info, const uint8_t *incomingData, int len);

void esp_now_receiver_setup();