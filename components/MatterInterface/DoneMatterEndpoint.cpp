#include <esp_log.h>
#include <esp_matter.h>
#include <esp_matter_endpoint.h>

#include "DoneMatterEndpoint.h"
static const char *TAG = "DoneMatterEndpoint";

namespace esp_matter {
using namespace cluster;

namespace endpoint {
namespace done_multiFunction_switch {
uint32_t get_device_type_id()
{
    return ESP_MATTER_DONE_MULTI_FUNCTION_SWITCH_DEVICE_TYPE_ID;
}

uint8_t get_device_type_version()
{
    return ESP_MATTER_DONE_MULTI_FUNCTION_SWITCH_DEVICE_TYPE_VERSION;
}

endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data)
{
    endpoint_t *endpoint = endpoint::create(node, flags, priv_data);
    add(endpoint, config);
    return endpoint;
}

esp_err_t add(endpoint_t *endpoint, config_t *config)
{
    if (!endpoint) {
        ESP_LOGE(TAG, "Endpoint cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t err = add_device_type(endpoint, get_device_type_id(), get_device_type_version());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device type id:%" PRIu32 ",err: %d", get_device_type_id(), err);
        return err;
    }

    descriptor::create(endpoint, &(config->descriptor), CLUSTER_FLAG_SERVER);
    cluster_t *identify_cluster = identify::create(endpoint, &(config->identify), CLUSTER_FLAG_SERVER);
    identify::command::create_trigger_effect(identify_cluster);    
    binding::create(endpoint, &(config->binding), CLUSTER_FLAG_SERVER);
    level_control::create(endpoint, &(config->level_control), CLUSTER_FLAG_SERVER,
                          level_control::feature::on_off::get_id());

    // descriptor::create(endpoint, &(config->descriptor), CLUSTER_FLAG_SERVER);
    // identify::create(endpoint, &(config->identify), CLUSTER_FLAG_SERVER | CLUSTER_FLAG_CLIENT);
    // binding::create(endpoint, &(config->binding), CLUSTER_FLAG_SERVER);
    // on_off::create(endpoint, NULL, CLUSTER_FLAG_CLIENT, ESP_MATTER_NONE_FEATURE_ID);
    // level_control::create(endpoint, NULL, CLUSTER_FLAG_CLIENT, ESP_MATTER_NONE_FEATURE_ID);

    return ESP_OK;
}    
}//done_multiFunction_switch

namespace done_coffee_maker {
uint32_t get_device_type_id()
{
    return ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_ID;
}

uint8_t get_device_type_version()
{
    return ESP_MATTER_DONE_COFFEE_MAKER_DEVICE_TYPE_VERSION;
}

endpoint_t *create(node_t *node, config_t *config, uint8_t flags, void *priv_data)
{
    endpoint_t *endpoint = endpoint::create(node, flags, priv_data);
    add(endpoint, config);
    return endpoint;
}

esp_err_t add(endpoint_t *endpoint, config_t *config)
{
    if (!endpoint) {
        ESP_LOGE(TAG, "Endpoint cannot be NULL");
        return ESP_ERR_INVALID_ARG;
    }
    esp_err_t err = add_device_type(endpoint, get_device_type_id(), get_device_type_version());
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device type id:%" PRIu32 ",err: %d", get_device_type_id(), err);
        return err;
    }

    descriptor::create(endpoint, &(config->descriptor), CLUSTER_FLAG_SERVER);
    cluster_t *identify_cluster = identify::create(endpoint, &(config->identify), CLUSTER_FLAG_SERVER);
    identify::command::create_trigger_effect(identify_cluster);
    groups::create(endpoint, &(config->groups), CLUSTER_FLAG_SERVER);
    scenes::create(endpoint, &(config->scenes), CLUSTER_FLAG_SERVER);

    level_control::create(endpoint, &(config->power), CLUSTER_FLAG_SERVER,
                          level_control::feature::on_off::get_id());

    //level_control::create(endpoint, &(config->cooking_mode), CLUSTER_FLAG_SERVER,NULL);
    // level_control::create(endpoint, &(config->grinder), CLUSTER_FLAG_SERVER,NULL);
    // level_control::create(endpoint, &(config->cup_counter), CLUSTER_FLAG_SERVER,NULL);
    // level_control::create(endpoint, &(config->cooking_procedure), CLUSTER_FLAG_SERVER,NULL);

    // temperature_measurement::create(endpoint, &(config->temperature_measurement), CLUSTER_FLAG_SERVER);
    // temperature_control::create(endpoint, &(config->temperature_control), CLUSTER_FLAG_SERVER, ESP_MATTER_NONE_FEATURE_ID);

    return ESP_OK;
}
} /* done_coffee_maker */
} /* endpoint */
} /* esp_matter */