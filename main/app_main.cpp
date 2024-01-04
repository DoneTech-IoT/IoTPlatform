#include "GlobalInit.h"
#include "nvsFlash.h"
#include "MatterInterface.h"
#include "SpotifyInterface.h"

// ****************************** GLobal Variables ****************************** //
SemaphoreHandle_t IsSpotifyAuthorizedSemaphore;
SemaphoreHandle_t WifiParamExistenceCheckerSemaphore;
SemaphoreHandle_t FinishWifiConfig;
GlobalInitInterfaceHandler_t GlobalInitInterfaceHandler;

QueueHandle_t HttpsBufQueue;
SemaphoreHandle_t HttpsResponseReadySemaphore;
SpotifyInterfaceHandler_t SpotifyInterfaceHandler;  

QueueHandle_t MatterBufQueue;
SemaphoreHandle_t MatterSemaphore = NULL;
MatterInterfaceHandler_t MatterInterfaceHandler;

SemaphoreHandle_t SpiffsMutex;      
// ****************************** GLobal Functions ****************************** //
void MatterAttributeUpdateCBMain(callback_type_t type, 
                uint16_t endpoint_id, uint32_t cluster_id,
                uint32_t attribute_id, esp_matter_attr_val_t *val, 
                void *priv_data);

extern "C" void app_main()
{   
    nvsFlashInit();
    nvs_flash_init();

    GlobalInitInterfaceHandler.HttpsBufQueue = &HttpsBufQueue;
    GlobalInitInterfaceHandler.HttpsResponseReadySemaphore = &HttpsResponseReadySemaphore;
    GlobalInitInterfaceHandler.IsSpotifyAuthorizedSemaphore = &IsSpotifyAuthorizedSemaphore;
    GlobalInitInterfaceHandler.WifiParamExistenceCheckerSemaphore = &WifiParamExistenceCheckerSemaphore;
    GlobalInitInterfaceHandler.FinishWifiConfig = &FinishWifiConfig;
    GlobalInitInterfaceHandler.SpiffsMutex = &SpiffsMutex;
    GlobalInit(&GlobalInitInterfaceHandler);
        
    MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
    MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
    MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    Matter_TaskInit(&MatterInterfaceHandler);

#ifdef SpotifyEnable
    SpotifyInterfaceHandler.HttpsBufQueue = &HttpsBufQueue;
    SpotifyInterfaceHandler.HttpsResponseReadySemaphore = &HttpsResponseReadySemaphore;
    SpotifyInterfaceHandler.ConfigAddressInSpiffs = SpotifyConfigAddressInSpiffs;
    //SpotifyInterfaceHandler.ReadTxtFileFromSpiffs = SpiffsWrite;
    // SpotifyInterfaceHandler.WriteTxtFileToSpiffs = SaveFileInSpiffsWithTxtFormat;
    // SpotifyInterfaceHandler.CheckAddressInSpiffs = SpiffsExistenceCheck;

    Spotify_TaskInit(&SpotifyInterfaceHandler, SPOTIFY_TASK_STACK_SIZE);
#endif

    vTaskDelay(5000/portTICK_PERIOD_MS);
    unsigned int numberOfTasks = uxTaskGetNumberOfTasks();
    printf("Number of tasks: %u\n", numberOfTasks);
    printf("CONFIG_FREERTOS_HZ =%d\n",CONFIG_FREERTOS_HZ);    
    size_t freeHeapSize;
    freeHeapSize = xPortGetFreeHeapSize();
    ESP_LOGE("TAG", "Free Heap Size: %u bytes\n", freeHeapSize);
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