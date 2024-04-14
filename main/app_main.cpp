#include "lvglGui.h"
#include "GlobalInit.h"
#include "nvsFlash.h"
#include "SpotifyInterface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "MatterInterface.h"

#define TIMER_TIME pdMS_TO_TICKS(500) // in millis
QueueHandle_t MatterBufQueue;
SemaphoreHandle_t MatterSemaphore = NULL;
MatterInterfaceHandler_t MatterInterfaceHandler;
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";
SpotifyInterfaceHandler_t SpotifyInterfaceHandler;

// ****************************** GLobal Functions ****************************** //
/**
 * @brief Function to change colors based on a timer callback
 */
void SpotifyPeriodicTimer(TimerHandle_t xTimer)
{
    bool CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, GetNowPlaying);
    if (CommandResult == false)
    {
        ESP_LOGE(TAG, "Playback info update failed");
        return;
    }
    GUI_UpdateSpotifyScreen(SpotifyInterfaceHandler.PlaybackInfo->ArtistName,
                            SpotifyInterfaceHandler.PlaybackInfo->SongName,
                            SpotifyInterfaceHandler.PlaybackInfo->AlbumName,
                            SpotifyInterfaceHandler.PlaybackInfo->Duration,
                            SpotifyInterfaceHandler.PlaybackInfo->Progress);
    ESP_LOGI(TAG, "Playback info updated");
}
// ****************************** GLobal Functions ****************************** //

void CallbackTest(char *buffer)
{
    ESP_LOGW("Spotify_callback_test ", "%s", buffer);
}

extern "C" void app_main()
{
    size_t freeHeapSize;
    freeHeapSize = xPortGetFreeHeapSize();
    ESP_LOGW("TAG", "Free Heap Size: %u bytes\n", freeHeapSize);
    GUI_TaskInit();
    freeHeapSize = xPortGetFreeHeapSize();
    ESP_LOGW("TAG", "Free Heap Size: %u bytes\n", freeHeapSize);
    GlobalInit();
    nvsFlashInit();
    SpiffsGlobalConfig();
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.ChangeGUIBuyMatterRequest=&MatterNetworkConnection;
    // MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    Matter_TaskInit(&MatterInterfaceHandler);
    vTaskDelay((pdMS_TO_TICKS(SEC * 5)));

    SpotifyInterfaceHandler.IsSpotifyAuthorizedSemaphore = &IsSpotifyAuthorizedSemaphore;
    SpotifyInterfaceHandler.ConfigAddressInSpiffs = SpotifyConfigAddressInSpiffs;
    Spotify_TaskInit(&SpotifyInterfaceHandler);
    unsigned int numberOfTasks = uxTaskGetNumberOfTasks();
    printf("Number of tasks: %u\n", numberOfTasks);
    printf("CONFIG_FREERTOS_HZ =%d\n", CONFIG_FREERTOS_HZ);
    freeHeapSize = xPortGetFreeHeapSize();
    ESP_LOGE(TAG, "Free Heap Size: %u bytes\n", freeHeapSize);
    // after this semaphore you can use playback command function in every where !
    if (xSemaphoreTake(IsSpotifyAuthorizedSemaphore, portMAX_DELAY) == pdTRUE)
    {
        bool CommandResult = false;
        CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, GetUserInfo);
        if (CommandResult == false)
        {
            ESP_LOGE(TAG, "User info update failed");
            return;
        }
        ESP_LOGI(TAG, "User info updated");
        TimerHandle_t xTimer = xTimerCreate("update", TIMER_TIME, pdTRUE, NULL, SpotifyPeriodicTimer);
        xTimerStart(xTimer, 0);
        if (xTimer != NULL)
        {
            if (xTimerStart(xTimer, 0) == pdPASS)
            {
                ESP_LOGI(TAG, "Timer getting start");
            }
        }
    }
}