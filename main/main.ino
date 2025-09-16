#include <Arduino.h>
#include "Communication/NimbleServer/NimbleServer.h"
#include "Communication/ESP-NOW/EspNowReceiver.h"
#include "RTOS/Tasks/TasksHeaders.h"
#include "RTOS/RtosUtils.h"
#include "Configs/ProgramRunConfigs.h"
#include "Configs/PinAssignments.h"
#include "ArduinoJson.h"

void setup() {
    /** Debugging setup */
    Serial.begin(115200);
    const char* TAG = "SETUP";  // Log tag
    esp_log_level_set("*", ESP_LOG_DEBUG);  // Set all log tags to debug level

    /** Components setup */
    esp_now_receiver_setup();
    ble_setup();

    // Creation status flag for all FreeRTOS kernel objects
    bool creationStatus = 1; 

    /** RTOS Task Creation */
    creationStatus &= create_and_check_task(ENABLE_T1_SEND_TO_BLUETOOTH, task_send_to_bluetooth, task1Name, configMINIMAL_STACK_SIZE*4, 1, &xTask_SendToBluetooth);

   // If any of the semaphore/mutex and queue has failed to create, exit
    if (creationStatus == 0) {
        ESP_LOGW(TAG, "Some RTOS kernel objects have failed to be created");
    }
    else {    
        // Display blinking LED to indicate start of program.
        pinMode(LED_STATUS_PIN, OUTPUT);
        digitalWrite(LED_STATUS_PIN, HIGH); 
        ESP_LOGI(TAG, "Setup successfully completed");
        vTaskDelay(pdMS_TO_TICKS(500));
        digitalWrite(LED_STATUS_PIN, LOW);
    }

}

void loop() {
    vTaskDelay(pdTICKS_TO_MS(10000));
}
