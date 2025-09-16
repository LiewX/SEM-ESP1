#pragma once
#include <Arduino.h>
#include <Arduino.h>
#include "../Timing.h"
#include "Communication/ESP-NOW/EspNowReceiver.h"
#include "Communication/Json_Formatter/JsonFormatter.h"
#include "Communication/NimbleServer/NimbleServer.h"

extern const char* task1Name;

// Global Variable
extern SemaphoreHandle_t xSem_Msg2Ready;
extern SemaphoreHandle_t xSem_Msg3Ready;

// Task Handle
extern TaskHandle_t xTask_SendToBluetooth;

// Function prototypes for task
void task_send_to_bluetooth(void *pvParameters);