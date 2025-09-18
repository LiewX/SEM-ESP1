#include <Arduino.h>
#include "RtosUtils.h"

// Log tag
static const char* TAG = "RTOS";

bool create_and_check_sem(SemaphoreHandle_t &sem, const char* semName) {
    sem = xSemaphoreCreateMutex();
    if (sem == NULL) {
        ESP_LOGW(TAG, "Failed to create semaphore '%s'", semName);
        return false;
    }
    ESP_LOGI(TAG, "'%s' created successfully", semName);
    return true;
}

bool create_and_check_binary_sem(SemaphoreHandle_t &sem, const char* semName) {
    sem = xSemaphoreCreateBinary();
    if (sem == NULL) {
        ESP_LOGW(TAG, "Failed to create binary semaphore '%s'", semName);
        return false;
    }
    ESP_LOGI(TAG, "'%s' created successfully", semName);
    return true;
}

bool create_and_check_queue(QueueHandle_t &queue, const char* queueName, size_t size, size_t itemSize) {
    queue = xQueueCreate(size, itemSize);
    if (queue == NULL) {
        ESP_LOGW(TAG, "Failed to create queue '%s'", queueName);
        return false;
    }
    ESP_LOGI(TAG, "'%s' created successfully", queueName);
    return true;
}

bool create_and_check_task(bool taskEnable, void (*taskFunc)(void*), const char* taskName, uint32_t stackSize, UBaseType_t priority, TaskHandle_t* taskHandle) {
    if (taskEnable) {
        BaseType_t status = xTaskCreate(taskFunc, taskName, stackSize, NULL, priority, taskHandle);
        if (status != pdPASS) {
            ESP_LOGW(TAG, "Failed to create task '%s'", taskName);
            return false;
        }
        ESP_LOGI(TAG, "'%s' created successfully", taskName);
    }
    return true;
}

void print_free_stack(TaskHandle_t taskHandle, const char* taskName) {
    if (taskHandle != nullptr) {
        UBaseType_t highWaterMark = uxTaskGetStackHighWaterMark(taskHandle);
        ESP_LOGI("MEMORY", "Stack High Watermark for %s: %u", taskName, (unsigned int) highWaterMark);
    }
}