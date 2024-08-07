#include "lvglGui.h"
#include "GlobalInit.h"
#include "nvsFlash.h"
#include "SpotifyInterface.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Setup_GPIO.h"
#include "MatterInterface.h"
#include "ServiceManger.h"
#include "Custom_Log.h"
#define CONFIG_DONE_COMPONENT_MQTT
#define TIMER_TIME pdMS_TO_TICKS(500) // in millis
#ifdef CONFIG_DONE_COMPONENT_MQTT
QueueHandle_t MQTTDataFromBrokerQueue;
SemaphoreHandle_t MQTTConnectedSemaphore;
SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
#endif
#ifdef CONFIG_DONE_COMPONENT_MATTER
QueueHandle_t MatterBufQueue;
SemaphoreHandle_t MatterSemaphore = NULL;
MatterInterfaceHandler_t MatterInterfaceHandler;
#endif
// ****************************** GLobal Variables ****************************** //
static const char *TAG = "Main";

#ifdef CONFIG_DONE_COMPONENT_SPOTIFY
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

#ifdef CONFIG_DONE_COMPONENT_LVGL
    GUI_UpdateSpotifyScreen(isNewSong,
                            SpotifyInterfaceHandler.PlaybackInfo->ArtistName,
                            SpotifyInterfaceHandler.PlaybackInfo->SongName,
                            SpotifyInterfaceHandler.PlaybackInfo->AlbumName,
                            SpotifyInterfaceHandler.PlaybackInfo->Duration,
                            SpotifyInterfaceHandler.PlaybackInfo->Progress,
                            SpotifyInterfaceHandler.CoverPhoto);
    ESP_LOGI(TAG, "Playback info updated");
#endif
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
#endif
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

    Log_RamOccupy("main", "service manager");

    ServiceMangerTaskInit();
    vTaskDelay(pdMS_TO_TICKS(1 * SEC));

    GlobalInit();
    nvsFlashInit();
    SpiffsGlobalConfig();

#ifdef CONFIG_DONE_COMPONENT_MATTER
    Log_RamOccupy("main", "service manager");
    Log_RamOccupy("main", "Matter usage");
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    MatterInterfaceHandler.ConnectToMatterNetwork = MatterNetworkConnected;
    Matter_TaskInit(&MatterInterfaceHandler);
#endif
#ifdef CONFIG_DONE_COMPONENT_MQTT
    static PublishConfig_str PublishConfig;
    static SubscriptionConfig_str SubscriptionConfig;
    static MQTT_Configuration_str Interface;
    SubscriptionConfig.Qos = 1;
    SubscriptionConfig.Retain = 0;
    strcpy(SubscriptionConfig.Topic, "test/azmon/bib");

    PublishConfig.Qos = 1;
    PublishConfig.Retain = 0;
    strcpy(PublishConfig.Topic, "test/azmon/bib");

    Interface.ClientID = 2225;
    Interface.PublishConfig = &PublishConfig;
    Interface.SubscriptionConfig = &SubscriptionConfig;

    Interface.ConnectedSemaphore = &MQTTConnectedSemaphore;
    Interface.DataFromBrokerQueue = &MQTTDataFromBrokerQueue;
    Interface.ErrorOrDisconnectSemaphore = &MQTTErrorOrDisconnectSemaphore;

    esp_err_t error;

    error = MQTT_Init(&Interface);
    if (error == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT Inited!");
    }
    else if (error == ESP_FAIL)
    {
        ESP_LOGE(TAG, "MQTT iit fail!");
    }
#endif
#ifdef CONFIG_DONE_COMPONENT_SPOTIFY
    vTaskDelay(pdMS_TO_TICKS(5000));
    Log_RamOccupy("main", "Matter usage");
    Log_RamOccupy("main", "spotify");

    SpotifyInterfaceHandler.IsSpotifyAuthorizedSemaphore = &IsSpotifyAuthorizedSemaphore;
    SpotifyInterfaceHandler.ConfigAddressInSpiffs = SpotifyConfigAddressInSpiffs;
    Spotify_TaskInit(&SpotifyInterfaceHandler);

    vTaskDelay(pdMS_TO_TICKS(3000));

    Log_RamOccupy("main", "spotify");

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
#endif
}

void MatterAttributeUpdateCBMain(
    callback_type_t type,
    uint16_t endpoint_id, uint32_t cluster_id,
    uint32_t attribute_id, esp_matter_attr_val_t *val,
    void *priv_data)
{
    // printf("callback_type_t: %u\n", type);
    // printf("endpoint_id: %u\n", endpoint_id);
    // printf("cluster_id: %lu\n", cluster_id);
    // printf("attribute_id: %lu\n", attribute_id);
    // printf("val: %p\n", val);
    // printf("priv_data: %p\n", priv_data);
}

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"
#include "esp_log.h"
#include "MQTT_Interface.h"

static const char *TAG = "main";

void app_main(void)
{

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    ESP_ERROR_CHECK(example_connect());

    SubscriptionConfig.Qos = 1;
    SubscriptionConfig.Retain = 0;
    strcpy(SubscriptionConfig.Topic, "test/azmon/bib");

    PublishConfig.Qos = 1;
    PublishConfig.Retain = 0;
    strcpy(PublishConfig.Topic, "test/azmon/bib");

    Interface.ClientID = 2225;
    Interface.PublishConfig = &PublishConfig;
    Interface.SubscriptionConfig = &SubscriptionConfig;

    Interface.ConnectedSemaphore = &MQTTConnectedSemaphore;
    Interface.DataFromBrokerQueue = &MQTTDataFromBrokerQueue;
    Interface.ErrorOrDisconnectSemaphore = &MQTTErrorOrDisconnectSemaphore;

    esp_err_t error;

    error = MQTT_Init(&Interface);
    if (error == ESP_OK)
    {
        ESP_LOGI(TAG, "MQTT Inited!");
    }
    else if (error == ESP_FAIL)
    {
        ESP_LOGE(TAG, "MQTT iit fail!");
    }
}
