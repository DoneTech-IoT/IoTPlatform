#include <esp_err.h>
#include <esp_log.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/ESP32/OpenthreadLauncher.h>
#endif

#include "MatterInterface.h"
#include "DoneMatterEndpoint.h"
#include "Custom_Log.h"
// ****************************** Local Variables
static const char *TAG = "MatterInterface";
uint16_t switch_endpoint_id = 0;
uint16_t coffee_maker_endpoint_id = 0;
uint16_t multiFunction_switch_id = 0;
static MatterInterfaceHandler_t *InterfaceHandler;
// ****************************** Local Functions

using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type)
    {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
    {
        ESP_LOGI(TAG, "Interface IP Address Changed");
        Log_RamStatus("Matter", "Interface IP Address Changed");
        break;
    }

    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
    {
        ESP_LOGI(TAG, "Commissioning complete");
        Log_RamStatus("Matter", "Commissioning complete");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
    {
        ESP_LOGI(TAG, "Commissioning failed, fail safe timer expired");
        Log_RamStatus("Matter", "Commissioning failed");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
    {
        ESP_LOGI(TAG, "Commissioning session started");
        Log_RamStatus("Matter", "Commissioning started");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
    {
        ESP_LOGI(TAG, "Commissioning session stopped");
        Log_RamStatus("Matter", "Commissioning stopped");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
    {
        ESP_LOGI(TAG, "Commissioning window opened");
        Log_RamStatus("Matter", "Commissioning window opened");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
    {
        ESP_LOGI(TAG, "Commissioning window closed");
        Log_RamStatus("Matter", "Commissioning window closed");
        break;
    }
    default:
        break;
    }
}
static esp_err_t app_identification_cb(identification::callback_type_t type, uint16_t endpoint_id, uint8_t effect_id,
                                       uint8_t effect_variant, void *priv_data)
{
    ESP_LOGI(TAG, "Identification callback: type: %u, effect: %u, variant: %u", type, effect_id, effect_variant);

    InterfaceHandler->MatterIdentificationCB(type, endpoint_id, effect_id, effect_variant, priv_data);

    return ESP_OK;
}

static esp_err_t app_attribute_update_cb(callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                                         uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
     esp_err_t err = ESP_OK;

    if (type == PRE_UPDATE) {
        /* Driver update */
        app_driver_handle_t driver_handle = (app_driver_handle_t)priv_data;
        err = app_driver_attribute_update(driver_handle, endpoint_id, cluster_id, attribute_id, val);
    }

    InterfaceHandler->MatterAttributeUpdateCB(type, endpoint_id, cluster_id, attribute_id, val, priv_data);
    
    return err;        
}

bool Matter_TaskInit(MatterInterfaceHandler_t *MatterInterfaceHandler)
{
    InterfaceHandler = MatterInterfaceHandler;

    if (InterfaceHandler->SharedBufQueue != NULL &&
        InterfaceHandler->SharedSemaphore != NULL &&
        // InterfaceHandler->MatterNetworkEventCB != NULL &&
        // InterfaceHandler->MatterIdentificationCB != NULL &&
        InterfaceHandler->MatterAttributeUpdateCB != NULL)
    {
        Log_RamStatus("Matter", "Start Matter");
        esp_err_t err = ESP_OK;
        /* Initialize driver */
        app_driver_handle_t switch_handle = app_driver_switch_init();
        app_reset_button_register(switch_handle);

        // app_driver_handle_t coffee_maker_handle = app_driver_coffee_maker_init();

        /* Create a Matter node and add the mandatory Root Node device type on endpoint 0 */
        Log_RamOccupy("Matter", "making node");
        node::config_t node_config;
        node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
        Log_RamOccupy("Matter", "making node");
        Log_RamOccupy("Matter", "making endpoint");
        on_off_switch::config_t switch_config;
        endpoint_t *endpoint1 = on_off_switch::create(node, &switch_config, ENDPOINT_FLAG_NONE, switch_handle);

        Log_RamOccupy("Matter", "making endpoint");
        done_coffee_maker::config_t coffee_maker_config;
        endpoint_t *endpoint2 = done_coffee_maker::create(node, &coffee_maker_config, ENDPOINT_FLAG_NONE, NULL /*coffee_maker_handle*/);

        done_multiFunction_switch::config_t multiFunction_switch_config;
        endpoint_t *endpoint3 = done_multiFunction_switch::create(node, &multiFunction_switch_config, ENDPOINT_FLAG_NONE, NULL /*coffee_maker_handle*/);

        /* These node and endpoint handles can be used to create/add other endpoints and clusters. */
        if (!node || !endpoint1 || !endpoint2 || !endpoint3)
        {
            ESP_LOGE(TAG, "Matter node creation failed");
        }

        // SEZ@Done on_off_switch doesn't have group user by default.
        // so add it manually.

        /* Add group cluster to the switch endpoint */
        cluster::groups::config_t groups_config;
        cluster::groups::create(endpoint1, &groups_config, CLUSTER_FLAG_SERVER | CLUSTER_FLAG_CLIENT);

        switch_endpoint_id = endpoint::get_id(endpoint1);
        ESP_LOGI(TAG, "Switch created with endpoint_id %d", switch_endpoint_id);

        coffee_maker_endpoint_id = endpoint::get_id(endpoint2);
        ESP_LOGI(TAG, "Coffee Maker created with endpoint_id %d", coffee_maker_endpoint_id);
       
        multiFunction_switch_id = endpoint::get_id(endpoint3);
        ESP_LOGI(TAG, "Multi Function switch created with endpoint_id %d", multiFunction_switch_id);

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
        Log_RamOccupy("Matter", "Matter start");
        err = esp_matter::start(app_event_cb);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Matter start failed: %d", err);
        }
        Log_RamOccupy("Matter", "Matter start");
#if CONFIG_ENABLE_CHIP_SHELL
        Log_RamOccupy("Matter", "Matter init peripheral");
        esp_matter::console::diagnostics_register_commands();
        esp_matter::console::wifi_register_commands();
        esp_matter::console::init();
        Log_RamOccupy("Matter", "Matter init peripheral");
#endif

        ESP_LOGI(TAG, "Matter app initiated successfully");
    }

    else
    {
        ESP_LOGW(TAG, "Matter is already initiated");
    }
    return ESP_OK;
}