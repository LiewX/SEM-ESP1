#include "JsonFormatter.h"

// Initialize JSON document classes
JsonDocument doc1; // JSON Document for ESP1 (self)
JsonDocument doc2; // JSON Document for ESP2
JsonDocument doc3; // JSON Document for ESP3
JsonDocument mergedDoc;

// Mutex to protect shared resource (JSON Documents doc1, doc2, doc3)
SemaphoreHandle_t xSem_Msg1Guard = nullptr;
SemaphoreHandle_t xSem_Msg2Guard = nullptr;
SemaphoreHandle_t xSem_Msg3Guard = nullptr;

void update_json_doc(EspID espNumber) {
    switch (espNumber) {
        case EspID::ESP1: {
            // Todo: get sensor values etc. Note: this value change can be in another function.
            // xSemaphoreTake(xSem_Msg1Guard, portMAX_DELAY);
            // doc1["RPM"] = ;
            // doc1["motorTemp"] = ;
            // doc1["fanPowerPercent"] = ;
            // xSemaphoreGive(xSem_Msg1Guard);
            break;
        }
        case EspID::ESP2: {
            xSemaphoreTake(xSem_Msg2Guard, portMAX_DELAY);
            // Update JSON document with received ESP-NOW data from ESP2
            doc2["power"]           = receivedData2.power;
            doc2["efficiency"]      = receivedData2.efficiency;
            doc2["batteryCapacity"] = receivedData2.batteryCapacity;
            xSemaphoreGive(xSem_Msg2Guard);
            break;
        }
        case EspID::ESP3: {
            xSemaphoreTake(xSem_Msg3Guard, portMAX_DELAY);
            // Update JSON document with received ESP-NOW data from ESP3
            doc3["pos_x"]   = receivedData3.posX;
            doc3["pos_y"]   = receivedData3.posY;
            xSemaphoreGive(xSem_Msg3Guard);
            break;
        }
        default:
            // Wrong ESP number provided
            ESP_LOGW("JSON FORMATTING", "Wrong ESP Number selected to format JSON.");
            break;
    }

}

void merge_docs(JsonDocument& dest, const JsonDocument& src) {
    JsonObjectConst srcObj = src.as<JsonObjectConst>();
    for (JsonPairConst kv : srcObj) {
        dest[kv.key()] = kv.value();
    }
}
