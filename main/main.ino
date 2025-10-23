#include <Arduino.h>
#include "Communication/NimbleServer/NimbleServer.h"
#include "Communication/ESP-NOW/EspNowReceiver.h"
#include "RTOS/Tasks/TasksHeaders.h"
#include "RTOS/RtosUtils.h"
#include "Configs/ProgramRunConfigs.h"
#include "Configs/PinAssignments.h"
#include "ArduinoJson.h"
#include "esp_log.h"

void setup() {
    /** Debugging setup */
    Serial.begin(115200);
    const char* TAG = "SETUP";  // Log tag
    ESP_LOGI(TAG, "Starting setup...");

    // Creation status flag for all FreeRTOS kernel objects
    bool creationStatus = 1; 
    
    /** RTOS Kernel semaphores creation */
    // Binary Semaphores to signal when data is ready from ESP1, ESP2, and ESP3
    creationStatus &= create_and_check_binary_sem(xSem_Msg1Ready, "Semaphore - Msg 1 Ready");
    creationStatus &= create_and_check_binary_sem(xSem_Msg2Ready, "Semaphore - Msg 2 Ready");
    creationStatus &= create_and_check_binary_sem(xSem_Msg3Ready, "Semaphore - Msg 3 Ready");
    // Mutex to protect shared resource (JSON Documents doc1, doc2, doc3)
    creationStatus &= create_and_check_sem(xSem_Msg1Guard, "Semaphore - Msg 1 Guard");
    creationStatus &= create_and_check_sem(xSem_Msg2Guard, "Semaphore - Msg 2 Guard");
    creationStatus &= create_and_check_sem(xSem_Msg3Guard, "Semaphore - Msg 3 Guard");
    
    /** Components setup */
    ble_setup();
    esp_now_receiver_setup();

    /** RTOS Task Creation */
    creationStatus &= create_and_check_task(ENABLE_T1_SEND_TO_BLUETOOTH, task_send_to_bluetooth, task1Name, configMINIMAL_STACK_SIZE*4, 1, &xTask_SendToBluetooth);

   // If any of the semaphore/mutex and queue has failed to create, exit
    if (creationStatus == 0) {
        ESP_LOGW(TAG, "Some RTOS kernel objects have failed to be created");
    }
    else {    
        ESP_LOGI(TAG, "Setup successfully completed");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

}

void loop() {
    // Nothing to do here - everything is handled in tasks defined in other files.
    vTaskDelay(pdMS_TO_TICKS(10000));
}
