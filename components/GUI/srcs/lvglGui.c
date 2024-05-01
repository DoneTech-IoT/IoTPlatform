#include "lvglGui.h"
#include "gui_guider.h"

StaticTask_t *xTaskLVGLBuffer;
StackType_t *xLVGLStack;
lv_color_t *LVGL_BigBuf1;
lv_color_t *LVGL_BigBuf2;

static const char *TAG = "LVGL_GUI";
void GUI_mainTask(void *pvParameter);

uint8_t GUI_AllocationMemory(uint32_t Stack)
{
    xTaskLVGLBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    xLVGLStack = (StackType_t *)malloc(Stack * MULTIPLIER * sizeof(StackType_t));
    LVGL_BigBuf1 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    LVGL_BigBuf2 = (lv_color_t *)malloc(LV_HOR_RES_MAX * 100 * MULTIPLIER * sizeof(lv_color_t));
    if (xTaskLVGLBuffer == NULL || xLVGLStack == NULL || LVGL_BigBuf1 == NULL || LVGL_BigBuf2 == NULL)
    {
        ESP_LOGE(TAG, "Memory allocation failed!");
        free(xTaskLVGLBuffer);
        free(xLVGLStack);
        free(LVGL_BigBuf2);
        free(LVGL_BigBuf1);
        return 0;
    }
    return 1;
}

/**
 * @brief Function to creat GUI task in staticly
 */
void GUI_TaskInit(TaskHandle_t *GuiTaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack)
{
    if (!GUI_AllocationMemory(TaskStack))
        return;
    *GuiTaskHandler = xTaskCreateStatic(
        GUI_mainTask,           // Task function
        "GUI_mainTask",         // Task name (for debugging)
        TaskStack * MULTIPLIER, // Stack size (in words)
        NULL,                   // Task parameters (passed to the task function)
        TaskPriority,           // Task priority (adjust as needed)
        xLVGLStack,             // Stack buffer
        xTaskLVGLBuffer         // Task control block
    );
    // this delay so important
    vTaskDelay(500);
}

/**
 * @brief main LVGL gui task this is important that know
 * all LVGL deriver function should be call in main task
 * you can not call them in other function !
 * this task run GUI with all thing that needed
 */
void GUI_mainTask(void *pvParameter)
{
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
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1));
        lv_task_handler();
    }
}

void KillGUI_Task(TaskHandle_t GUITaskHandler)
{
    vTaskDelete(GUITaskHandler);
    free(xTaskLVGLBuffer);
    free(xLVGLStack);
    free(LVGL_BigBuf2);
    free(LVGL_BigBuf1);
}

/**
 * @brief Function to update the LVGL screen
 * @param Artist: Artist name
 * @param Title: Title of the song
 * @param Album: Album name
 * @return void
 */
void GUI_UpdateSpotifyScreen(bool songUpdated, char *Artist, char *Song, char *Album, int DurationMS, int ProgressMS, uint8_t *coverPhoto)
{
    int minutues = ProgressMS / 60000;
    int second = (ProgressMS % 60000) / 1000;
    char time[20];
    sprintf(time, "%d:%d", minutues, second);
    lv_event_send(guider_ui.Spotify_Page_label_time, LV_EVENT_VALUE_CHANGED, time);

    if (DurationMS == 0)
    {
        ESP_LOGE(TAG, "Duration is zero");
        return;
    }
    int progress = (ProgressMS * 100) / DurationMS;
    lv_event_send(guider_ui.Spotify_Page_bar_progress, LV_EVENT_VALUE_CHANGED, progress);

    if (!songUpdated)
    {
        return;
    }
    lv_event_send(guider_ui.Spotify_Page_Artist_name, LV_EVENT_VALUE_CHANGED, Artist);
    lv_event_send(guider_ui.Spotify_Page_Song_name, LV_EVENT_VALUE_CHANGED, Song);
    lv_event_send(guider_ui.Spotify_Page_Album_name, LV_EVENT_VALUE_CHANGED, Album);
    lv_event_send(guider_ui.Matter_logo, LV_EVENT_VALUE_CHANGED, NULL);
    lv_event_send(guider_ui.Spotify_Page_img_song, LV_EVENT_VALUE_CHANGED, coverPhoto);
}

void MatterNetworkConnected()
{
    lv_event_send(guider_ui.Matter_logo, LV_EVENT_VALUE_CHANGED, NULL);
}
