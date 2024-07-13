#ifndef DONE_COFFEE_MAKER_H
#define DONE_COFFEE_MAKER_H

#include <esp_err.h>
#include <esp_matter.h>

#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID 0x1001
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION 1

static uint16_t cookingMode_endpointID = 0;
static uint16_t grinder_endpointID = 0;
static uint16_t cupCounter_endpointID = 0;

esp_err_t create_DoneCoffeeMaker(esp_matter::node_t* node);

#endif //DONE_COFFEE_MAKER_H