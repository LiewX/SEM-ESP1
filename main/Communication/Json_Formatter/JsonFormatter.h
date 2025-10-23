#pragma once

#include <Arduino.h>
#include "ArduinoJson.h"
#include "RTOS/RtosUtils.h"
#include "Configs/ProgramRunConfigs.h"
#include "Communication/ESP-NOW/EspNowReceiver.h"

/** Global Variables */
// Declare JSON document classes
extern JsonDocument doc1; // JSON Document for ESP1 (self)
extern JsonDocument doc2; // JSON Document for ESP2
extern JsonDocument doc3; // JSON Document for ESP3
extern JsonDocument mergedDoc;
// Mutex to protect shared resource (JSON Documents doc1, doc2, doc3)
extern SemaphoreHandle_t xSem_Msg1Guard;
extern SemaphoreHandle_t xSem_Msg2Guard;
extern SemaphoreHandle_t xSem_Msg3Guard;

// Enum for ESP identifiers
enum class EspID : uint32_t {
    ESP1 = 1,
    ESP2 = 2,
    ESP3 = 3
};

/**
 * @brief Updates corresponding JSON doc based on specified ESP target
 * @param espNumber Usage: (EspID::ESP1 / EspID::ESP2 / EspID::ESP3)
 */
void update_json_doc(EspID espNumber);

/**
 * @brief
 * @param dest
 * @param src
 */
void merge_docs(JsonDocument& dest, const JsonDocument& src);