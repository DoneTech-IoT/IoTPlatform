#ifndef DONE_MATTER_ENDPOINT_H
#define DONE_MATTER_ENDPOINT_H

#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_identify.h>

//SEZ@Done: we define custom ID 
typedef enum
{
    ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID = 0x1000,
    ESP_MATTER_DONE_MULTI_FUNCTION_SWITCH_DEVICE_TYPE_ID = 0x1001,    
    ESP_MATTER_DONE_COOKING_MODE_SWITCH_DEVICE_TYPE_ID = 0x1002, 
    ESP_MATTER_DONE_GRINDER_SWITCH_DEVICE_TYPE_ID = 0x1003,
    ESP_MATTER_DONE_CUP_COUNTER_DEVICE_TYPE_ID = 0x1004,
} DeviceTypeID_t;

typedef enum  
{
    ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION = 1,
    
} DeviceTypeVersion_t;

namespace esp_matter {
namespace endpoint {

namespace done_multiFunction_switch {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;        
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding; 
    typedef uint32_t done_endpointType_t;
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config, done_endpointType_t type);
}//done_multiFunction_switch

namespace done_cookingMode_switch {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;        
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding;        
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_cookingMode_switch

namespace done_grinder_switch {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;        
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding;        
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_grinder_switch

namespace done_cupCounter_switch {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;        
    cluster::level_control::config_t level_control;
    cluster::binding::config_t binding;        
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_cupCounter_switch

namespace done_cooking_procedure {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;        
    cluster::level_control::config_t level_control;
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
}//done_cooking_procedure

namespace done_coffee_maker {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;
    cluster::groups::config_t groups;
    cluster::scenes::config_t scenes;
    
    cluster::level_control::config_t power;
    cluster::level_control::config_t cooking_mode;
    cluster::level_control::config_t grinder;
    cluster::level_control::config_t cup_counter;
    cluster::level_control::config_t cooking_procedure;
    // cluster::temperature_measurement::config_t temperature_measurement;
    // cluster::temperature_control::config_t temperature_control;
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
} /* coffee_maker */    

} /* endpoint */
} /* esp_matter */

#endif //DONE_MATTER_ENDPOINT_H