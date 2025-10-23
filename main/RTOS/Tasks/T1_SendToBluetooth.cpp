#include "T1_SendToBluetooth.h"

// "Send to Bluetooth"
const char* task1Name  = "Task - Send to Bluetooth";

// Log Tag
static const char* TAG = "TASK - SEND TO BLUETOOTH";

// Task Handle
TaskHandle_t xTask_SendToBluetooth = nullptr;

/** Semaphore Handle */
// Binary Semaphores to signal when data is ready from ESP1, ESP2, and ESP3
SemaphoreHandle_t xSem_Msg1Ready = nullptr;
SemaphoreHandle_t xSem_Msg2Ready = nullptr;
SemaphoreHandle_t xSem_Msg3Ready = nullptr;

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

        // If any data is ready, merge and send
        if (status1 == pdTRUE || status2 == pdTRUE || status3 == pdTRUE) {
            mergedDoc.clear();  // Clear previous values

            if (status1 == pdTRUE) {
                xSemaphoreTake(xSem_Msg1Guard, portMAX_DELAY);
                mergedDoc.set(doc1);  // overwrite mergedDoc with doc1
                xSemaphoreGive(xSem_Msg1Guard);
            }
            if (status2 == pdTRUE) {
                xSemaphoreTake(xSem_Msg2Guard, portMAX_DELAY);
                merge_docs(mergedDoc, doc2);  // Insert msg 2 to merged json doc
                xSemaphoreGive(xSem_Msg2Guard);
            }
            if (status3 == pdTRUE) {
                xSemaphoreTake(xSem_Msg3Guard, portMAX_DELAY);
                merge_docs(mergedDoc, doc3);  // Insert msg 3 to merged json doc
                xSemaphoreGive(xSem_Msg3Guard);
            }
            
            /** Serialize message  */
            size_t len = serializeJson(mergedDoc, buffer);
            
            // Append newline if there is still room
            // Comment this during launch
            if (len < sizeof(buffer) - 1) {
                buffer[len] = '\n';      // add newline
                buffer[len + 1] = '\0';  // null-terminate
                len++;                   // length now includes newline
            }

            /** Send to bluetooth */
            if (len > 0) {
                send_bluetooth_msg(buffer);
            } else {
                ESP_LOGW(TAG, "JSON serialization failed");
            }

            // Delay until the next execution time
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
        
        /** No data ready, just delay until next cycle */
        else {
            vTaskDelayUntil(&xLastWakeTime, xFrequency);
            continue;
        }

        
    }
}