#include "JsonFormatter.h"

// Initialize JSON document classes
JsonDocument doc1; // JSON Document for ESP1 (self)
JsonDocument doc2; // JSON Document for ESP2
JsonDocument doc3; // JSON Document for ESP3
JsonDocument mergedDoc;

/**
 * @brief Updates corresponding JSON doc based on specified ESP target
 * @param espNumber
 */
void update_json_doc(EspID espNumber) {
    switch (espNumber) {
        case EspID::ESP1: {
            // Todo: get RPM value. Note: this value change can be in another function.
            // doc1["RPM"] = ;
            break;
        }
        case EspID::ESP2: {
            doc2["power"]       = receivedData2.power;
            doc2["efficiency"]  = receivedData2.efficiency;
            break;
        }
        case EspID::ESP3: {
            doc3["Pos_x"]       = receivedData3.posX;
            doc3["Pos_y"]       = receivedData3.posY;
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
