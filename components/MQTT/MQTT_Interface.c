#include <stdio.h>
#include "MQTT_Interface.h"

// Tag used for logging MQTT interface messages
static const char *TAG = "MQTT_Interface";

// Static buffer and stack for the MQTT task
StaticTask_t *xTaskMQTTBuffer;
StackType_t *xMQTTStack;

// Structure holding MQTT interface configurations and state
MQTT_Interface_str MQTT_Interface;
// Array of user properties for MQTT messages
esp_mqtt5_user_property_item_t UserPropertyArray[USE_PROPERTY_ARR_SIZE];
// Structure holding MQTT thread connection data
MQTT_ThreadConnection_str MQTT_ThreadConnection;

/**
 * @brief Internal function to establish MQTT thread connection.
 * Initializes semaphores and queue for MQTT thread communication.
 */
static void MQTT_InternalConnection()
{
    MQTT_ThreadConnection.MQTT_ConnectedSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_DisconnectedSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_BrokerDataSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_ErrorInCoreSemaphore = xSemaphoreCreateBinary();
    MQTT_ThreadConnection.MQTT_BufQueue = xQueueCreate(1, sizeof(MQTT_Data_str));
    esp_err_t error = MQTTCore_ThreadConnectionMaker(&MQTT_ThreadConnection);
    if (error == ESP_FAIL)
    {
        ESP_LOGI(TAG, "can not make thread connection");
        return;
    }
}

/**
 * @brief Subscribes to a specified MQTT topic.
 * Attempts to subscribe to the given topic if the MQTT connection is active.
 * @param Topic The topic to subscribe to.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Subscribe(char *Topic)
{
    if (MQTT_Interface.IsConnectedFlag == 1)
    {

        char topic[TOPIC_LEN] = {0};
        strcpy(topic, Topic);
        MQTTCore_Subscriber(topic,
                            &MQTT_Interface.MainClient,
                            &MQTT_Interface.SubscribeProperty,
                            UserPropertyArray,
                            &MQTT_Interface.SubscribeProperty.user_property);
    }
    return false;
}

/**
 * @brief Unsubscribes from a specified MQTT topic.
 * Attempts to unsubscribe from the given topic if the MQTT connection is active.
 * @param Topic The topic to unsubscribe from.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_UnSubscribe(char *Topic)
{
    if (MQTT_Interface.IsConnectedFlag == 1)
    {
        char topic[TOPIC_LEN] = {0};
        strcpy(topic, Topic);
        MQTTCore_Unsubscribe(topic,
                             &MQTT_Interface.MainClient,
                             UserPropertyArray,
                             &MQTT_Interface.UnsubscribeProperty,
                             &MQTT_Interface.UnsubscribeProperty.user_property);
        return true;
    }
    return false;
}

/**
 * @brief Publishes a message to a specified MQTT topic.
 * Publishes the given data to the specified topic if the MQTT connection is active.
 * @param Topic The topic to publish to.
 * @param Data The data to publish.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Publish(char *Topic, char *Data)
{
    if (MQTT_Interface.IsConnectedFlag == 1)
    {
        MQTT_Data_str MQTT_Data;
        memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
        memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
        strcpy(MQTT_Data.Topic, Topic);
        strcpy(MQTT_Data.RawData, Data);
        MQTTCore_Publisher(MQTT_Data.Topic,
                           MQTT_Data.RawData,
                           &MQTT_Interface.MainClient,
                           UserPropertyArray,
                           &MQTT_Interface.PublishProperty,
                           &MQTT_Interface.PublishProperty.user_property,
                           MQTT_Interface.MQTT_Config.PublishConfig->Qos,
                           MQTT_Interface.MQTT_Config.PublishConfig->Retain);
        return true;
    }
    return false;
}

/**
 * @brief Disconnects from the MQTT broker.
 * This function handles the MQTT disconnection process.
 */
void MQTT_Disconnect()
{
    MQTTCore_Disconnect(&MQTT_Interface.MainClient,
                        UserPropertyArray,
                        &MQTT_Interface.DisconnectProperty,
                        &MQTT_Interface.DisconnectProperty.user_property);
}

/**
 * @brief Main task function for managing MQTT operations.
 * This task initializes semaphores and queues for MQTT operations, establishes
 * a thread connection, and handles various states such as initialization,
 * connection, publishing, subscribing, and disconnection.
 * @param pvParameter Parameters passed to the task (currently unused).
 * @return void
 */
void MQTT_MainTask(void *pvParameter)
{
    ESP_LOGI(TAG, "MQTT task getting run");

    TickType_t WaitForSemaphore = 0;
    uint8_t Counter = 0;

    MQTT_InternalConnection();
    MQTT_ConfigFunction(UserPropertyArray, &MQTT_Interface);
    int State = MQTT_INIT;
    while (true)
    {
        switch (State)
        {
        case MQTT_INIT:
        {
            static int Counter;
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_ConnectedSemaphore, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
            {
                xSemaphoreGive((*MQTT_Interface.MQTT_Config.ConnectedSemaphore));
                MQTT_Interface.IsConnectedFlag = 1;
                State = MQTT_CONNECTED;
            }
            else
            {
                ESP_LOGE(TAG, "connect else happen! counter = %d", Counter);
                Counter++;
                if (Counter == TRY_TO_CONNECT)
                {
                    ESP_LOGI(TAG, "stat 3");
                    State = MQTT_DISCONNECT;
                    break;
                }
                State = MQTT_INIT;
                break;
            }
            break;
        }
        case MQTT_CONNECTED:
        {
            ESP_LOGI(TAG, "MQTT_CONNECTED");
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_ErrorInCoreSemaphore, WaitForSemaphore) == pdTRUE)
            {
                State = MQTT_ERROR;
            }
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_BrokerDataSemaphore, WaitForSemaphore) == pdTRUE)
            {
                State = MQTT_DATA_CONNECTION;
            }
            if (xSemaphoreTake(MQTT_ThreadConnection.MQTT_DisconnectedSemaphore, WaitForSemaphore) == pdTRUE)
            {
                State = MQTT_DISCONNECT;
            }
#ifdef MQTT_TEST
            TestPublishAndSubscribe();
#endif
            break;
        }
        case MQTT_DATA_CONNECTION:
        {
            MQTT_Data_str MQTT_Data;
            memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
            memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
            if (xQueueReceive(MQTT_ThreadConnection.MQTT_BufQueue, &MQTT_Data, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
            {
                if (xQueueSend((*MQTT_Interface.MQTT_Config.DataFromBrokerQueue), MQTT_Data.RawData, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
                {
                    ESP_LOGI(TAG, "Send data to app layer");
                    test_MQtt();
                }
                else
                {
                    ESP_LOGE(TAG, "Send data to app layer failed");
                }
            }
            else
            {
                ESP_LOGE(TAG, "receive data from broker failed");
            }
            State = MQTT_CONNECTED;
            break;
        }
        case MQTT_DISCONNECT:
        {
            xSemaphoreGive(*MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore);
            State = MQTT_KILL;
            break;
        }
        case MQTT_KILL:
        {
            break;
        }
        case MQTT_ERROR:
        {
            xSemaphoreGive(*MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore);
            State = MQTT_CONNECTED;
            break;
        }
        }
    }
}

/**
 * @brief Initializes and creates the MQTT task.
 * This function sets up the MQTT task with the specified priority and stack size,
 * if the configuration flag is set. It allocates memory for the task stack and control block
 * and creates the task statically.
 * @param MQTT_TaskHandler Pointer to the task handle for the created task.
 * @param TaskPriority Priority of the task.
 * @param TaskStack Stack size for the task.
 * @return void
 */
void MQTT_TaskInit(TaskHandle_t *MQTT_TaskHandler, UBaseType_t TaskPriority, uint32_t TaskStack)
{
    if (MQTT_Interface.ConfigIsTrueFlag == 1)
    {
        xTaskMQTTBuffer = (StaticTask_t *)malloc(sizeof(StaticTask_t));
        xMQTTStack = (StackType_t *)malloc(TaskStack * sizeof(StackType_t));
        *MQTT_TaskHandler = xTaskCreateStatic(
            MQTT_MainTask,   // Task function
            "MQTT_MainTask", // Task name (for debugging)
            TaskStack,       // Stack size (in words)
            NULL,            // Task parameters (passed to the task function)
            TaskPriority,    // Task priority (adjust as needed)
            xMQTTStack,      // Stack buffer
            xTaskMQTTBuffer  // Task control block
        );
        ESP_LOGI(TAG, "MQTT task successfully created!");
    }
    else
    {
        ESP_LOGE(TAG, "MQTT task  need config !");
    }
}

/**
 * @brief Initializes the MQTT interface with the provided configuration.
 * This function sets up the MQTT interface using the specified configuration structure.
 * It validates the configuration and, if valid, initializes the task and sets the configuration flag.
 * @param MQTT_InputConfig Pointer to the configuration structure for MQTT.
 * @return esp_err_t Status code indicating success or failure.
 */
esp_err_t MQTT_Init(MQTT_Configuration_str *MQTT_InputConfig)
{
    if (MQTT_InputConfig->ConnectedSemaphore != NULL &&
        MQTT_InputConfig->DataFromBrokerQueue != NULL &&
        MQTT_InputConfig->ErrorOrDisconnectSemaphore != NULL &&
        MQTT_InputConfig->PublishConfig != NULL &&
        MQTT_InputConfig->SubscriptionConfig != NULL)
    {
        MQTT_Interface.ConfigIsTrueFlag = 1;
        MQTT_Interface.MQTT_Config.ClientID = MQTT_InputConfig->ClientID;
        MQTT_Interface.MQTT_Config.ConnectedSemaphore = MQTT_InputConfig->ConnectedSemaphore;
        MQTT_Interface.MQTT_Config.DataFromBrokerQueue = MQTT_InputConfig->DataFromBrokerQueue;
        MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore = MQTT_InputConfig->ErrorOrDisconnectSemaphore;
        MQTT_Interface.MQTT_Config.PublishConfig = MQTT_InputConfig->PublishConfig;
        MQTT_Interface.MQTT_Config.SubscriptionConfig = MQTT_InputConfig->SubscriptionConfig;

        MQTT_Interface.MQTT_Config.ConnectedSemaphore = xSemaphoreCreateBinary();
        MQTT_Interface.MQTT_Config.ErrorOrDisconnectSemaphore = xSemaphoreCreateBinary();
        MQTT_Interface.MQTT_Config.DataFromBrokerQueue = xQueueCreate(2500, sizeof(char));
        MQTT_Interface.IsConnectedFlag = 0;
        /*in future we check all config */
        /* To do ** save in spiffs */
        TaskHandle_t MQTT_TaskHandler;
        UBaseType_t TaskPriority = tskIDLE_PRIORITY + 1;
        uint32_t TaskStack = MQTT_STACK;
        /* At future this task should be create by service manager*/
        MQTT_TaskInit(&MQTT_TaskHandler, TaskPriority, MQTT_STACK);

        return ESP_OK;
    }
    else
        return ESP_FAIL;
}

/**
 * @brief Deletes the MQTT task and frees associated resources.
 * This function deletes the specified MQTT task and releases the memory allocated for
 * the task stack and control block.
 * @param TaskHandler Pointer to the task handle to be deleted.
 * @return void
 */
void MQTT_TaskKill(TaskHandle_t *TaskHandler)
{
    if (TaskHandler != NULL)
    {
        vTaskDelete(*TaskHandler);
        free(xTaskMQTTBuffer);
        free(xMQTTStack);
    }
    else
    {
        ESP_LOGE(TAG, "MQTT task does not delete");
    }
}

#ifdef MQTT_TEST

/**
 * @brief Test function for MQTT publish and subscribe.
 * This function is used for testing purposes to publish and subscribe to a test topic.
 */
void TestPublishAndSubscribe()
{
    MQTT_Data_str MQTT_Data;
    memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
    memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
    strcpy(MQTT_Data.Topic, "test_MQtt");
    char temp[] = R"EOF(
    This JSON structure includes information about a \n
    hypothetical company, its employees, products, partners, and some \n
    additional metadata. The content has been balanced to approach the target \n
    character count of 2400 characters. You can customize this JSON message \n
    further as needed!
    )EOF";
    strcpy(MQTT_Data.RawData, temp);
    MQTT_Subscribe(MQTT_Data.Topic);

    vTaskDelay(500);

    MQTT_Publish(MQTT_Data.Topic, MQTT_Data.RawData);

    vTaskDelay(500);
    memset(MQTT_Data.RawData, 0x0, sizeof(MQTT_Data.RawData));
    memset(MQTT_Data.Topic, 0x0, sizeof(MQTT_Data.Topic));
    if (xQueueReceive(MQTT_ThreadConnection.MQTT_BufQueue, &MQTT_Data, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
    {
        ESP_LOGE(TAG, "DATA is :%s", MQTT_Data.RawData);
    }
    vTaskDelay(1000);

    MQTT_UnSubscribe(MQTT_Data.Topic);

    vTaskDelay(500);

    MQTT_Disconnect();
}

/**
 * @brief Test function to receive and log MQTT data.
 * This function is used for testing purposes to receive data from the broker and log it.
 */
void test_MQtt()
{
    if (xSemaphoreTake((*MQTT_Interface.MQTT_Config.ConnectedSemaphore), 1) == pdTRUE)
    {
        char RawData[2500];
        memset(RawData, 0x0, 2500);
        if (xQueueReceive((*MQTT_Interface.MQTT_Config.DataFromBrokerQueue), RawData, 1) == pdTRUE)
        {
            ESP_LOGE(TAG, "DATA is :%s", RawData);
        }
    }
}
#endif
