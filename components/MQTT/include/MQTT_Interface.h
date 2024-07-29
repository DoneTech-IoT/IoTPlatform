#ifdef __cplusplus
extern "C" {
#endif
#ifndef MQTT_H_
#define MQTT_H_

#include "MQTT_Typedef.h"
#include "MQTT_Core.h"
#include "MQTT_Config.h"

#define DATA_TEST "bib bib bib"
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
bool MQTT_Subscribe(char *Topic);
bool MQTT_UnSubscribe(char *Topic);
bool MQTT_Publish(char *Topic, char *Data);
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