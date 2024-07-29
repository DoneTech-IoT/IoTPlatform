#ifdef __cplusplus
extern "C" {
#endif
#ifndef MQTT_H_
#define MQTT_H_

#include "MQTT_Typedef.h"
#include "MQTT_Core.h"
#include "MQTT_Config.h"

#define MQTT_STACK 30*1000

#define SEC 1000

/**
 * @brief Main task function for managing MQTT operations.
 * This task initializes semaphores and queues for MQTT operations, establishes
 * a thread connection, and handles various states such as initialization,
 * connection, publishing, subscribing, and disconnection.
 * @param pvParameter Parameters passed to the task (currently unused).
 * @return void
 */
esp_err_t MQTT_Init(MQTT_Configuration_str *MQTT_InputConfigConfig);

/**
 * @brief Subscribes to a specified MQTT topic.
 * Attempts to subscribe to the given topic if the MQTT connection is active.
 * @param Topic The topic to subscribe to.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Subscribe(char *Topic);

/**
 * @brief Unsubscribes from a specified MQTT topic.
 * Attempts to unsubscribe from the given topic if the MQTT connection is active.
 * @param Topic The topic to unsubscribe from.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_UnSubscribe(char *Topic);

/**
 * @brief Publishes a message to a specified MQTT topic.
 * Publishes the given data to the specified topic if the MQTT connection is active.
 * @param Topic The topic to publish to.
 * @param Data The data to publish.
 * @return true if MQTT network connected was successful, false otherwise.
 */
bool MQTT_Publish(char *Topic, char *Data);

/**
 * @brief Disconnects from the MQTT broker.
 * This function handles the MQTT disconnection process.
 */
void MQTT_Disconnect();


// #define MQTT_TEST
#ifdef MQTT_TEST 
void TestPublishAndSubscribe();
void test_MQtt();
#endif
#endif
#ifdef __cplusplus
}
#endif