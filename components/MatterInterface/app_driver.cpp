/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <esp_matter.h>
#include <esp_matter_console.h>
#include <led_driver.h>

#include <app_priv.h>
#include <app_reset.h>

using chip::kInvalidClusterId;
static constexpr chip::CommandId kInvalidCommandId = 0xFFFF'FFFF;

using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::cluster;

static const char *TAG = "app_driver";
extern uint16_t switch_endpoint_id;

#if CONFIG_ENABLE_CHIP_SHELL
static char console_buffer[101] = {0};
static esp_err_t app_driver_bound_console_handler(int argc, char **argv)
{
    if (argc == 1 && strncmp(argv[0], "help", sizeof("help")) == 0) {
        printf("Bound commands:\n"
               "\thelp: Print help\n"
               "\tinvoke: <local_endpoint_id> <cluster_id> <command_id> parameters ... \n"
               "\t\tExample: matter esp bound invoke 0x0001 0x0003 0x0000 0x78.\n"
               "\tinvoke-group: <local_endpoint_id> <cluster_id> <command_id> parameters ...\n"
               "\t\tExample: matter esp bound invoke-group 0x0001 0x0003 0x0000 0x78.\n");
    } else if (argc >= 4 && strncmp(argv[0], "invoke", sizeof("invoke")) == 0) {
        client::command_handle_t cmd_handle;
        uint16_t local_endpoint_id = strtol((const char *)&argv[1][2], NULL, 16);
        cmd_handle.cluster_id = strtol((const char *)&argv[2][2], NULL, 16);
        cmd_handle.command_id = strtol((const char *)&argv[3][2], NULL, 16);        

        if (argc > 4) {
            console_buffer[0] = argc - 4;
            for (int i = 0; i < (argc - 4); i++) {
                if ((argv[4 + i][0] != '0') || (argv[4 + i][1] != 'x') ||
                    (strlen((const char *)&argv[4 + i][2]) > 10)) {
                    ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
                    return ESP_ERR_INVALID_ARG;
                }
                strcpy((console_buffer + 1 + 10 * i), &argv[4 + i][2]);
            }

            cmd_handle.command_data = console_buffer;
        }

        client::cluster_update(local_endpoint_id, &cmd_handle);
    } else if (argc >= 4 && strncmp(argv[0], "invoke-group", sizeof("invoke-group")) == 0) {
        client::command_handle_t cmd_handle;
        uint16_t local_endpoint_id = strtol((const char *)&argv[1][2], NULL, 16);
        cmd_handle.cluster_id = strtol((const char *)&argv[2][2], NULL, 16);
        cmd_handle.command_id = strtol((const char *)&argv[3][2], NULL, 16);        

        if (argc > 4) {
            console_buffer[0] = argc - 4;
            for (int i = 0; i < (argc - 4); i++) {
                if ((argv[4 + i][0] != '0') || (argv[4 + i][1] != 'x') ||
                    (strlen((const char *)&argv[4 + i][2]) > 10)) {
                    ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
                    return ESP_ERR_INVALID_ARG;
                }
                strcpy((console_buffer + 1 + 10 * i), &argv[4 + i][2]);
            }

            cmd_handle.command_data = console_buffer;
        }

        client::cluster_update(local_endpoint_id, &cmd_handle);
    } else {
        ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

static esp_err_t app_driver_client_console_handler(int argc, char **argv)
{
    if (argc == 1 && strncmp(argv[0], "help", sizeof("help")) == 0) {
        printf("Client commands:\n"
               "\thelp: Print help\n"
               "\tinvoke: <fabric_index> <remote_node_id> <remote_endpoint_id> <cluster_id> <command_id> parameters "
               "... \n"
               "\t\tExample: matter esp client invoke 0x0001 0xBC5C01 0x0001 0x0003 0x0000 0x78.\n"
               "\tinvoke-group: <fabric_index> <group_id> <cluster_id> <command_id> parameters ... \n"
               "\t\tExample: matter esp client invoke-group 0x0001 0x257 0x0003 0x0000 0x78.\n");
    } else if (argc >= 6 && strncmp(argv[0], "invoke", sizeof("invoke")) == 0) {
        client::command_handle_t cmd_handle;
        uint8_t fabric_index = strtol((const char *)&argv[1][2], NULL, 16);
        uint64_t node_id = strtol((const char *)&argv[2][2], NULL, 16);
        cmd_handle.endpoint_id = strtol((const char *)&argv[3][2], NULL, 16);
        cmd_handle.cluster_id = strtol((const char *)&argv[4][2], NULL, 16);
        cmd_handle.command_id = strtol((const char *)&argv[5][2], NULL, 16);        

        if (argc > 6) {
            console_buffer[0] = argc - 6;
            for (int i = 0; i < (argc - 6); i++) {
                if ((argv[6 + i][0] != '0') || (argv[6 + i][1] != 'x') ||
                    (strlen((const char *)&argv[6 + i][2]) > 10)) {
                    ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
                    return ESP_ERR_INVALID_ARG;
                }
                strcpy((console_buffer + 1 + 10 * i), &argv[6 + i][2]);
            }

            cmd_handle.command_data = console_buffer;
        }

        client::connect(fabric_index, node_id, &cmd_handle);
    } else if (argc >= 5 && strncmp(argv[0], "invoke-group", sizeof("invoke-group")) == 0) {
        client::command_handle_t cmd_handle;
        uint8_t fabric_index = strtol((const char *)&argv[1][2], NULL, 16);
        cmd_handle.group_id = strtol((const char *)&argv[2][2], NULL, 16);
        cmd_handle.cluster_id = strtol((const char *)&argv[3][2], NULL, 16);
        cmd_handle.command_id = strtol((const char *)&argv[4][2], NULL, 16);        

        if (argc > 5) {
            console_buffer[0] = argc - 5;
            for (int i = 0; i < (argc - 5); i++) {
                if ((argv[5 + i][0] != '0') || (argv[5 + i][1] != 'x') ||
                    (strlen((const char *)&argv[5 + i][2]) > 10)) {
                    ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
                    return ESP_ERR_INVALID_ARG;
                }
                strcpy((console_buffer + 1 + 10 * i), &argv[5 + i][2]);
            }

            cmd_handle.command_data = console_buffer;
        }

        client::group_command_send(fabric_index, &cmd_handle);
    } else {
        ESP_LOGE(TAG, "Incorrect arguments. Check help for more details.");
        return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

static void app_driver_register_commands()
{
    /* Add console command for bound devices */
    static const esp_matter::console::command_t bound_command = {
        .name = "bound",
        .description = "This can be used to simulate on-device control for bound devices."
                       "Usage: matter esp bound <bound_command>. "
                       "Bound commands: help, invoke",
        .handler = app_driver_bound_console_handler,
    };
    esp_matter::console::add_commands(&bound_command, 1);

    /* Add console command for client to control non-bound devices */
    static const esp_matter::console::command_t client_command = {
        .name = "client",
        .description = "This can be used to simulate on-device control for client devices."
                       "Usage: matter esp client <client_command>. "
                       "Client commands: help, invoke",
        .handler = app_driver_client_console_handler,
    };
    esp_matter::console::add_commands(&client_command, 1);
}
#endif // CONFIG_ENABLE_CHIP_SHELL

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id,      uint32_t cluster_id,uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    esp_err_t err = ESP_OK;
    // if (endpoint_id == light_endpoint_id) {
    //     led_driver_handle_t handle = (led_driver_handle_t)driver_handle;
    //     if (cluster_id == OnOff::Id) {
    //         if (attribute_id == OnOff::Attributes::OnOff::Id) {
    //             err = app_driver_light_set_power(handle, val);
    //         }
    //     } else if (cluster_id == LevelControl::Id) {
    //         if (attribute_id == LevelControl::Attributes::CurrentLevel::Id) {
    //             err = app_driver_light_set_brightness(handle, val);
    //         }
    //     } else if (cluster_id == ColorControl::Id) {
    //         if (attribute_id == ColorControl::Attributes::CurrentHue::Id) {
    //             err = app_driver_light_set_hue(handle, val);
    //         } else if (attribute_id == ColorControl::Attributes::CurrentSaturation::Id) {
    //             err = app_driver_light_set_saturation(handle, val);
    //         } else if (attribute_id == ColorControl::Attributes::ColorTemperatureMireds::Id) {
    //             err = app_driver_light_set_temperature(handle, val);
    //         }
    //     }
    // }
    return err;
}

void app_driver_client_command_callback(client::peer_device_t *peer_device, client::command_handle_t *cmd_handle,void *priv_data)
{
    // on_off light switch should support on_off cluster and identify cluster commands sending.
    if (cmd_handle->cluster_id == OnOff::Id) {
        switch (cmd_handle->command_id) {
        case OnOff::Commands::Off::Id: {
            on_off::command::send_off(peer_device, cmd_handle->endpoint_id);
            break;
        };
        case OnOff::Commands::On::Id: {
            on_off::command::send_on(peer_device, cmd_handle->endpoint_id);
            break;
        };
        case OnOff::Commands::Toggle::Id: {
            on_off::command::send_toggle(peer_device, cmd_handle->endpoint_id);
            break;
        };
        default:
            break;
        }
    } else if (cmd_handle->cluster_id == Identify::Id) {
        if (cmd_handle->command_id == Identify::Commands::Identify::Id) {
            if (((char *)cmd_handle->command_data)[0] != 1) {
                ESP_LOGE(TAG, "Number of parameters error");
                return;
            }
            identify::command::send_identify(peer_device, cmd_handle->endpoint_id,
                                             strtol((const char *)(cmd_handle->command_data) + 1, NULL, 16));
        } else {
            ESP_LOGE(TAG, "Unsupported command");
        }
    } else {
        ESP_LOGE(TAG, "Unsupported cluster");
    }
}

void app_driver_client_group_command_callback(uint8_t fabric_index, client::command_handle_t *cmd_handle,
                                              void *priv_data)
{
    // on_off light switch should support on_off cluster and identify cluster commands sending.
    if (cmd_handle->cluster_id == OnOff::Id) {
        switch (cmd_handle->command_id) {
        case OnOff::Commands::Off::Id: {
            on_off::command::group_send_off(fabric_index, cmd_handle->group_id);
            break;
        }
        case OnOff::Commands::On::Id: {
            on_off::command::group_send_on(fabric_index, cmd_handle->group_id);
            break;
        }
        case OnOff::Commands::Toggle::Id: {
            on_off::command::group_send_toggle(fabric_index, cmd_handle->group_id);
            break;
        }
        default:
            break;
        }
    } else if (cmd_handle->cluster_id == Identify::Id) {
        if (cmd_handle->command_id == Identify::Commands::Identify::Id) {
            if (((char *)cmd_handle->command_data)[0] != 1) {
                ESP_LOGE(TAG, "Number of parameters error");
                return;
            }
            identify::command::group_send_identify(fabric_index, cmd_handle->group_id,
                                                   strtol((const char *)(cmd_handle->command_data) + 1, NULL, 16));
        } else {
            ESP_LOGE(TAG, "Unsupported command");
        }
    } else {
        ESP_LOGE(TAG, "Unsupported cluster");
    }
}

static void app_driver_button_toggle_cb(void *arg, void *data)
{
    ESP_LOGI(TAG, "Toggle button pressed");
    client::command_handle_t cmd_handle;
    cmd_handle.cluster_id = OnOff::Id;
    cmd_handle.command_id = OnOff::Commands::Toggle::Id;    

    lock::chip_stack_lock(portMAX_DELAY);
    client::cluster_update(switch_endpoint_id, &cmd_handle);
    lock::chip_stack_unlock();
}

app_driver_handle_t app_driver_switch_init()
{
    /* Initialize button */
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_toggle_cb, NULL);

    /* Other initializations */
#if CONFIG_ENABLE_CHIP_SHELL
    app_driver_register_commands();
#endif // CONFIG_ENABLE_CHIP_SHELL
    client::set_command_callback(app_driver_client_command_callback, app_driver_client_group_command_callback, NULL);

    return (app_driver_handle_t)handle;
}

app_driver_handle_t app_driver_coffee_maker_init()
{    
    //just repeat body of above func.
    button_config_t config = button_driver_get_config();
    button_handle_t handle = iot_button_create(&config);
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_toggle_cb, NULL);

    return (app_driver_handle_t)handle;
}

int get_endpoint(gpio_button* button) {
    for (int i = 0; i < configured_buttons; i++) {
        if (button_list[i].button == button) {
            return button_list[i].endpoint;
        }
    }
    return -1;
}

#if CONFIG_GENERIC_SWITCH_TYPE_LATCHING
static void app_driver_button_switch_latched(void *arg, void *data)
{
    ESP_LOGI(TAG, "Switch lached pressed");
    gpio_button * button = (gpio_button*)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    // Press moves Position from 0 (idle) to 1 (press)
    uint8_t newPosition = 1;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // SwitchLatched event takes newPosition as event data
    switch_cluster::event::send_switch_latched(switch_endpoint_id, newPosition);
    lock::chip_stack_unlock();
}
#endif
#if CONFIG_GENERIC_SWITCH_TYPE_MOMENTARY
static void app_driver_button_initial_pressed(void *arg, void *data)
{
    ESP_LOGI(TAG, "Initial button pressed");
    gpio_button * button = (gpio_button*)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    // Press moves Position from 0 (idle) to 1 (press)
    uint8_t newPosition = 1;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // InitialPress event takes newPosition as event data
    switch_cluster::event::send_initial_press(switch_endpoint_id, newPosition);
    lock::chip_stack_unlock();
}

static void app_driver_button_release(void *arg, void *data)
{
    gpio_button *button = (gpio_button *)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    if(iot_button_get_ticks_time((button_handle_t)arg) < 5000){
    ESP_LOGI(TAG, "Short button release");
    // Release moves Position from 1 (press) to 0 (idle)
    uint8_t previousPosition = 1;
    uint8_t newPosition = 0;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // ShortRelease event takes previousPosition as event data
    switch_cluster::event::send_short_release(switch_endpoint_id, previousPosition);
    lock::chip_stack_unlock();
    }
    else{
    ESP_LOGI(TAG, "Long button release");
    // Release moves Position from 1 (press) to 0 (idle)
    uint8_t previousPosition = 1;
    uint8_t newPosition = 0;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // LongRelease event takes previousPositionPosition as event data
    switch_cluster::event::send_long_release(switch_endpoint_id, previousPosition);
    lock::chip_stack_unlock();
    }
}

static void app_driver_button_long_pressed(void *arg, void *data)
{
    ESP_LOGI(TAG, "Long button pressed ");
    gpio_button *button = (gpio_button *)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    // Press moves Position from 0 (idle) to 1 (press)
    uint8_t newPosition = 1;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // LongPress event takes newPosition as event data
    switch_cluster::event::send_long_press(switch_endpoint_id, newPosition);
    lock::chip_stack_unlock();
}

static int current_number_of_presses_counted = 1;

static void app_driver_button_multipress_ongoing(void *arg, void *data)
{
    ESP_LOGI(TAG, "Multipress Ongoing");
    gpio_button * button = (gpio_button *)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    // Press moves Position from 0 (idle) to 1 (press)
    uint8_t newPosition = 1;
    current_number_of_presses_counted++;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // MultiPress Ongoing event takes newPosition and current_number_of_presses_counted as event data
    switch_cluster::event::send_multi_press_ongoing(switch_endpoint_id, newPosition, current_number_of_presses_counted);
    lock::chip_stack_unlock();
}

static void app_driver_button_multipress_complete(void *arg, void *data)
{
    ESP_LOGI(TAG, "Multipress Complete");
    gpio_button * button = (gpio_button *)data;
    int switch_endpoint_id = (button != NULL) ? get_endpoint(button) : 1;
    // Press moves Position from 0 (idle) to 1 (press)
    uint8_t previousPosition = 1;
    uint8_t newPosition = 0;
    static int total_number_of_presses_counted = current_number_of_presses_counted;
    lock::chip_stack_lock(portMAX_DELAY);
    chip::app::Clusters::Switch::Attributes::CurrentPosition::Set(switch_endpoint_id, newPosition);
    // MultiPress Complete event takes previousPosition and total_number_of_presses_counted as event data
    switch_cluster::event::send_multi_press_complete(switch_endpoint_id, previousPosition, total_number_of_presses_counted);
    // Reset current_number_of_presses_counted to initial value
    current_number_of_presses_counted = 1;
    lock::chip_stack_unlock();
}
#endif

app_driver_handle_t app_driver_button_init(gpio_button * button)
{
    /* Initialize button */
    button_config_t config = button_driver_get_config();
    if(button != NULL)
    {
            config.type =  button_type_t::BUTTON_TYPE_GPIO;
            config.gpio_button_config.gpio_num = button->GPIO_PIN_VALUE;
    }
    button_handle_t handle = iot_button_create(&config);


#if CONFIG_GENERIC_SWITCH_TYPE_LATCHING
    iot_button_register_cb(handle, BUTTON_DOUBLE_CLICK, app_driver_button_switch_latched, button);
#endif

#if CONFIG_GENERIC_SWITCH_TYPE_MOMENTARY
    iot_button_register_cb(handle, BUTTON_PRESS_DOWN, app_driver_button_initial_pressed, button);
    iot_button_register_cb(handle, BUTTON_PRESS_UP, app_driver_button_release, button);
    iot_button_register_cb(handle, BUTTON_LONG_PRESS_START, app_driver_button_long_pressed, button);
    iot_button_register_cb(handle, BUTTON_PRESS_REPEAT, app_driver_button_multipress_ongoing, button);
    iot_button_register_cb(handle, BUTTON_PRESS_REPEAT_DONE, app_driver_button_multipress_complete, button);
#endif
    return (app_driver_handle_t)handle;
}