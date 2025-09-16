#include "T1_SendToBluetooth.h"

// "Send to Bluetooth"
const char* task1Name  = "Task - Send to Bluetooth";

// Log Tag
static const char* TAG = "TASK - SEND TO BLUETOOTH";

// Task Handle
TaskHandle_t xTask_SendToBluetooth = nullptr;

// Semaphore Handle
SemaphoreHandle_t xSem_Msg1Ready = nullptr;
SemaphoreHandle_t xSem_Msg2Ready = nullptr;
SemaphoreHandle_t xSem_Msg3Ready = nullptr;

// Task Definition
void task_send_to_bluetooth(void *pvParameters) {
    const TickType_t xFrequency = pdMS_TO_TICKS(SEND_TO_BLUETOOTH_PERIOD); // Set task running frequency
    char buffer[256];
    BaseType_t status1 = pdFALSE;
    BaseType_t status2 = pdFALSE;
    BaseType_t status3 = pdFALSE;
    TickType_t xLastWakeTime = xTaskGetTickCount();   // Initialize last wake time
    
    for (;;) {
        /** Check if data is ready from ESP 1, 2, and 3 */
        status1 = xSemaphoreTake(xSem_Msg1Ready, 0);
        status2 = xSemaphoreTake(xSem_Msg2Ready, 0);
        status3 = xSemaphoreTake(xSem_Msg3Ready, 0);

        mergedDoc.clear();  // Clear previous values

        if (status1 == pdTRUE) {
            // Insert msg 1 to merged json doc
            mergedDoc.set(doc1);  // overwrite mergedDoc
        }
        if (status2 == pdTRUE) {
            // Insert msg 2 to merged json doc
            merge_docs(mergedDoc, doc2);
        }
        if (status3 == pdTRUE) {
            // Insert msg 3 to merged json doc
            merge_docs(mergedDoc, doc3);            
        }
        
        /** Serialize message  */
        size_t len = serializeJson(mergedDoc, buffer);
        
        // Append newline if there is still room
        // Comment this during launch
        // if (len < sizeof(buffer) - 1) {
        //     buffer[len] = '\n';      // add newline
        //     buffer[len + 1] = '\0';  // null-terminate
        //     len++;                   // length now includes newline
        // }

        /** Send to bluetooth */
        if (len > 0) {
            send_bluetooth_msg(buffer);
        } else {
            ESP_LOGW(TAG, "JSON serialization failed");
        }

        // Delay until the next execution time
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}