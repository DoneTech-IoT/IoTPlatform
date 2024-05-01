#include "lvglGui.h"
#include "GlobalInit.h"
#include "nvsFlash.h"
#include "SpotifyInterface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "MatterInterface.h"
#include "ServiceManger.h"

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
    static char imgLink[100];
    bool CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, GetNowPlaying);
    if (CommandResult == false)
    {
        ESP_LOGE(TAG, "Playback info update failed");
        return;
    }

    bool isNewSong = strcmp(SpotifyInterfaceHandler.PlaybackInfo->SongImageURL, imgLink);
    if (isNewSong)
    {
        CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, GetCoverPhoto);
        vTaskDelay(pdMS_TO_TICKS(100));
        if (CommandResult == false)
        {
            ESP_LOGE(TAG, "Cover photo update failed");
            return;
        }
        strcpy(imgLink, SpotifyInterfaceHandler.PlaybackInfo->SongImageURL);
    }

    GUI_UpdateSpotifyScreen(isNewSong,
                            SpotifyInterfaceHandler.PlaybackInfo->ArtistName,
                            SpotifyInterfaceHandler.PlaybackInfo->SongName,
                            SpotifyInterfaceHandler.PlaybackInfo->AlbumName,
                            SpotifyInterfaceHandler.PlaybackInfo->Duration,
                            SpotifyInterfaceHandler.PlaybackInfo->Progress,
                            SpotifyInterfaceHandler.CoverPhoto);
    ESP_LOGI(TAG, "Playback info updated");
}
void IRAM_ATTR BackBottomCallBack_(void *arg, void *data)
{
    if (xSemaphoreTake(IsSpotifyAuthorizedSemaphore, 0) == pdTRUE)
    {
        bool CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, Play);
        if (CommandResult == false)
        {
            ESP_LOGE(TAG, "Play failed");
            return;
        }
    }
}
void IRAM_ATTR AcceptBottomCallBack_(void *arg, void *data)
{
    if (xSemaphoreTake(IsSpotifyAuthorizedSemaphore, 0) == pdTRUE)
    {
        bool CommandResult = Spotify_SendCommand(SpotifyInterfaceHandler, Pause);
        if (CommandResult == false)
        {
            ESP_LOGE(TAG, "Pause failed");
            return;
        }
    }
}
// ****************************** GLobal Functions ****************************** //
void MatterAttributeUpdateCBMain(callback_type_t type,
                                 uint16_t endpoint_id, uint32_t cluster_id,
                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
                                 void *priv_data);

void CallbackTest(char *buffer)
{
    ESP_LOGW("Spotify_callback_test ", "%s", buffer);
}

extern "C" void app_main()
{
    size_t freeHeapSize;
    freeHeapSize = xPortGetFreeHeapSize();
    UBaseType_t TaskPriority = tskIDLE_PRIORITY + 1;
    TaskHandle_t GuiTaskHandler ;
    GUI_TaskInit(&GuiTaskHandler, TaskPriority);
    freeHeapSize = xPortGetFreeHeapSize();
    ESP_LOGW("TAG", "Free Heap Size: %u bytes\n", freeHeapSize);
    GlobalInit();
    nvsFlashInit();
    SpiffsGlobalConfig();
    // testServiceManger();
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    Matter_TaskInit(&MatterInterfaceHandler);

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

void MatterAttributeUpdateCBMain(
    callback_type_t type,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id, esp_matter_attr_val_t *val,
    void *priv_data)
{
    printf("callback_type_t: %u\n", type);
    printf("endpoint_id: %u\n", endpoint_id);
    printf("cluster_id: %lu\n", cluster_id);
    printf("attribute_id: %lu\n", attribute_id);
    printf("val: %p\n", val);
    printf("priv_data: %p\n", priv_data);
}