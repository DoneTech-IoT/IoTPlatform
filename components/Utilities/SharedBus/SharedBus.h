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

typedef enum {
    UI_INTERFACE_ID = 1,
    MATTER_INTERFACE_ID = 2,
    MQTT_INTERFACE_ID = 3,  
    LOG_INTERFACE_ID = 4,
} TaskInterfaceID_t;

typedef struct {
    uint8_t SourceID;
    uint8_t PacketID;    
    void *data;
} SharedBusPacket_t;

/**
 * @brief Initialize the SharedBus.
 * @return Always true.
 */
esp_err_t SharedBusInit();

/**
 * @brief Prepare needed Bits and send the Packet.
 * @return True if queue is available, False if queue is busy.
 */
esp_err_t SharedBusSend(SharedBusPacket_t SharedBusPacket);

/**
 * @brief Check needed Bits to receive the Packet.
 * @return True if successfully received, false if receiver and sender are the same component.
 */
esp_err_t SharedBusRecieve(
    SharedBusPacket_t *SharedBusPacket, 
    TaskInterfaceID_t interfaceID);    

#ifdef __cplusplus
}
#endif

#endif //SHARED_BUS_H
