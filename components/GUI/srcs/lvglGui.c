#include "lvglGui.h"
#include "gui_guider.h"
#include "Custom_Log.h"
StaticTask_t *xTaskLVGLBuffer;
StackType_t *xLVGLStack;
lv_color_t *LVGL_BigBuf1;
lv_color_t *LVGL_BigBuf2;

static const char *TAG = "LVGL_GUI";
void GUI_mainTask(void *pvParameter);

/**
 * @brief Allocates memory for LVGL components.
 * This function allocates memory for LVGL components such as task buffer, task stack,
 * and two big buffers.
 * @param Stack Size of the LVGL task stack.
 * @return 1 if memory allocation succeeds, 0 otherwise.
 */
uint8_t GUI_MemoryAllocation(uint32_t Stack)
{
    Log_RamOccupy("LVGL", "allocate memory");
    xTaskLVGLBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xLVGLStack = (StackType_t *)malloc(Stack * sizeof(StackType_t));
    LVGL_BigBuf1 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    LVGL_BigBuf2 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    if (xTaskLVGLBuffer == NULL || xLVGLStack == NULL || LVGL_BigBuf1 == NULL || LVGL_BigBuf2 == NULL)
    {
        ESP_LOGE(TAG, "Memory allocation failed!");
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
        return false;
        Log_RamOccupy("LVGL", "allocate memory");
        return 0;
    }
    return true;
    Log_RamOccupy("LVGL", "allocate memory");
    return 1;
}

/**
 * @brief Initializes the GUI task.
 * This function initializes the GUI task by allocating memory and creating the task.
 * @param GuiTaskHandler Pointer to the variable that will hold the GUI task handler.
 * @param TaskPriority Priority of the GUI task.
 * @param TaskStack Size of the GUI task stack.
 * @return void
 */
void GUI_TaskInit(TaskHandle_t *GuiTaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack)
{
    uint8_t GUI_MemoryAllocationStatus = GUI_MemoryAllocation(TaskStack);
    if (GUI_MemoryAllocationStatus == false)
    {
        ESP_LOGE(TAG, "GUI task can not be created ");
        return;
    }
    Log_RamStatus("LVGL", "create task");
    Log_RamOccupy("LVGL", "create task");
    *GuiTaskHandler = xTaskCreateStatic(
        GUI_mainTask,   // Task function
        "GUI_mainTask", // Task name (for debugging)
        TaskStack,      // Stack size (in words)
        NULL,           // Task parameters (passed to the task function)
        TaskPriority,   // Task priority (adjust as needed)
        xLVGLStack,     // Stack buffer
        xTaskLVGLBuffer // Task control block
    );
    Log_RamOccupy("LVGL", "create task");
    // this delay so important
    vTaskDelay(500);
    ESP_LOGI(TAG, "GUI task successfully created!");
}

/**
 * @brief Main task for LVGL GUI operations.
 * This function initializes LVGL, sets up the display driver,
 * registers the display driver, sets up the user interface,
 * starts the LVGL timer, and handles LVGL tasks continuously.
 * @param pvParameter Pointer to task parameters (unused).
 * @return void
 */
void GUI_mainTask(void *pvParameter)
{
    Log_RamOccupy("LVGL", "starting GUI task");
    lv_disp_draw_buf_t disp_draw_buf;
    lv_init();
    lvgl_driver_init();
    lv_disp_draw_buf_init(&disp_draw_buf, LVGL_BigBuf1, LVGL_BigBuf2, LV_HOR_RES_MAX * 100);
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.draw_buf = &disp_draw_buf;
    lv_disp_drv_register(&disp_drv);
    setup_ui(&guider_ui);
    LVGL_Timer();
    Log_RamOccupy("LVGL", "starting GUI task");

    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
        lv_task_handler();
        // lv_refr_now(NULL);
    }
}

/**
 * @brief Kills the GUI task and frees allocated memory.
 * This function deletes the GUI task and frees the memory allocated for LVGL components.
 * @param GUITaskHandler Handler of the GUI task to be killed.
 * @return void
 */
void GUI_TaskKill(TaskHandle_t *TaskHandler)
{
    if (*TaskHandler == NULL)
    {
        lv_deinit();
        vTaskDelete(TaskHandler);
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
    }
}
#define SEC 1000

void TimerTest(int milliseconds)
{
    int totalSeconds = milliseconds / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    char *timeString = (char *)malloc(20 * sizeof(char));
    if (timeString == NULL)
    {
        return;
    }
    snprintf(timeString, 20, "%02d:%02d", minutes, seconds);
    lv_event_send(guider_ui.screen_Timer, LV_EVENT_VALUE_CHANGED, timeString);
}
void CountOfCountTest(int cup)
{
    lv_event_send(guider_ui.screen_CountOfCup, LV_EVENT_VALUE_CHANGED, (void *)cup);
}
void GUItest()
{
    // CountOfCountTest(1);
    lv_event_send(guider_ui.screen_Coffee, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC);
    lv_event_send(guider_ui.screen_Coffee, LV_EVENT_KEY, NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 2);
    // lv_event_send(guider_ui.screen_tea, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 3);
    // lv_event_send(guider_ui.screen_tea, IMAGE_OFF, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 4);
    // lv_event_send(guider_ui.screen_scop, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 5);
    // lv_event_send(guider_ui.screen_SmallGrind, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 6);
    // lv_event_send(guider_ui.screen_SmallGrind, IMAGE_OFF, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 7);
    // lv_event_send(guider_ui.screen_MediumGrind, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 8);
    // lv_event_send(guider_ui.screen_MediumGrind, IMAGE_OFF, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 9);
    // lv_event_send(guider_ui.screen_longGrind, IMAGE_ON, NULL);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // TimerTest(SEC * 10);
    // lv_event_send(guider_ui.screen_longGrind, IMAGE_OFF, NULL);
    // CountOfCountTest(6);
}
