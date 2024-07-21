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

static void app_driver_LevelControlUpdateCurrentValue(
    LevelControlCurrentValueMode_t mode,
    const uint8_t &explicitCurrentValue,
    const uint16_t &endpoint_id)    
{    
    uint32_t cluster_id = LevelControl::Id;
    uint32_t attribute_id = LevelControl::Attributes::CurrentLevel::Id;
    uint32_t attMaxLevel_id = LevelControl::Attributes::MaxLevel::Id;
    uint32_t attMinLevel_id = LevelControl::Attributes::MinLevel::Id;

    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = cluster::get(endpoint, cluster_id);
    attribute_t *attribute = attribute::get(cluster, attribute_id);
    attribute_t *attMaxLevel = attribute::get(cluster, attMaxLevel_id);
    attribute_t *attMinLevel = attribute::get(cluster, attMinLevel_id);

    esp_matter_attr_val_t val_currentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MaxLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MinLevel = esp_matter_invalid(NULL);

    attribute::get_val(attribute, &val_currentLevel);
    attribute::get_val(attMaxLevel, &val_MaxLevel);
    attribute::get_val(attMinLevel, &val_MinLevel);

    if(mode == INCREMENT_MODE) {
        val_currentLevel.val.u8 = val_currentLevel.val.u8++;
        if(val_currentLevel.val.u8 > val_MaxLevel.val.u8)
            val_currentLevel.val.u8 = val_MinLevel.val.u8;
    }
    else if(mode == DECREMENT_MODE) {
        val_currentLevel.val.u8 = val_currentLevel.val.u8--;
        if(val_currentLevel.val.u8 < val_MinLevel.val.u8)
            val_currentLevel.val.u8 = val_MaxLevel.val.u8;
    }
    else if(mode == EXPLICIT_MODE) {
        val_currentLevel.val.u8 = explicitCurrentValue;        
    }

    attribute::update(endpoint_id, cluster_id, attribute_id, &val_currentLevel);    
}

static void app_driver_InitKeyWithPressCallback(
    button_handle_t handle,
    int32_t gpioPin,
    void(*callbackFunc)(void *button_handle, void *usr_data))
{
    button_config_t config = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = gpioPin,
            .active_level = CONFIG_DONE_KEY_ACTIVE_LEVEL,
        }
    };

    handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, callbackFunc, NULL);
}

static void app_driver_PowerKeyCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "app_driver_PowerKeyCB");
    uint16_t endpoint_id = powerKey_endpointID;
    uint32_t cluster_id = Switch::Id;
    uint32_t attribute_id = Switch::Attributes::CurrentLevel::Id;
    uint32_t attMaxLevel_id = Switch::Attributes::MaxLevel::Id;
    uint32_t attMinLevel_id = Switch::Attributes::MinLevel::Id;

    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = cluster::get(endpoint, cluster_id);
    attribute_t *attribute = attribute::get(cluster, attribute_id);
    attribute_t *attMaxLevel = attribute::get(cluster, attMaxLevel_id);
    attribute_t *attMinLevel = attribute::get(cluster, attMinLevel_id);

    esp_matter_attr_val_t val_currentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MaxLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MinLevel = esp_matter_invalid(NULL);

    attribute::get_val(attribute, &val_currentLevel);
    attribute::get_val(attMaxLevel, &val_MaxLevel);
    attribute::get_val(attMinLevel, &val_MinLevel);

    val_currentLevel.val.u8 = val_currentLevel.val.u8++;
    if(val_currentLevel.val.u8 > val_MaxLevel.val.u8)
        val_currentLevel.val.u8 = val_MinLevel.val.u8;

    attribute::update(endpoint_id, cluster_id, attribute_id, &val_currentLevel);    
}

static void app_driver_PowerKeyLongPressCB(void *arg, void *data)
{
    
}

static app_driver_handle_t app_driver_PowerKeyInit()
{
    app_driver_handle_t handle;
    button_handle_t btn_handle, btn1_handle;    

    app_driver_InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_POWER_KEY,
        app_driver_PowerKeyCB);

    iot_button_register_cb(btn_handle, 
        BUTTON_LONG_PRESS_START, 
        app_driver_PowerKeyLongPressCB, NULL);
    
    app_driver_InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_MICRO_SWITCH,
        app_driver_PowerKeyCB);

    return (app_driver_handle_t)handle;
}

static void app_driver_cookingModeGrindCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "app_driver_cookingModeGrindCB");
    app_driver_LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, grinder_endpointID);    
    app_driver_LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, GRINDER_MODE, cookingMode_endpointID);    

}

static void app_driver_CookingModeCoffeeCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "app_driver_CookingModeCoffeeCB");
    app_driver_LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, COFFEE_MODE, cookingMode_endpointID);        
}

static void app_driver_CookingModeTeaCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "app_driver_CookingModeTeaCB");
    app_driver_LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, TEA_MODE, cookingMode_endpointID);        
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
    ESP_LOGI(TAG, "app_driver_CupCounterKeyCB");
    app_driver_LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, cupCounter_endpointID);        
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
    done_MasterPower_key::config_t powerKey_config;
    cookingMode_config.on_off.on_off= true;    
    powerKey_config.switch.current_position = 1;  
    powerKey_config.switch.number_of_positions = 2;
    powerKey_config.  
    endpoint_t *powerKey_endpoint = done_MasterPower_key::create(node, &powerKey_config, ENDPOINT_FLAG_NONE, powerKey_handle);        
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