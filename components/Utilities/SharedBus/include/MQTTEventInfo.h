#ifndef MQTT_EVENT_INFO_H
#define MQTT_EVENT_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    BROKER_CONNECT,
    BROKER_DISCONNECT,
} MQTTEventTypes;

typedef struct 
{    
    MQTTEventTypes MQTTEventTypes;
} MQTTEventPacket;

#ifdef __cplusplus
}
#endif

#endif //MQTT_EVENT_INFO_H