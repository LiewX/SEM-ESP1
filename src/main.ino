#include <Arduino.h>
#include "EspNowReceiver.h"
#include "NimbleServer.h"
#include "ArduinoJson.h"

// Allocate the JSON document
JsonDocument doc;

// Main task handle
TaskHandle_t mainLoopTaskHandle = nullptr;

void setup() {
    Serial.begin(115200);
    
    esp_now_receiver_setup();
    ble_setup();

    doc["time"] = 0;
    doc["power"] = 0;
    doc["speed"] = 0;

    mainLoopTaskHandle = xTaskGetCurrentTaskHandle();
}

void loop() {
    /** Loop here and send notifications to connected peers */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    doc["time"] = receivedData.timeCount;
    doc["power"] = receivedData.power;
    doc["speed"] = receivedData.speed;
    
    // Serialize into fixed buffer
    char buffer[256];
    size_t len = serializeJson(doc, buffer);
    
    // Append newline if there is still room
    // Comment this during launch
    if (len < sizeof(buffer) - 1) {
        buffer[len] = '\n';      // add newline
        buffer[len + 1] = '\0';  // null-terminate
        len++;                   // length now includes newline
    }

    if (len > 0) {
        send_bluetooth_msg(buffer);
    } else {
        Serial.println("JSON serialization failed");
    }
}
