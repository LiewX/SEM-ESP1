#pragma once
#include <Arduino.h>

/** 
 *  @brief Semaphore creation and checking function
 *  @param sem semaphore handle
 *  @param semName semaphore name
 *  @return 1 if successful; if not, return 0
 */
bool create_and_check_sem(SemaphoreHandle_t &sem, const char* semName);


/**
 *  @brief Queue creation and checking function
 *  @param queue queue handle
 *  @param queueName queue name
 *  @param size number of items in queue
 *  @param itemSize
 *  @return 1 if successful; if not, return 0
 */
bool create_and_check_queue(QueueHandle_t &queue, const char* queueName, size_t size, size_t itemSize);

/**
 *  @brief Task creation and checking function
 *  @param taskEnable macro definition
 *  @param taskFunc main function for task
 *  @param taskName 
 *  @param stackSize 
 *  @param priority
 *  @param taskHandle task handle
 *  @return 1 if successful; if not, return 0
 */
bool create_and_check_task(bool taskEnable, void (*taskFunc)(void*), const char* taskName, uint32_t stackSize, UBaseType_t priority, TaskHandle_t* taskHandle);

/**
 *  @brief Checking and printing free stack (available from allocated amount) of each task
 *  @param taskHandle 
 *  @param taskName 
 */
void print_free_stack(TaskHandle_t taskHandle, const char* taskName);