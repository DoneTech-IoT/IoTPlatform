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
uint16_t PowerKeyEndpointID;
uint16_t CookingModeEndpointID;
uint16_t GrinderEndpointID;
uint16_t CupCounterEndpointID;

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

/**
 * @brief read value of an attribute 
 * @param[in] EndpointID Endpoint ID of the attribute.
 * @param[in] ClusterID Cluster ID of the attribute.
 * @param[in] AttributeID Attribute ID of the attribute.
 * @param[in] AttrVal Pointer to `esp_matter_attr_val_t`. 
 * Use appropriate elements as per the value type.
 */
static void GetAttributeValue(
    const uint16_t &EndpointID,
    const uint32_t &ClusterID,
    const uint32_t &AttributeID,
    esp_matter_attr_val_t *AttrVal)
{
    node_t *node = node::get();
    endpoint_t *endpoint = endpoint::get(node, EndpointID);
    cluster_t *cluster = cluster::get(endpoint, ClusterID);
    attribute_t *attribute = attribute::get(cluster, AttributeID);

    attribute::get_val(attribute, AttrVal);
}

/**
 * @brief change levelControl current value attr.
 * @param[in] Mode mode of changing current value attr.
 * @param[in] ExplicitCurrentValue get direct value to currentValue Attr.
 * @param[in] EndpointID Attribute ID of the attribute.
 */
static void LevelControlUpdateCurrentValue(
    const LevelControlCurrentValueMode_t &Mode,
    const uint8_t &ExplicitCurrentValue,
    const uint16_t &EndpointID)    
{    
    esp_matter_attr_val_t valCurrentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valMaxLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valMinLevel = esp_matter_invalid(NULL);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCurrentLevel);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::MaxLevel::Id,
        &valMaxLevel);

    GetAttributeValue(
        EndpointID,
        LevelControl::Id,
        LevelControl::Attributes::MinLevel::Id,
        &valMinLevel);     
    
    if(Mode == INCREMENT_MODE) {
        valCurrentLevel.val.u8 += 1;
        if(valCurrentLevel.val.u8 > valMaxLevel.val.u8)
            valCurrentLevel.val.u8 = valMinLevel.val.u8;
    }
    else if(Mode == DECREMENT_MODE) {
        valCurrentLevel.val.u8 -= 1;
        if(valCurrentLevel.val.u8 < valMinLevel.val.u8)
            valCurrentLevel.val.u8 = valMaxLevel.val.u8;
    }
    else if(Mode == EXPLICIT_MODE) {
        valCurrentLevel.val.u8 = ExplicitCurrentValue;        
    }

    attribute::update(EndpointID, 
        LevelControl::Id, 
        LevelControl::Attributes::CurrentLevel::Id, 
        &valCurrentLevel);    
}

/**
 * @brief init a key and register Press callback
 * @param[in] Handle handler
 * @param[in] GpioPin gpio pin for button 
 * @param[in] callbackFunc callback for Press event 
 */
static void InitKeyWithPressCallback(
    button_handle_t Handle,
    int32_t GpioPin,
    void(*CallbackFunc)(void *button_handle, void *usr_data))
{
    button_config_t config = {
        .type = BUTTON_TYPE_GPIO,
        .gpio_button_config = {
            .gpio_num = GpioPin,
            .active_level = CONFIG_DONE_KEY_ACTIVE_LEVEL,
        }
    };

    Handle = iot_button_create(&config);
    iot_button_register_cb(Handle, BUTTON_PRESS_DOWN, CallbackFunc, NULL);
}

static void MicroSwitchCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "MicroSwitchCB");    
    esp_matter_attr_val_t MicroSwitchState = esp_matter_invalid(NULL);        
        
    GetAttributeValue(
        PowerKeyEndpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &MicroSwitchState);                     
    MicroSwitchState.val.b = !MicroSwitchState.val.b;
    if(MicroSwitchState.val.b == ERROR_MODE)
    {
        ESP_LOGI(TAG, "MicroSwitch Error Intrrupt");           
        BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);   

        LevelControlUpdateCurrentValue(
            EXPLICIT_MODE, PAUSE_MODE, PowerKeyEndpointID);     
    }

    attribute::update(PowerKeyEndpointID, 
        BooleanState::Id, 
        BooleanState::Attributes::StateValue::Id, 
        &MicroSwitchState);
}

static void PowerKeyCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "PowerKeyCB");    

    esp_matter_attr_val_t valCurrentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valBoolean = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valOnOff = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valCookingMode = esp_matter_invalid(NULL);

    GetAttributeValue(
        PowerKeyEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCurrentLevel);

    GetAttributeValue(
        PowerKeyEndpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &valBoolean);              

    GetAttributeValue(
        PowerKeyEndpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        &valOnOff);              

    GetAttributeValue(
        CookingModeEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCookingMode);

    if(valOnOff.val.b) //when powerON
    {        
        if (valBoolean.val.b == ERROR_MODE)
        {
            ESP_LOGI(TAG, "MicroSwitch Error");           
            BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
        }
        else 
        {
            LevelControlUpdateCurrentValue(
                EXPLICIT_MODE, ON_MODE, PowerKeyEndpointID);    
            
            //To Do : //register powerKeyHold callback

            switch (valCookingMode.val.u8)
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
        valOnOff.val.b = true;
        attribute::update(PowerKeyEndpointID, 
            OnOff::Id,
            OnOff::Attributes::OnOff::Id,
            &valOnOff);    

        BuzzerPlay(BuzzerEffect_t::ON_BIZ);
        //to Do
        //turn On LCD                                     
    } 
}

static void PowerKeyLongPressCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "PowerKeyLongPressCB");    

    esp_matter_attr_val_t valCurrentLevel = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valBoolean = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valOnOff = esp_matter_invalid(NULL);
    esp_matter_attr_val_t valCookingMode = esp_matter_invalid(NULL);

    GetAttributeValue(
        PowerKeyEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCurrentLevel);

    GetAttributeValue(
        PowerKeyEndpointID,
        BooleanState::Id,
        BooleanState::Attributes::StateValue::Id,
        &valBoolean);              

    GetAttributeValue(
        PowerKeyEndpointID,
        OnOff::Id,
        OnOff::Attributes::OnOff::Id,
        &valOnOff);              

    GetAttributeValue(
        CookingModeEndpointID,
        LevelControl::Id,
        LevelControl::Attributes::CurrentLevel::Id,
        &valCookingMode);

    if (valBoolean.val.b == ERROR_MODE)
    {
        ESP_LOGI(TAG, "MicroSwitch Error");           
        BuzzerPlay(BuzzerEffect_t::TRIPLE_BIZ);
    }    
    else 
    {
        if(valCurrentLevel.val.u8 == ON_MODE)
        {

        }
    }
}

static app_driver_handle_t PowerKeyInit()
{
    app_driver_handle_t handle;
    button_handle_t btnHandle;    

    InitKeyWithPressCallback(btnHandle, 
        CONFIG_DONE_COFFEE_MAKER_POWER_KEY,
        PowerKeyCB);

    iot_button_register_cb(btnHandle, 
        BUTTON_LONG_PRESS_START, 
        PowerKeyLongPressCB, NULL);
    
    InitKeyWithPressCallback(btnHandle, 
        CONFIG_DONE_COFFEE_MAKER_MICRO_SWITCH,
        MicroSwitchCB);

    return (app_driver_handle_t)handle;
}

static void CookingModeGrindCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeGrindCB");
    LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, GrinderEndpointID);    
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, GRINDER_MODE, CookingModeEndpointID);    
}

static void CookingModeCoffeeCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeCoffeeCB");
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, COFFEE_MODE, CookingModeEndpointID);        
}

static void CookingModeTeaCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CookingModeTeaCB");
    LevelControlUpdateCurrentValue(
        EXPLICIT_MODE, TEA_MODE, CookingModeEndpointID);        
}

static app_driver_handle_t CookingModeInit()
{
    app_driver_handle_t handle;
    button_handle_t btn1Handle, btn2Handle, btn3Handle;    

    InitKeyWithPressCallback(btn1Handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_GRINDER,
        CookingModeGrindCB);

    InitKeyWithPressCallback(btn2Handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_COFFEE,
        CookingModeCoffeeCB);    

    InitKeyWithPressCallback(btn3Handle, 
        CONFIG_DONE_COFFEE_MAKER_COOKING_MODE_TEA,
        CookingModeTeaCB);        

    return (app_driver_handle_t)handle;
}

static void CupCounterKeyCB(void *Arg, void *Data)
{
    ESP_LOGI(TAG, "CupCounterKeyCB");
    LevelControlUpdateCurrentValue(
        INCREMENT_MODE, DONT_CARE, CupCounterEndpointID);        
}

static app_driver_handle_t CupCounterInit()
{
    app_driver_handle_t handle;
    button_handle_t btnHandle;    

    InitKeyWithPressCallback(btnHandle, 
        CONFIG_DONE_COFFEE_MAKER_CUP_COUNTER_KEY,
        CupCounterKeyCB);

    return (app_driver_handle_t)handle;
}

esp_err_t CookingModeSetEnable(
    app_driver_handle_t DriverHandle, 
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t GrinderSetEnable(
    app_driver_handle_t DriverHandle, 
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

esp_err_t CupCounterSetEnable(
    app_driver_handle_t DriverHandle, 
    esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    return err;
}

/**
 * @brief This API should be called to update the driver for the attribute being updated.
 * This is usually called from the common `app_attribute_update_cb()`.
 * @param[in] EndpointID Endpoint ID of the attribute.
 * @param[in] ClusterID Cluster ID of the attribute.
 * @param[in] AttributeID Attribute ID of the attribute.
 * @param[in] val Pointer to `esp_matter_attr_val_t`. Use appropriate elements as per the value type.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerAttributeUpdate(
    app_driver_handle_t DriverHandle, 
    const uint16_t &EndpointID, const uint32_t &ClusterID,
    const uint32_t &AttributeID, esp_matter_attr_val_t *Val)
{
    esp_err_t err = ESP_OK;

    if (EndpointID == PowerKeyEndpointID) 
    {
        if (ClusterID == OnOff::Id) 
        {            
            if (AttributeID == OnOff::Attributes::OnOff::Id) 
            {
                //err = app_driver_light_set_power(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterOnOff_id %ld, attributeOnOff_id %ld", 
                PowerKeyEndpointID, ClusterID, AttributeID);
            }        
        }
    }
    else if (EndpointID == CookingModeEndpointID) {        
    }
    else if (EndpointID == GrinderEndpointID) {        
    }
    else if (EndpointID == CupCounterEndpointID) {        
        //led_driver_handle_t handle = (led_driver_handle_t)DriverHandle;
        if (ClusterID == OnOff::Id) {            
            if (AttributeID == OnOff::Attributes::OnOff::Id) {
                //err = app_driver_light_set_power(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterOnOff_id %ld, attributeOnOff_id %ld", 
                PowerKeyEndpointID, ClusterID, AttributeID);
            }
            
        } else if (ClusterID == LevelControl::Id) {
            if (AttributeID == LevelControl::Attributes::CurrentLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterLevelControl_id %ld, attributeCurrentLevel_id %ld", PowerKeyEndpointID, ClusterID, AttributeID);
            }
            else if (AttributeID == LevelControl::Attributes::MinLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterLevelControl_id %ld, attributeMinLevel_id %ld", PowerKeyEndpointID, ClusterID, AttributeID);
            }
            else if (AttributeID == LevelControl::Attributes::MaxLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterLevelControl_id %ld, attributeMaxLevel_id %ld", PowerKeyEndpointID, ClusterID, AttributeID);
            }
            else if (AttributeID == LevelControl::Attributes::MaxLevel::Id) {
                //err = app_driver_light_set_brightness(handle, val);
                ESP_LOGW(TAG, "EndpointID %d, clusterLevelControl_id %ld, attributeMaxLevel_id %ld", PowerKeyEndpointID, ClusterID, AttributeID);
            }
        }
    }

    return err;
}

/** 
 * @brief create coffee maker device with its related endpoints
 * @param[in] Node Endpoint0 or root Node.
 * @return ESP_OK on success.
 * @return error in case of failure.
 */
esp_err_t DoneCoffeeMakerCreate(node_t* Node)
{
    esp_err_t err = ESP_OK;

    app_driver_handle_t powerKeyHandle = PowerKeyInit();    
    DoneMasterPowerKey::config_t powerKeyConfig;
    powerKeyConfig.on_off.on_off = true;//for powerOn when Plug in Powerline.  
    powerKeyConfig.boolean.state_value = true;//MicroSwitchMode_t::NORMAL_MODE  
    powerKeyConfig.level_control.current_level = STANDBY_MODE;
    powerKeyConfig.level_control.lighting.min_level = 1;
    powerKeyConfig.level_control.lighting.max_level = 3;      
    endpoint_t *powerKeyEndpoint = DoneMasterPowerKey::create(
        Node, &powerKeyConfig,    
        ENDPOINT_FLAG_NONE, powerKeyHandle);                
    if (!powerKeyEndpoint)
    {
        ESP_LOGE(TAG, "powerKeyEndpoint creation failed");
    }
    else 
    {
        PowerKeyEndpointID = endpoint::get_id(powerKeyEndpoint);
        ESP_LOGI(TAG, "powerKeyEndpoint created with EndpointID %d", PowerKeyEndpointID);
    }

    app_driver_handle_t cookingModeHandle = CookingModeInit();    
    DoneMultiFunctionSwitch::config_t CookingModeConfig;
    CookingModeConfig.on_off.on_off= true;
    CookingModeConfig.level_control.current_level = GRINDER_MODE;
    CookingModeConfig.level_control.lighting.min_level = 1;
    CookingModeConfig.level_control.lighting.max_level = 3;
    endpoint_t *cookingModeEndpoint = DoneMultiFunctionSwitch::create(Node, &CookingModeConfig, ENDPOINT_FLAG_NONE, cookingModeHandle);
    if (!cookingModeEndpoint)
    {
        ESP_LOGE(TAG, "cookingModeEndpoint creation failed");
    }
    else 
    {
        CookingModeEndpointID = endpoint::get_id(cookingModeEndpoint);
        ESP_LOGI(TAG, "cookingModeEndpoint created with EndpointID %d", CookingModeEndpointID);
    }

    done_MultiFunctionSwitch::config_t grinderConfig;
    grinderConfig.on_off.on_off= true;        
    grinderConfig.level_control.current_level = 1;
    grinderConfig.level_control.lighting.min_level = 1;
    grinderConfig.level_control.lighting.max_level = 3;
    endpoint_t *grinderEndpoint = done_MultiFunctionSwitch::create(Node, &grinderConfig,         ENDPOINT_FLAG_NONE, NULL);
    if (!grinderEndpoint)
    {
        ESP_LOGE(TAG, "grinderEndpoint creation failed");
    }
    else 
    {
        GrinderEndpointID = endpoint::get_id(grinderEndpoint);
        ESP_LOGI(TAG, "grinderEndpoint created with EndpointID %d", GrinderEndpointID);
    }

    app_driver_handle_t cupCounterHandle = CupCounterInit();
    done_MultiFunctionSwitch::config_t cupCounterConfig;
    cupCounterConfig.on_off.on_off= true;        
    cupCounterConfig.level_control.current_level = 2;
    cupCounterConfig.level_control.lighting.min_level = 2;
    cupCounterConfig.level_control.lighting.max_level = 7;
    endpoint_t *cupCounterEndpoint = done_MultiFunctionSwitch::create(Node, &cupCounterConfig, ENDPOINT_FLAG_NONE, cupCounterHandle);
    if (!cupCounterEndpoint)
    {
        ESP_LOGE(TAG, "cupCounter_endpoint creation failed");
    }
    else 
    {
        CupCounterEndpointID = endpoint::get_id(cupCounterEndpoint);
        ESP_LOGI(TAG, "cupCounterEndpoint created with EndpointID %d", CupCounterEndpointID);
    }

    return err;
}