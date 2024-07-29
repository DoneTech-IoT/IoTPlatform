#include <esp_matter_core.h>
#include <esp_matter_cluster.h>
#include <esp_matter_identify.h>
#include <esp_matter_endpoint.h>
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "app_priv.h"
#include "iot_button.h"
#include "Buzzer.h"

static const char *TAG = "DoneCoffeeMaker";
uint16_t powerKey_endpointID;
uint16_t cookingMode_endpointID;
uint16_t grinder_endpointID;
uint16_t cupCounter_endpointID;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

/**
 * @brief read value of an attribute 
 * @param[in] endpoint_id Endpoint ID of the attribute.
 * @param[in] cluster_id Cluster ID of the attribute.
 * @param[in] attribute_id Attribute ID of the attribute.
 * @param[in] attr_val Pointer to `esp_matter_attr_val_t`. 
 * Use appropriate elements as per the value type.
 */
static void GetAttributeValue(
    const uint16_t &endpoint_id,
    const uint32_t &cluster_id,
    const uint32_t &attribute_id,
    esp_matter_attr_val_t *attr_val)
{
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, endpoint_id);
    cluster_t *cluster = cluster::get(endpoint, cluster_id);
    attribute_t *attribute = attribute::get(cluster, attribute_id);

    attribute::get_val(attribute, attr_val);
}

/**
 * @brief change levelControl current value attr.
 * @param[in] mode mode of changing current value attr.
 * @param[in] explicitCurrentValue get direct value to currentValue Attr.
 * @param[in] endpoint_id Attribute ID of the attribute.
 */
static void LevelControlUpdateCurrentValue(
    const LevelControlCurrentValueMode_t &mode,
    const uint8_t &explicitCurrentValue,
    const uint16_t &endpoint_id)    
{    
    esp_matter_attr_val_t val_currentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MaxLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_MinLevel = esp_matter_invalid(NULL);

    GetAttributeValue(
        endpoint_id,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &val_currentLevel);

    GetAttributeValue(
        endpoint_id,
        LevelControl::Id,
        LevelControl::Attributes::MaxLevel::Id,
        &val_MaxLevel);

    GetAttributeValue(
        endpoint_id,
        LevelControl::Id,
        LevelControl::Attributes::MinLevel::Id,
        &val_MinLevel);     
    
    if(mode == INCREMENT_MODE) {
        val_currentLevel.val.u8 += 1;
        if(val_currentLevel.val.u8 > val_MaxLevel.val.u8)
            val_currentLevel.val.u8 = val_MinLevel.val.u8;
    }
    else if(mode == DECREMENT_MODE) {
        val_currentLevel.val.u8 -= 1;
        if(val_currentLevel.val.u8 < val_MinLevel.val.u8)
            val_currentLevel.val.u8 = val_MaxLevel.val.u8;
    }
    else if(mode == EXPLICIT_MODE) {
        val_currentLevel.val.u8 = explicitCurrentValue;        
    }

    attribute::update(endpoint_id, 
        LevelControl::Id, 
        LevelControl::Attributes::CurrentLevel::Id, 
        &val_currentLevel);    
}

/**
 * @brief init a key and register Press callback
 * @param[in] handle handler
 * @param[in] gpioPin gpio pin for button 
 * @param[in] callbackFunc callback for Press event 
 */
static void InitKeyWithPressCallback(
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

static void MicroSwitchCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "MicroSwitchCB");    
    esp_matter_attr_val_t attr_val = esp_matter_invalid(NULL);
    GetAttributeValue(
        powerKey_endpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &attr_val);                     
    attr_val.val.b = !attr_val.val.b;

    attribute::update(powerKey_endpointID, BooleanState::Id, 
        BooleanState::Attributes::StateValue::Id, &attr_val);
}

static void PowerKeyCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "PowerKeyCB");    

    esp_matter_attr_val_t val_currentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_boolean = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_onOff = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_cookingMode = esp_matter_invalid(NULL);

    GetAttributeValue(
        powerKey_endpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &val_currentLevel);

    GetAttributeValue(
        powerKey_endpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &val_boolean);              

    GetAttributeValue(
        powerKey_endpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        &val_onOff);              

    GetAttributeValue(
        cookingMode_endpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &val_cookingMode);

    if(val_onOff.val.b) //when powerON
    {        
        if (val_boolean.val.b == ERROR_MODE)
        {
            ESP_LOGI(TAG, "MicroSwitch Error");           
            BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
        }
        else 
        {
            LevelControlUpdateCurrentValue(
                EXPLICIT_MODE, ON_MODE, powerKey_endpointID);    
            
            //To Do : //register powerKeyHold callback

            switch (val_cookingMode.val.u8)
            {                
                case GRINDER_MODE:
                {
                    ESP_LOGI(TAG, "GRINDER_MODE");                    
                    ESP_LOGI(TAG, "TurnOn GrinderMotor Actuator");
                    break;
                }
                case COFFEE_MODE:
                {
                    ESP_LOGI(TAG, "COFFEE_MODE");                       
                    ESP_LOGI(TAG, "TurnOn Water Actuator");
                    ESP_LOGI(TAG, "TurnOn Heater");        
                    ESP_LOGI(TAG, "TurnOn Timer for Coffee");        
                    break;
                }
                case TEA_MODE:
                {
                    ESP_LOGI(TAG, "TEA_MODE");    
                    ESP_LOGI(TAG, "TurnOn Water Actuator");
                    ESP_LOGI(TAG, "TurnOn Heater");        
                    ESP_LOGI(TAG, "TurnOn Timer for Tea");               
                    break;
                }                        
                default:
                    break;
            }
        }        
    }
    else //when powerOFF  
    {   
        ESP_LOGI(TAG, "Power On");       
        val_onOff.val.b = true;
        attribute::update(powerKey_endpointID, 
            OnOff::Id,
            OnOff::Attributes::OnOff::Id,
            &val_onOff);    

        BuzzerPlay(BuzzerEffect_t::ON_BIZ);
        //to Do
        //turn On LCD                                     
    } 
}

static void PowerKeyLongPressCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "PowerKeyLongPressCB");    

    esp_matter_attr_val_t val_currentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_boolean = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_onOff = esp_matter_invalid(NULL);
    esp_matter_attr_val_t val_cookingMode = esp_matter_invalid(NULL);

    GetAttributeValue(
        powerKey_endpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &val_currentLevel);

    GetAttributeValue(
        powerKey_endpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &val_boolean);              

    GetAttributeValue(
        powerKey_endpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        &val_onOff);              

    GetAttributeValue(
        cookingMode_endpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &val_cookingMode);

    if (val_boolean.val.b == ERROR_MODE)
    {
        ESP_LOGI(TAG, "MicroSwitch Error");           
        BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
    }    
    else 
    {
        if(val_currentLevel.val.u8 == ON_MODE)
        {

        }
    }
}

static app_driver_handle_t PowerKeyInit()
{
    app_driver_handle_t handle;
    button_handle_t btn_handle;    

    InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_POWER_KEY,
        PowerKeyCB);

    iot_button_register_cb(btn_handle, 
        BUTTON_LONG_PRESS_START, 
        PowerKeyLongPressCB, NULL);
    
    InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_MICRO_SWITCH,
        MicroSwitchCB);

    return (app_driver_handle_t)handle;
}

static void CookingModeGrindCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "CookingModeGrindCB");
    LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, grinder_endpointID);    
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, GRINDER_MODE, cookingMode_endpointID);    
}

static void CookingModeCoffeeCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "CookingModeCoffeeCB");
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, COFFEE_MODE, cookingMode_endpointID);        
}

static void CookingModeTeaCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "CookingModeTeaCB");
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, TEA_MODE, cookingMode_endpointID);        
}

static app_driver_handle_t CookingModeInit()
{
    app_driver_handle_t handle;
    button_handle_t btn1_handle, btn2_handle, btn3_handle;    

    InitKeyWithPressCallback(btn1_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_GRINDER,
        CookingModeGrindCB);

    InitKeyWithPressCallback(btn2_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_COFFEE,
        CookingModeCoffeeCB);    

    InitKeyWithPressCallback(btn3_handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_TEA,
        CookingModeTeaCB);        

    return (app_driver_handle_t)handle;
}

static void CupCounterKeyCB(void *arg, void *data)
{
    ESP_LOGI(TAG, "CupCounterKeyCB");
    LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, cupCounter_endpointID);        
}

static app_driver_handle_t CupCounterInit()
{
    app_driver_handle_t handle;
    button_handle_t btn_handle;    

    InitKeyWithPressCallback(btn_handle, 
        CONFIG_DONE_COFFEE_MAKER_CUP_COUNTER_KEY,
        CupCounterKeyCB);

    return (app_driver_handle_t)handle;
}

esp_err_t CookingModeSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t GrinderSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t CupCounterSetEnable(
    app_driver_handle_t driver_handle, 
    esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    return err;
}

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
    const uint32_t &attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;

    if (endpoint_id == powerKey_endpointID) {
        if (cluster_id == OnOff::Id) {            
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                //err = app_driver_light_set_power(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterOnOff_id %ld, attributeOnOff_id %ld", 
                powerKey_endpointID, cluster_id, attribute_id);
            }        
        }
    }
    else if (endpoint_id == cookingMode_endpointID) {        
    }
    else if (endpoint_id == grinder_endpointID) {        
    }
    else if (endpoint_id == cupCounter_endpointID) {        
        //led_driver_handle_t handle = (led_driver_handle_t)driver_handle;
        if (cluster_id == OnOff::Id) {            
            if (attribute_id == OnOff::Attributes::OnOff::Id) {
                //err = app_driver_light_set_power(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterOnOff_id %ld, attributeOnOff_id %ld", 
                powerKey_endpointID, cluster_id, attribute_id);
            }
            
        } else if (cluster_id == LevelControl::Id) {
            if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterLevelControl_id %ld, attributeCurrentLevel_id %ld", powerKey_endpointID, cluster_id, attribute_id);
            }
            else if (attribute_id == LevelControl::Attributes::MinLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterLevelControl_id %ld, attributeMinLevel_id %ld", powerKey_endpointID, cluster_id, attribute_id);
            }
            else if (attribute_id == LevelControl::Attributes::MaxLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterLevelControl_id %ld, attributeMaxLevel_id %ld", powerKey_endpointID, cluster_id, attribute_id);
            }
            else if (attribute_id == LevelControl::Attributes::MaxLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "endpoint_id %d, clusterLevelControl_id %ld, attributeMaxLevel_id %ld", powerKey_endpointID, cluster_id, attribute_id);
            }
        }
    }

    return err;
}

/** 
 * @brief create coffee maker device with its related endpoints
 * @param[in] node Endpoint0 or root Node.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerCreate(node_t* node)
{
    esp_err_t err = ESP_OK;

    app_driver_handle_t powerKey_handle = PowerKeyInit();    
    done_MasterPowerKey::config_t powerKey_config;
    powerKey_config.on_off.on_off = false;    
    powerKey_config.boolean.state_value = true;//MicroSwitchMode_t::NORMAL_MODE  
    powerKey_config.level_control.current_level = 1;
    powerKey_config.level_control.lighting.min_level = 1;
    powerKey_config.level_control.lighting.max_level = 3;      
    endpoint_t *powerKey_endpoint = done_MasterPowerKey::create(
        node, &powerKey_config,    
        ENDPOINT_FLAG_NONE, powerKey_handle);                
    if (!powerKey_endpoint)
    {
        ESP_LOGE(TAG, "powerKey_endpoint creation failed");
    }
    else 
    {
        powerKey_endpointID = endpoint::get_id(powerKey_endpoint);
        ESP_LOGI(TAG, "powerKey_endpoint created with endpoint_id %d", powerKey_endpointID);
    }

    app_driver_handle_t cookingMode_handle = CookingModeInit();    
    done_MultiFunctionSwitch::config_t cookingMode_config;
    cookingMode_config.on_off.on_off= true;
    cookingMode_config.level_control.current_level = 1;
    cookingMode_config.level_control.lighting.min_level = 1;
    cookingMode_config.level_control.lighting.max_level = 3;
    endpoint_t *cookingMode_endpoint = done_MultiFunctionSwitch::create(node, &cookingMode_config, ENDPOINT_FLAG_NONE, cookingMode_handle);
    if (!cookingMode_endpoint)
    {
        ESP_LOGE(TAG, "cookingMode_endpoint creation failed");
    }
    else 
    {
        cookingMode_endpointID = endpoint::get_id(cookingMode_endpoint);
        ESP_LOGI(TAG, "cookingMode_endpoint created with endpoint_id %d", cookingMode_endpointID);
    }

    done_MultiFunctionSwitch::config_t grinder_config;
    grinder_config.on_off.on_off= true;        
    grinder_config.level_control.current_level = 1;
    grinder_config.level_control.lighting.min_level = 1;
    grinder_config.level_control.lighting.max_level = 3;
    endpoint_t *grinder_endpoint = done_MultiFunctionSwitch::create(node, &grinder_config,         ENDPOINT_FLAG_NONE, NULL);
    if (!grinder_endpoint)
    {
        ESP_LOGE(TAG, "grinder_endpoint creation failed");
    }
    else 
    {
        grinder_endpointID = endpoint::get_id(grinder_endpoint);
        ESP_LOGI(TAG, "grinder_endpoint created with endpoint_id %d", grinder_endpointID);
    }

    app_driver_handle_t cupCounter_handle = CupCounterInit();
    done_MultiFunctionSwitch::config_t cupCounter_config;
    cupCounter_config.on_off.on_off= true;        
    cupCounter_config.level_control.current_level = 2;
    cupCounter_config.level_control.lighting.min_level = 2;
    cupCounter_config.level_control.lighting.max_level = 7;
    endpoint_t *cupCounter_endpoint = done_MultiFunctionSwitch::create(node, &cupCounter_config, ENDPOINT_FLAG_NONE, cupCounter_handle);
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