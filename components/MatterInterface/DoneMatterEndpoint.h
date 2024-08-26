#ifndef DONE_MATTER_ENDPOINT_H
#define DONE_MATTER_ENDPOINT_H

#include <esp_matter_cluster.h>
#include <esp_matter_core.h>
#include <esp_matter_identify.h>

//SEZ@Done: we define custum ID 
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID 0x1000
#define ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION 1

namespace esp_matter {
namespace endpoint {
namespace done_coffee_maker {
typedef struct config {
    cluster::descriptor::config_t descriptor;
    cluster::identify::config_t identify;
    cluster::groups::config_t groups;
    cluster::scenes::config_t scenes;
    cluster::on_off::config_t on_off;
    cluster::level_control::config_t level_control;
    cluster::temperature_measurement::config_t temperature_measurement;
    cluster::temperature_control::config_t temperature_control;
} config_t;

uint32_t get_device_type_id();
uint8_t get_device_type_version();
endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data);
esp_err_t add(endpoint_t *endpoint, config_t *config);
} /* coffee_maker */    

} /* endpoint */
} /* esp_matter */

#endif //DONE_MATTER_ENDPOINT_H