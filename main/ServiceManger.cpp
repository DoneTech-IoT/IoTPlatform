#include "ServiceManger.h"
#include "lvglGui.h"
#include <stdio.h>

void ServiceMangerTask(void *pvParameter);
static const char *TAG = "Service Manger";
StaticTask_t *xTaskServiceMangerBuffer;
StackType_t *xServiceMangerStack;
ServiceManger_t ServiceManger;

void ServiceMangerTaskInit()
{
    xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceMangerTask,
        "ServiceMangerTask",
        SERVICE_MANGER_STACK,
        NULL,
        tskIDLE_PRIORITY,
        xServiceMangerStack,
        xTaskServiceMangerBuffer);
}
void GUI_TaskCreator()
{
    ServiceManger.tasks[GUI_Task].maximumRAM_Needed = LVGL_STACK * 2;
    strcpy(ServiceManger.tasks[GUI_Task].name, "GUI");
    ServiceManger.tasks[GUI_Task].ramType = PSRAM_;
    ServiceManger.tasks[GUI_Task].startupRAM = GUI_Task;
    ServiceManger.tasks[GUI_Task].TaskCreator = GUI_TaskCreator;
    ServiceManger.tasks[GUI_Task].taskStack = LVGL_STACK;
    ServiceManger.tasks[GUI_Task].priority = tskIDLE_PRIORITY + 1;
    ServiceManger.tasks[GUI_Task].taskHandler = NULL;
    GUI_TaskInit(&ServiceManger.tasks[GUI_Task].taskHandler, ServiceManger.tasks[GUI_Task].priority, ServiceManger.tasks[GUI_Task].taskStack);
}
void TaskKiller(int TaskNumber)
{
    vTaskDelete(ServiceManger.tasks[TaskNumber].taskHandler);
    ESP_LOGI(TAG, "Task %d Deleted !", TaskNumber);
}
void ServiceMangerInit()
{
    GUI_TaskCreator();
    ESP_LOGI(TAG, "GUI Created !");
    // SpotifyTaskCreator();
    // ESP_LOGI(TAG, "Spotify Created !");
}
void ServiceMangerTask(void *pvParameter)
{
    ServiceMangerInit();
    vTaskDelay(pdMS_TO_TICKS(5000));
    TaskKiller(GUI_Task);
    vTaskDelay(pdMS_TO_TICKS(5000));
    ServiceManger.tasks[GUI_Task].TaskCreator();
    // while (true)
    // {

    // }
}