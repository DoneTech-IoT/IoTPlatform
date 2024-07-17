#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_identify.h>
#include <esp_matter_endpoint.h>
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "app_priv.h"
#include "iot_button.h"

static const char *TAG = "DoneCoffeeMaker";
uint16_t powerKey_endpointID;
uint16_t cookingMode_endpointID;
uint16_t grinder_endpointID;
uint16_t cupCounter_endpointID;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

static void app_driver_InitKeyWithPressCallback(
    button_handle_t handle,
    int32_t gpioPin,
    void* callbackFunc)
{
    button_config_t config = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = gpioPin,
            .active_level = CONFIG_DONE_INPUT_ACTIVE_LEVEL,
        }
    };

    handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, callbackFunc, NULL);
}

static void app_driver_PowerKeyCB(void *arg, void *data)
{
    
}

static void app_driver_PowerKeyLongPressCB(void *arg, void *data)
{
    
}

static app_driver_handle_t app_driver_PowerKeyInit()
{
    app_driver_handle_t handle;
    button_handle_t btn_handle;    

    app_driver_InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_POWER_KEY,
        app_driver_PowerKeyCB);

    iot_button_register_cb(btn_handle, 
        BUTTON_LONG_PRESS_START, 
        app_driver_PowerKeyLongPressCB, NULL);

    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(powerKey_endpointID, newPosition);
    lock::chip_stack_unlock();
    
    return (app_driver_handle_t)handle;
}

static void app_driver_cookingModeGrindCB(void *arg, void *data)
{
    
}

static void app_driver_CookingModeCoffeeCB(void *arg, void *data)
{
    
}

static void app_driver_CookingModeTeaCB(void *arg, void *data)
{
    
}

static app_driver_handle_t app_driver_cookingModeInit()
{
    app_driver_handle_t handle;
    button_handle_t btn1_handle, btn2_handle, btn3_handle;    

    app_driver_InitKeyWithPressCallback(btn1_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_GRINDER,
        app_driver_cookingModeGrindCB);

    app_driver_InitKeyWithPressCallback(btn2_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_COFFEE,
        app_driver_CookingModeCoffeeCB);    

    app_driver_InitKeyWithPressCallback(btn3_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_TEA,
        app_driver_CookingModeTeaCB);        

    return (app_driver_handle_t)handle;
}

static void app_driver_CupCounterKeyCB(void *arg, void *data)
{
    
}

static app_driver_handle_t app_driver_cupCounterInit()
{
    app_driver_handle_t handle;
    button_handle_t btn_handle;    

    app_driver_InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_CUP_COUNTER_KEY,
        app_driver_CupCounterKeyCB);

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

    app_driver_handle_t powerKey_handle = app_driver_PowerKeyInit();    
    generic_switch::config_t powerKey_config;
    powerKey_config.
    powerKey_config.
    powerKey_config.
    powerKey_config.
    endpoint_t *powerKey_endpoint = generic_switch::create(node, &powerKey_config, ENDPOINT_FLAG_NONE, powerKey_handle);        
    if (!powerKey_endpoint)
    {
        ESP_LOGE(TAG, "powerKey_endpoint creation failed");
    }
    else 
    {
        powerKey_endpointID = endpoint::get_id(powerKey_endpoint);
        ESP_LOGI(TAG, "powerKey_endpoint created with endpoint_id %d", powerKey_endpointID);
    }

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
    cupCounter_config.level_control.current_level = 2;
    cupCounter_config.level_control.lighting.min_level = 2;
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