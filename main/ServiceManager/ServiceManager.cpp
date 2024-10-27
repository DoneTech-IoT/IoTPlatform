#include "ServiceManager.h"
#include "CoffeeMakerApp.hpp"
#include "SharedBus.h"
#include "GUI.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#define WIFI_SSID      "Done_IOT_1"
#define WIFI_PASS      "87654321@"
static EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
static const char *TAG = "wifi";
static int retry_count = 0;
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Disconnected from WiFi, retrying...");
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "Connected! IP Address: " IPSTR, IP2STR(&event->ip_info.ip));
    }
}

void wifi_init_sta(void) {
    // Initialize NVS
    // esp_err_t ret = nvs_flash_init();
    // if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    //     ESP_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // ESP_ERROR_CHECK(ret);

    // Initialize TCP/IP stack
    esp_netif_init();
    esp_event_loop_create_default();

    // Create WiFi station interface
    esp_netif_create_default_wifi_sta();

    // Configure WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    // Register event handler for WiFi and IP events
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL, &instance_any_id);
    
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL, &instance_got_ip);
    // Set WiFi configuration
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    // Start WiFi
    esp_wifi_start();

    ESP_LOGI(TAG, "WiFi initialization complete.");
}

//static const char *TAG = "Service_Manager";

#ifdef CONFIG_DONE_COMPONENT_MATTER
MatterInterfaceHandler_t MatterInterfaceHandler;
static QueueHandle_t MatterBufQueue;
static SemaphoreHandle_t MatterSemaphore = NULL;
static TaskHandle_t MatterHandle = NULL;
#endif  //CONFIG_DONE_COMPONENT_MATTER

#ifdef CONFIG_DONE_COMPONENT_MQTT
MQTT_InterfaceHandler_t MQTT_InterfaceHandler;
static QueueHandle_t MQTTDataFromBrokerQueue;
static SemaphoreHandle_t MQTTConnectedSemaphore;
static SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
static TaskHandle_t MQTTHandle = NULL;

#endif  //CONFIG_DONE_COMPONENT_MQTT

#define TASK_LIST_BUFFER_SIZE 512
// #define MONITORING
/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter);

// Global instance of Service Manager
ServiceManger_t ServiceManger;

#ifdef CONFIG_DONE_COMPONENT_MATTER
void MatterAttributeUpdateCBMain(callback_type_t type,
                                 uint16_t endpoint_id, uint32_t cluster_id,
                                 uint32_t attribute_id, esp_matter_attr_val_t *val,
                                 void *priv_data)
{
    printf("callback_type_t: %u\n", type);
    printf("endpoint_id: %u\n", endpoint_id);
    printf("cluster_id: %lu\n", cluster_id);
    printf("attribute_id: %lu\n", attribute_id);
    printf("val: %p\n", val);
    printf("priv_data: %pGlobalInitGlobalInitGlobalInit\n", priv_data);
}
#endif

/**
 * @brief Deletes a task.
 * This function deletes the specified task.
 * @param TaskNumber The number of the task to be deleted.
 * @return void
 */
void TaskKiller(int TaskNumber)
{
    ServiceManger.Services[TaskNumber].TaskKiller(&ServiceManger.Services[TaskNumber].taskHandler);
    ESP_LOGI(TAG, "Task %d Deleted !", TaskNumber);
}

/**
 * @brief Initializes the Service Manager task.
 * This function initializes the Service Manager task by allocating memory and creating the task.
 * @return void
 */
void ServiceManger_Init()
{
    StaticTask_t *xTaskServiceMangerBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
    StackType_t *xServiceMangerStack = (StackType_t *)malloc(SERVICE_MANGER_STACK * sizeof(StackType_t));
    xTaskCreateStatic(
        ServiceMangerTask,         // Task function
        "ServiceMangerTask",       // Task name (for debugging)
        SERVICE_MANGER_STACK,      // Stack size (in words)
        NULL,                      // Task parameters (passed to the task function)
        tskIDLE_PRIORITY + 1,      // Task priority (adjust as needed)
        xServiceMangerStack,       // Stack buffer
        xTaskServiceMangerBuffer); // Task control block
}

/* 
    * @brief run given service.
    * This function runs the given service by initializing the service parameters and creating the task.
    * @param[in] serviceParams Service parameters
    * @param[in] id Service to run
    * @retval ESP_OK if the service is run successfully, otherwise ESP_FAIL
*/ 
esp_err_t ServiceManager_RunService(ServiceParams_t serviceParams)
{
    esp_err_t err = ESP_OK;

    // Call the function pointer (init_func) with proper arguments
    TaskInitPtr init_func = serviceParams.TaskInit;
    err = init_func(serviceParams.interfaceHandler, 
                    &serviceParams.taskHandler, 
                    serviceParams.priority, 
                    serviceParams.taskStack);
    if (err != ESP_OK) 
    {
        ESP_LOGE(TAG, "Failed to create MQTT!");
        return err;
    }

    return ESP_OK;
}

/**
 * @brief Task function for the Service Manager task.
 * This task initializes and manages other tasks.
 * @param pvParameter Pointer to task parameters.
 * @return void
 */
void ServiceMangerTask(void *pvParameter)
{
    esp_err_t err;
    nvsFlashInit();
    
    if (SharedBusInit())
    {
        ESP_LOGI(TAG, "initialized SharedBus successfully");
    }
    else
    {
        ESP_LOGE(TAG, "Failed to Initialize SharedBus.");
    }

    ServiceParams_t GUIParams;
    GUIParams.maximumRAM_Needed = LVGL_STACK * 2;
    strcpy(GUIParams.name, "GUI");
    GUIParams.ramType = PSRAM_;
    GUIParams.TaskKiller = GUI_TaskKill;
    GUIParams.taskStack = LVGL_STACK;
    GUIParams.priority = tskIDLE_PRIORITY + 1;
    GUIParams.taskHandler = NULL;
    GUIParams.TaskInit = GUI_TaskInit;
    err = ServiceManager_RunService(GUIParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create GUI!");
    }
    else
    {
        ESP_LOGI(TAG, "GUI Created !");        
    }
    vTaskDelay(pdMS_TO_TICKS(1000));

// #ifdef CONFIG_DONE_COMPONENT_MATTER
//     // Config and Run Matter
//     MatterInterfaceHandler.SharedBufQueue = &MatterBufQueue;
//     MatterInterfaceHandler.SharedSemaphore = &MatterSemaphore;
//     MatterInterfaceHandler.MatterAttributeUpdateCB = MatterAttributeUpdateCBMain;
    
//     ServiceParams_t MatterParams;
//     strcpy(MatterParams.name, "Matter");
//     MatterParams.interfaceHandler = &MatterInterfaceHandler;
//     MatterParams.maximumRAM_Needed = 0;
//     MatterParams.ramType = SRAM_;
//     MatterParams.TaskKiller = Matter_TaskKill;
//     MatterParams.taskStack = MATTER_STACK_SIZE;
//     MatterParams.priority = tskIDLE_PRIORITY + 1;
//     MatterParams.taskHandler = MatterHandle;
//     MatterParams.TaskInit = Matter_TaskInit;
//     err = ServiceManager_RunService(MatterParams);
//     if (err)
//     {
//         ESP_LOGE(TAG, "Failed to create Matter !");
//     }
//     else 
//     {
//         ESP_LOGI(TAG, "Matter Created !");
//     }
// #endif

//     vTaskDelay(pdMS_TO_TICKS(100));
//TODO wifi
    wifi_init_sta();


#ifdef CONFIG_DONE_COMPONENT_MQTT
    // Config and Run MQTT
    MQTT_InterfaceHandler.ErrorDisconnectSemaphore = &MQTTErrorOrDisconnectSemaphore;
    MQTT_InterfaceHandler.IsConnectedSemaphore = &MQTTConnectedSemaphore;
    MQTT_InterfaceHandler.BrokerIncomingDataQueue = &MQTTDataFromBrokerQueue;

    ServiceParams_t MQTTParams;
    strcpy(MQTTParams.name, "MQTT");
    MQTTParams.maximumRAM_Needed = 0;
    MQTTParams.interfaceHandler = &MQTT_InterfaceHandler;
    MQTTParams.ramType = SRAM_;
    MQTTParams.TaskKiller = MQTT_TaskKill;
    MQTTParams.taskStack = MQTT_STACK;
    MQTTParams.priority = tskIDLE_PRIORITY + 1;
    MQTTParams.taskHandler = MQTTHandle;
    MQTTParams.TaskInit = MQTT_TaskInit;
    err = ServiceManager_RunService (MQTTParams);
    if (err)
    {
        ESP_LOGE(TAG, "Failed to create MQTT !");
    }
    else
    {
        ESP_LOGI(TAG, "MQTT Created !");
        vTaskDelay(pdMS_TO_TICKS(500));
        MQTT_Start();
        vTaskDelay(pdMS_TO_TICKS(500));   
    }
#endif  //CONFIG_DONE_COMPONENT_MQTT

    CoffeeMakerApplication(&MQTTDataFromBrokerQueue, &MQTTConnectedSemaphore, &MQTTErrorOrDisconnectSemaphore);
    //char pcTaskList[TASK_LIST_BUFFER_SIZE];
    while (true)
    {
#ifdef MONITORING
// vTaskList(pcTaskList);
// ESP_LOGI(TAG, "Task List:\n%s\n", pcTaskList);
#endif
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
