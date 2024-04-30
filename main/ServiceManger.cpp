#include "ServiceManger.h"

#include <stdio.h>
static const char *TAG = "Service Manger";
// Function to initialize and test Task Manager
// void testServiceManger()
// {
//     ServiceManger ServiceManger;
    // ServiceManger.maxSRAM = 0;
    // Task task1 = {"Task 1", 1, 100, NULL, SRAM};
    // Task task2 = {"Task 2", 2, 150, NULL, PSRAM};
    // Task task3 = {"Task 3", 3, 120, NULL, SRAM};

    // // Add tasks to the task manager
    // ServiceManger.tasks[0] = task1;
    // ServiceManger.tasks[1] = task2;
    // ServiceManger.tasks[2] = task3;

    // Calculate maximum SRAM needed by any task
    // for (int i = 0; i < 3; i++)
    // {
    //     if (ServiceManger.tasks[i].ramType == SRAM && ServiceManger.tasks[i].startupRAM > ServiceManger.maxSRAM)
    //     {
    //         ServiceManger.maxSRAM = ServiceManger.tasks[i].startupRAM;
    //     }
    // }
    // ESP_LOGI(TAG, "Task Manager Details:\n");
    // // ESP_LOGI(TAG, "Maximum SRAM needed by any task: %d\n", (int)(ServiceManger.maxSRAM));
    // ESP_LOGI(TAG, "Tasks:\n");
    // for (int i = 0; i < 3; i++)
    // {
    //     ESP_LOGI(TAG, "Task %d:\n", (int)(i + 1));
    //     ESP_LOGI(TAG, "Name: %s\n", ServiceManger.tasks[i].name);
    //     ESP_LOGI(TAG, "Priority: %d\n", (int)ServiceManger.tasks[i].priority);
    //     // ESP_LOGI(TAG, "Startup RAM needed: %d\n", (int)ServiceManger.tasks[i].startupRAM);
    //     ESP_LOGI(TAG, "RAM Type: %s\n", (ServiceManger.tasks[i].ramType == SRAM) ? "SRAM" : "PSRAM");
    //     ESP_LOGI(TAG, "\n");
    // }
// }
