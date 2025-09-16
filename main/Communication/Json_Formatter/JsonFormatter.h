#pragma once

#include <Arduino.h>
#include "ArduinoJson.h"
#include "RTOS/RtosUtils.h"
#include "Configs/ProgramRunConfigs.h"
#include "Communication/ESP-NOW/EspNowReceiver.h"

// Declare JSON document classes
extern JsonDocument doc1; // JSON Document for ESP1 (self)
extern JsonDocument doc2; // JSON Document for ESP2
extern JsonDocument doc3; // JSON Document for ESP3
extern JsonDocument mergedDoc;

// Enum for ESP identifiers
enum class EspID : uint32_t {
    ESP1 = 1,
    ESP2 = 2,
    ESP3 = 3
};

void update_json_doc(EspID espNumber);

// /**
//  * @brief 
//  * @param espNumber
//  * @param args
//  */
// // Variadic template function to update JSON docs with variable number of arguments
// template<typename... Args>
// void update_json_doc(EspID espNumber, Args... args) {
//     // Pack all arguments into a tuple for indexed access
//     auto values = std::tuple<Args...>(args...);

//     switch (espNumber) {
//         case EspID::ESP1: {
//             doc1["time"]  = std::get<0>(values);
//             doc1["power"] = std::get<1>(values);
//             doc1["speed"] = std::get<2>(values);
//             break;
//         }

//         case EspID::ESP2: {
//             doc2["time"]  = std::get<0>(values);
//             doc2["temp"]  = std::get<1>(values);
//             break;
//         }

//         case EspID::ESP3: {
//             doc3["time"]   = std::get<0>(values);
//             doc3["power"]  = std::get<1>(values);
//             doc3["speed"]  = std::get<2>(values);
//             break;
//         }

//         default:
//             // Wrong ESP number provided
//             ESP_LOGW("JSON FORMATTING", "Wrong ESP Number selected to format JSON.");
//             break;
//     }
// }

/**
 * @brief
 * @param dest
 * @param src
 */
void merge_docs(JsonDocument& dest, const JsonDocument& src);