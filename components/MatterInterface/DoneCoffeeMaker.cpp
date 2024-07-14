#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_identify.h>
#include <esp_matter_endpoint.h>
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "app_priv.h"

static const char *TAG = "DoneCoffeeMaker";
uint16_t cookingMode_endpointID;
uint16_t grinder_endpointID;
uint16_t cupCounter_endpointID;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

static app_driver_handle_t app_driver_cookingModeInit()
{
    app_driver_handle_t handle;

    return (app_driver_handle_t)handle;
}

static app_driver_handle_t app_driver_grinderInit()
{
    app_driver_handle_t handle;

    return (app_driver_handle_t)handle;
}

static app_driver_handle_t app_driver_cupCounterInit()
{
    app_driver_handle_t handle;

    return (app_driver_handle_t)handle;
}

esp_err_t app_driver_cookingModeSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t app_driver_grinderSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t app_driver_cupCounterSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t create_DoneCoffeeMaker(node_t* node)
{
    esp_err_t err = ESP_OK;

    app_driver_handle_t cookingMode_handle = app_driver_cookingModeInit();    
    done_multiFunction_switch::config_t cookingMode_config;
    cookingMode_config.on_off.on_off= true;
    cookingMode_config.level_control.current_level = 1;
    cookingMode_config.level_control.lighting.min_level = 1;
    cookingMode_config.level_control.lighting.max_level = 3;
    endpoint_t *cookingMode_endpoint = done_multiFunction_switch::create(node, &cookingMode_config, ENDPOINT_FLAG_NONE, cookingMode_handle);

    if (!cookingMode_endpoint)
    {
        ESP_LOGE(TAG, "cookingMode_endpoint creation failed");
    }
    else 
    {
        cookingMode_endpointID = endpoint::get_id(cookingMode_endpoint);
        ESP_LOGI(TAG, "cookingMode_endpoint created with endpoint_id %d", cookingMode_endpointID);
    }

    app_driver_handle_t grinder_handle = app_driver_grinderInit();
    done_multiFunction_switch::config_t grinder_config;
    grinder_config.on_off.on_off= true;        
    grinder_config.level_control.current_level = 1;
    grinder_config.level_control.lighting.min_level = 1;
    grinder_config.level_control.lighting.max_level = 3;
    endpoint_t *grinder_endpoint = done_multiFunction_switch::create(node, &grinder_config, ENDPOINT_FLAG_NONE, grinder_handle);

    if (!grinder_endpoint)
    {
        ESP_LOGE(TAG, "grinder_endpoint creation failed");
    }
    else 
    {
        grinder_endpointID = endpoint::get_id(grinder_endpoint);
        ESP_LOGI(TAG, "grinder_endpoint created with endpoint_id %d", grinder_endpointID);
    }

    app_driver_handle_t cupCounter_handle = app_driver_cupCounterInit();
    done_multiFunction_switch::config_t cupCounter_config;
    cupCounter_config.on_off.on_off= true;        
    cupCounter_config.level_control.current_level = 1;
    cupCounter_config.level_control.lighting.min_level = 1;
    cupCounter_config.level_control.lighting.max_level = 6;
    endpoint_t *cupCounter_endpoint = done_multiFunction_switch::create(node, &cupCounter_config, ENDPOINT_FLAG_NONE, cupCounter_handle);

    if (!cupCounter_endpoint)
    {
        ESP_LOGE(TAG, "cupCounter_endpoint creation failed");
    }
    else 
    {
        cupCounter_endpointID = endpoint::get_id(cupCounter_endpoint);
        ESP_LOGI(TAG, "grinder_endpoint created with endpoint_id %d", cupCounter_endpointID);
    }

    return err;
}