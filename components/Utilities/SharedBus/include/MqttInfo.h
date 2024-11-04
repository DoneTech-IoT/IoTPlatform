#ifndef MQTT_INFO_H
#define MQTT_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

// Define constants for topic length and raw data length
#define TOPIC_LEN 50
#define MQTT_RAW_DATA_LEN 2500

typedef struct
{
    char Topic[TOPIC_LEN];
    char CoreRawData[MQTT_RAW_DATA_LEN];
} MqttPacket;

#ifdef __cplusplus
}
#endif

#endif //MQTT_INFO_H