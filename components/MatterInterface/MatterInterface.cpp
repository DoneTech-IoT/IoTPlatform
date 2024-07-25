#include <esp_err.h>
#include <esp_log.h>

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include <platform/ESP32/OpenthreadLauncher.h>
#endif

#include "MatterInterface.h"
#include "DoneMatterEndpoint.h"
#include "DoneCoffeeMaker.h"
#include "Custom_Log.h"

// ****************************** Local Variables
static const char *TAG = "MatterInterface";
static MatterInterfaceHandler_t *InterfaceHandler;
// ****************************** Local Functions
using namespace esp_matter;
using namespace esp_matter::attribute;
using namespace esp_matter::endpoint;
using namespace chip::app::Clusters;

uint16_t switch_endpoint_id = 0;

static void app_event_cb(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type)
    {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
    {
        chip::CommissioningWindowManager &commissionMgr = chip::Server::GetInstance().GetCommissioningWindowManager();
        if (!commissionMgr.IsCommissioningWindowOpen())
        {
            if (InterfaceHandler->ConnectToMatterNetwork != NULL)
                InterfaceHandler->ConnectToMatterNetwork();
        }
        Log_RamStatus("Matter", "Interface IP Address Changed");
        ESP_LOGW(TAG, "Interface IP Address changed");
        break;
    }
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        ESP_LOGW(TAG, "Commissioning complete");
        Log_RamStatus("Matter", "Commissioning complete");
        break;

    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
        ESP_LOGW(TAG, "Commissioning failed, fail safe timer expired");
        Log_RamStatus("Matter", "Commissioning failed");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        Log_RamStatus("Matter", "Commissioning started");
        ESP_LOGW(TAG, "Commissioning session started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        ESP_LOGW(TAG, "Commissioning session stopped");
        Log_RamStatus("Matter", "Commissioning started");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        Log_RamStatus("Matter", "Commissioning window opened");
        ESP_LOGW(TAG, "Commissioning window opened");
        break;

    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        Log_RamStatus("Matter", "Commissioning window closed");
        ESP_LOGW(TAG, "Commissioning window closed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricRemoved:
        Log_RamStatus("Matter", "Fabric removed successfully");
        ESP_LOGW(TAG, "Fabric removed successfully");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricWillBeRemoved:
        Log_RamStatus("Matter", "Fabric will be removed");
        ESP_LOGW(TAG, "Fabric will be removed");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricUpdated:
        Log_RamStatus("Matter", "Fabric is updated");
        ESP_LOGW(TAG, "Fabric is updated");
        break;

    case chip::DeviceLayer::DeviceEventType::kFabricCommitted:
        Log_RamStatus("Matter", "Fabric is committed");
        ESP_LOGW(TAG, "Fabric is committed");
        InterfaceHandler->ConnectToMatterNetwork();
        break;

    case chip::DeviceLayer::DeviceEventType::kBLEDeinitialized:
        Log_RamStatus("Matter", "BLE deinitialized");
        ESP_LOGW(TAG, "BLE deinitialized and memory reclaimed");
        break;
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

    if (type == PRE_UPDATE) {//update before save in database(usually drivers)
        app_driver_handle_t driver_handle = (app_driver_handle_t)priv_data;
        err = app_driver_attribute_update(driver_handle, endpoint_id, cluster_id, attribute_id, val);
    }
    if (type == POST_UPDATE) {//update after save in database
        
    }
    if (type == READ) {
        
    }
    if (type == WRITE) {
        
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
        
        /* Create a Matter node and add the mandatory Root Node device type on endpoint 0 */
        Log_RamOccupy("Matter", "making node");
        node::config_t node_config;
        node_t *node = node::create(&node_config, app_attribute_update_cb, app_identification_cb);
        Log_RamOccupy("Matter", "making node");

        Log_RamOccupy("Matter", "making endpoint");
        on_off_switch::config_t switch_config;
        endpoint_t *endpoint1 = on_off_switch::create(node, &switch_config, ENDPOINT_FLAG_NONE, switch_handle);

        Log_RamOccupy("Matter", "making endpoint");        
        err = create_DoneCoffeeMaker(node);        

        /* These node and endpoint handles can be used to create/add other endpoints and clusters. */
        if (!node || !endpoint1)
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