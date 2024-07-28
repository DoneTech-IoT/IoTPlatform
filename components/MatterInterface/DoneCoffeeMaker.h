#ifndef DONE_COFFEE_MAKER_H
#define DONE_COFFEE_MAKER_H

#include <esp_err.h>
#include <esp_matter.h>
#include "app_priv.h"

#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID 0x1001
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION 1

typedef enum {
    INCREMENT_MODE = 0,
    DECREMENT_MODE = 1,
    EXPLICIT_MODE = 2    
} LevelControlCurrentValueMode_t;

typedef enum {
    DONT_CARE = 0,
    GRINDER_MODE = 1,
    COFFEE_MODE = 2,
    TEA_MODE = 3    
} CookingMode_t;

typedef enum {
    STANDBY_MODE = 1,
    ON_MODE = 2,
    PAUSE_MODE = 3,    
} PowerKeyMode_t;

typedef enum {
    ERROR_MODE = 0,        
    NORMAL_MODE = 1    
} MicroSwitchMode_t;

/**
 * @brief This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerAttributeUpdate(
    app_driver_handle_t driver_handle,
    const uint16_t &endpoint_id, const uint32_t &cluster_id,
    const uint32_t &attribute_id, esp_matter_attr_val_t *val);

/** 
 * @brief create coffee maker device with its related endpoints
 * @param[in] node Endpoint0 or root Node.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerCreate(esp_matter::node_t* node);

#endif //DONE_COFFEE_MAKER_H