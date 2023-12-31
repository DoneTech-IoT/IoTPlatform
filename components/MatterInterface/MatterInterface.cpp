#include <esp_err.h>
#include <esp_log.h>


#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/ESP32/OpenthreadLauncher.h>
#endif

#include "MatterInterface.h"

// ****************************** Local Variables 
static const char *TAG = "MatterTask";
uint16_t switch_endpoint_id = 0;
static MatterInterfaceHandler_t InterfaceHandler;
// ****************************** Local Functions 

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
        ESP_LOGI(TAG, "Interface IP Address Changed");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGI(TAG, "Commissioning complete");
        break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
        ESP_LOGI(TAG, "Commissioning failed, fail safe timer expired");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        ESP_LOGI(TAG, "Commissioning session started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        ESP_LOGI(TAG, "Commissioning session stopped");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        ESP_LOGI(TAG, "Commissioning window opened");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        ESP_LOGI(TAG, "Commissioning window closed");
        break;

    default:
        break;
    }

    //InterfaceHandler.MatterNetworkEventCB(event, arg);
}

static esp_err_t app_identification_cb(identification::callback_type_t type, uint16_t endpoint_id, uint8_t effect_id,
                                       uint8_t effect_variant, void *priv_data)
{
    ESP_LOGI(TAG, "Identification callback: type: %u, effect: %u, variant: %u", type, effect_id, effect_variant);

    InterfaceHandler.MatterIdentificationCB(type, endpoint_id, effect_id, effect_variant, priv_data);

    return ESP_OK;
}

static esp_err_t app_attribute_update_cb(callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                                         uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    if (type == PRE_UPDATE) {
        /* Handle the attribute updates here. */
    }
 
    InterfaceHandler.MatterAttributeUpdateCB(type, endpoint_id, cluster_id, attribute_id, val, priv_data);

    return ESP_OK;
}

bool Matter_TaskInit(MatterInterfaceHandler_t *MatterInterfaceHandler)
{
    InterfaceHandler = *MatterInterfaceHandler;

    if(InterfaceHandler.SharedBufQueue != NULL &&
       InterfaceHandler.SharedSemaphore  != NULL && 
       //InterfaceHandler.MatterNetworkEventCB != NULL &&
       //InterfaceHandler.MatterIdentificationCB != NULL &&
       InterfaceHandler.MatterAttributeUpdateCB != NULL)        
    {
        esp_err_t err = ESP_OK;
    
        /* Initialize driver */
        app_driver_handle_t switch_handle = app_driver_switch_init();
        app_reset_button_register(switch_handle);

        /* Create a Matter node and add the mandatory Root Node device type on endpoint 0 */
        node::config_t node_config;
        node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);

        on_off_switch::config_t switch_config;
        endpoint_t *endpoint = on_off_switch::create(node, &switch_config, ENDPOINT_FLAG_NONE, switch_handle);

        /* These node and endpoint handles can be used to create/add other endpoints and clusters. */
        if (!node || !endpoint) {
            ESP_LOGE(TAG, "Matter node creation failed");
        }

        /* Add group cluster to the switch endpoint */
        cluster::groups::config_t groups_config;
        cluster::groups::create(endpoint, &groups_config, CLUSTER_FLAG_SERVER | CLUSTER_FLAG_CLIENT);

        switch_endpoint_id = endpoint::get_id(endpoint);
        ESP_LOGI(TAG, "Switch created with endpoint_id %d", switch_endpoint_id);

    #if CHIP_DEVICE_CONFIG_ENABLE_THREAD
        /* Set OpenThread platform config */
        esp_openthread_platform_config_t config = {
            .radio_config = ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG(),
            .host_config = ESP_OPENTHREAD_DEFAULT_HOST_CONFIG(),
            .port_config = ESP_OPENTHREAD_DEFAULT_PORT_CONFIG(),
        };
        set_openthread_platform_config(&config);
    #endif

        /* Matter start */
        err = esp_matter::start(app_event_cb);
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "Matter start failed: %d", err);
        }

    #if CONFIG_ENABLE_CHIP_SHELL

        esp_matter::console::diagnostics_register_commands();
        esp_matter::console::wifi_register_commands();
        esp_matter::console::init();
    #endif

        ESP_LOGI(TAG, "Matter app initiated successfully");
    }    

    else
    {
        ESP_LOGW(TAG, "Matter is already initiated");
    }
    return ESP_OK;
}