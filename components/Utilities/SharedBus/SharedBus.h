#ifndef SHARED_BUS_H
#define SHARED_BUS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include "PacketInfo.h"

typedef enum {
    NOT_ANY_ID = 0,
    SERVICE_MANAGER_INTERFACE_ID,
    UI_INTERFACE_ID,
    MATTER_INTERFACE_ID,
    MQTT_INTERFACE_ID,  
    LOG_INTERFACE_ID,    
    MAX_TASK_ID
} TaskInterfaceID_t;

typedef struct {
    uint8_t SourceID;
    uint8_t PacketID;    
    void *data;
} SharedBusPacket_t;

/**
 * @brief Initialize the SharedBus.
 * @param void This function have no parameter.
 * @return Always true.
 */
esp_err_t SharedBusInit(void);

/**
 * @brief Prepare needed Bits and send the Packet.
 * @param SahredBusPacket The Packet to publish on Bus.
 * @return True if queue is available, False if queue is busy.
 */
esp_err_t SharedBusSend(SharedBusPacket_t SharedBusPacket);

/**
 * @brief Check needed Bits to receive the Packet.
 * @param SharedBusPacket The received Packet.
 * @param interfaceID The ID of receiver component.
 * @return True if successfully received, false if receiver and sender are the same component.
 */
esp_err_t SharedBusReceive(
    SharedBusPacket_t *SharedBusPacket, 
    TaskInterfaceID_t interfaceID);    

/**
 * @brief Check all deamon run and confirm
 * @param interfaceID The ID of tasks.
 * @return True if task daemon were ran
 */
esp_err_t SharedBusTaskDaemonRunsConfirmed(    
    TaskInterfaceID_t interfaceID); 

/**
 * @brief get permission all task to continue if its daemon was ran before 
 * @return task id to run
 */
uint8_t SharedBusTaskContinuousPermission();

/**
 * @brief confirm the running of task body
 * @return nothing
 */
void SharedBusTaskContinuousConfirm();

#ifdef __cplusplus
}
#endif

#endif //SHARED_BUS_H
