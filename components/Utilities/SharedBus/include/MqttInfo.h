#ifndef MQTT_INFO_H
#define MQTT_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct
{
    char CoreRawData[MQTT_RAW_DATA_LEN];
} MqttPacket;

#ifdef __cplusplus
}
#endif

#endif //MQTT_INFO_H