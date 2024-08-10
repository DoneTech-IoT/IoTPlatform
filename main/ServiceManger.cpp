#include "ServiceManger.h"
#include <stdio.h>
#include"string.h"
static const char *TAG = "Service_Manger";
#define TASK_LIST_BUFFER_SIZE 512
// #define MONITORING
/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter);

// Static variables for Service Manager task
StaticTask_t *xTaskServiceMangerBuffer;
StackType_t *xServiceMangerStack;

// Global instance of Service Manager
ServiceManger_t ServiceManger;

/**
 * @brief Deletes a task.
 * This function deletes the specified task.
 * @param TaskNumber The number of the task to be deleted.
 * @return void
 */
void TaskKiller(int TaskNumber)
{
    ServiceManger.tasks[TaskNumber].TaskKiller(&ServiceManger.tasks[TaskNumber].taskHandler);
    ESP_LOGI(TAG, "Task %d Deleted !", TaskNumber);
}

/**
 * @brief Creates the Spotify task.
 * This function configures and creates the Spotify task.
 * @return void
 */
void SpotifyTaskCreator()
{
    ESP_LOGI(TAG, "TO DO");
}

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceMangerTaskInit()
{
    xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceMangerTask,         // Task function
        "ServiceMangerTask",       // Task name (for debugging)
        SERVICE_MANGER_STACK,      // Stack size (in words)
        NULL,                      // Task parameters (passed to the task function)
        tskIDLE_PRIORITY + 1,      // Task priority (adjust as needed)
        xServiceMangerStack,       // Stack buffer
        xTaskServiceMangerBuffer); // Task control block
}

/**
 * @brief Initializes the Service Manager.
 * This function initializes the Service Manager by creating necessary tasks.
 * @return void
 */
void ServiceMangerInit()
{
#ifdef CONFIG_DONE_COMPONENT_SPOTIFY
    SpotifyTaskCreator();
    ESP_LOGI(TAG, "Spotify Created !");
#endif
}

/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter)
{
    ServiceMangerInit();
    char pcTaskList[TASK_LIST_BUFFER_SIZE];
    while (true)
    {
        // vTaskList(pcTaskList);
#ifdef MONITORING
        // ESP_LOGI(TAG, "Task List:\n%s\n", pcTaskList);
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
