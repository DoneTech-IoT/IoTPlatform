#ifndef PACKET_IFO_H
#define PACKET_IFO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "CoffeeMakerInfo.h"
#include "MatterEventInfo.h"

typedef enum {
    COFFEE_MAKER_PACKET_ID = 1,
    MATTER_EVENT_PACKET_ID = 2,
} PacketIDs;

#ifdef __cplusplus
}
#endif

#endif //PACKET_IFO_H