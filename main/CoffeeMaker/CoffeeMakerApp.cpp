#include "CoffeeMakerApp.hpp"

static const char *TAG = "coffeeMakerApp";
static char CoffeeMakerJsonOutPut[2500];

void CoffeeMakerJsonCreator(CoffeeMakerJson_str CoffeeMakerJson, char *CoffeeMakerJsonOutPut)
{

    cJSON *root = cJSON_CreateObject();
    cJSON_AddBoolToObject(root, "Coffee", CoffeeMakerJson.CoffeeFlag); // true
    cJSON *coffeeProperty = cJSON_CreateObject();
    cJSON_AddNumberToObject(coffeeProperty, "Ginder level", CoffeeMakerJson.GinderLevel);
    cJSON_AddNumberToObject(coffeeProperty, "Cup", CoffeeMakerJson.Cups);
    cJSON_AddItemToObject(root, "Coffee Property", coffeeProperty);

    cJSON *security = cJSON_CreateObject();
    cJSON_AddStringToObject(security, "Mac", CoffeeMakerJson.DeviceMACAddress);
    cJSON_AddStringToObject(security, "Pass", CoffeeMakerJson.Pass);
    cJSON_AddItemToObject(root, "Security", security);

    cJSON_AddBoolToObject(root, "Tea", 0); // false

    cJSON *teaProperty = cJSON_CreateObject();
    cJSON_AddNumberToObject(teaProperty, "Cups", CoffeeMakerJson.Cups);
    cJSON_AddNumberToObject(teaProperty, "Temp", CoffeeMakerJson.Temp);
    cJSON_AddItemToObject(root, "Tea property", teaProperty);

    cJSON_AddNumberToObject(root, "Update Time", CoffeeMakerJson.UpdateTime);

    strcpy(CoffeeMakerJsonOutPut, cJSON_Print(root));

    cJSON_Delete(root);
}

void CoffeeMakerJsonParser(CoffeeMakerJson_str *CoffeeMakerJson, char *CoffeeMakerJsonInput)
{
    cJSON *root = cJSON_Parse(CoffeeMakerJsonInput);
    if (!root)
    {
        ESP_LOGE("json ", "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }
    cJSON *coffee = cJSON_GetObjectItem(root, "Coffee");
    if (cJSON_IsBool(coffee))
    {
        CoffeeMakerJson->CoffeeFlag = cJSON_IsTrue(coffee) ? 1 : 0;
    }

    cJSON *coffee_property = cJSON_GetObjectItem(root, "Coffee Property");
    if (cJSON_IsObject(coffee_property))
    {
        cJSON *ginder_level = cJSON_GetObjectItem(coffee_property, "Ginder level");
        if (cJSON_IsNumber(ginder_level))
        {
            CoffeeMakerJson->GinderLevel = (uint8_t)ginder_level->valueint;
        }
    }

    cJSON *security = cJSON_GetObjectItem(root, "Security");
    if (cJSON_IsObject(security))
    {
        cJSON *mac = cJSON_GetObjectItem(security, "Mac");
        cJSON *pass = cJSON_GetObjectItem(security, "Pass");
        if (cJSON_IsString(mac))
        {
            strncpy(CoffeeMakerJson->DeviceMACAddress, mac->valuestring, sizeof(CoffeeMakerJson->DeviceMACAddress) - 1);
        }
        if (cJSON_IsString(pass))
        {
            strncpy(CoffeeMakerJson->Pass, pass->valuestring, sizeof(CoffeeMakerJson->Pass) - 1);
        }
    }
    // Extract and set the "Tea" value
    cJSON *tea = cJSON_GetObjectItem(root, "Tea");
    if (cJSON_IsBool(tea))
    {
        CoffeeMakerJson->TeaFlag = cJSON_IsTrue(tea) ? 1 : 0;
    }

    // Extract and set the "Tea property" object
    cJSON *tea_property = cJSON_GetObjectItem(root, "Tea property");
    if (cJSON_IsObject(tea_property))
    {
        cJSON *cups = cJSON_GetObjectItem(tea_property, "Cups");
        cJSON *temp = cJSON_GetObjectItem(tea_property, "Temp");
        if (cJSON_IsNumber(cups))
        {
            CoffeeMakerJson->Cups = (uint8_t)cups->valueint;
        }
        if (cJSON_IsNumber(temp))
        {
            CoffeeMakerJson->Temp = (uint8_t)temp->valueint;
        }
    }

    // Extract and set the "Update Time" value
    cJSON *update_time = cJSON_GetObjectItem(root, "Update Time");
    if (cJSON_IsNumber(update_time))

    {
        CoffeeMakerJson->UpdateTime = (uint16_t)update_time->valueint;
    }
    cJSON_Delete(root);
}

void parserTEST(char *temp)
{
    CoffeeMakerJson_str CoffeeMakerJson;
    printf("%s", temp);
    CoffeeMakerJsonParser(&CoffeeMakerJson, temp);
    printf("Coffee Flag: %u\n", CoffeeMakerJson.CoffeeFlag);
    printf("Tea Flag: %u\n", CoffeeMakerJson.TeaFlag);
    printf("Ginder Level: %u\n", CoffeeMakerJson.GinderLevel);
    printf("Cups: %u\n", CoffeeMakerJson.Cups);
    printf("Temp: %u\n", CoffeeMakerJson.Temp);
    printf("Update Time: %u\n", CoffeeMakerJson.UpdateTime);
    printf("Device MAC Address: %s\n", CoffeeMakerJson.DeviceMACAddress);
    printf("Pass: %s\n", CoffeeMakerJson.Pass);
}
void JSON_TEST()
{
    CoffeeMakerJson_str CoffeeMakerJson;
    CoffeeMakerJson.CoffeeFlag = true;
    CoffeeMakerJson.Cups = 4;
    strcpy(CoffeeMakerJson.DeviceMACAddress, "EE:EE:EE:EE:EE:EE");
    strcpy(CoffeeMakerJson.Pass, "12345");
    CoffeeMakerJson.GinderLevel = 1;
    CoffeeMakerJson.TeaFlag = false;
    CoffeeMakerJson.Temp = 60;
    CoffeeMakerJson.UpdateTime = 60;

    CoffeeMakerJsonCreator(CoffeeMakerJson, CoffeeMakerJsonOutPut);
    parserTEST(CoffeeMakerJsonOutPut);
}
void ApplyOnScreen()
{
    
}
QueueHandle_t MQTTDataFromBrokerQueue;
SemaphoreHandle_t MQTTConnectedSemaphore;
SemaphoreHandle_t MQTTErrorOrDisconnectSemaphore;
void RunMQTTAndTestJson()
{
    memset(CoffeeMakerJsonOutPut, 0x0, sizeof(CoffeeMakerJsonOutPut));
    while (true)
    {
        if (xSemaphoreTake(MQTTConnectedSemaphore, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
        {
        }
        if (xSemaphoreTake(MQTTErrorOrDisconnectSemaphore, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
        {
            break;
        }

        if (xQueueReceive(MQTTDataFromBrokerQueue, CoffeeMakerJsonOutPut, pdMS_TO_TICKS(MQTT_SEC)) == pdTRUE)
        {
            CoffeeMakerJson_str CoffeeMakerJson;
            CoffeeMakerJsonParser(&CoffeeMakerJson, CoffeeMakerJsonOutPut);
            ApplyOnScreen();
        }
    }
}
