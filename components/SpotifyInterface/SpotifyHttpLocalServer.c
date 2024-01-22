#include "SpotifyHttpLocalServer.h"
#include "esp_mac.h"
static const char *TAG = "SpotifyTask";
// ******************************
HttpLocalServerParam_t HttpLocalServerLocalParam;

/**
 * @brief This function handles the first HTTPS request to Spotify and redirects the user to the authorization page.
 * @param[in] req The HTTP request object.
 * @return Returns ESP_OK if the request is processed successfully.
 */
static esp_err_t Spotify_RequestDataAccess(httpd_req_t *req)
{
    char *LocalURL;
    LocalURL = (char *)malloc((2 * SMALL_BUF) * sizeof(char));
    if (LocalURL == NULL)
    {
        ESP_LOGI(TAG, "Failed to allocate memory for the array.\n\n");
    }
    memset(LocalURL, 0x0, SMALL_BUF * 2);
    if (((*HttpLocalServerLocalParam.status) == IDLE))
    {

        ESP_LOGI(TAG, "Starting authorization, sending request for TOKEN");
        sprintf(LocalURL, "http://accounts.spotify.com/authorize/?client_id=%s&response_type=code&redirect_uri=%s&scope=user-read-private%%20user-read-currently-playing%%20user-read-playback-state%%20user-modify-playback-state", ClientId, ReDirectUri);
        httpd_resp_set_hdr(req, "Location", LocalURL);
        httpd_resp_set_type(req, "text/plain");
        httpd_resp_set_status(req, "302");
        httpd_resp_send(req, "", HTTPD_RESP_USE_STRLEN);
    }
    else
    {
        ESP_LOGW(TAG, "Spotify is already initiated");
    }
    free(LocalURL);
    return ESP_OK;
}

/**
 * @brief This function handles the callback HTTPS request from Spotify and processes the response data.
 * @param[in] req The HTTP request object.
 * @return Returns ESP_OK if the request is processed successfully.
 */
static esp_err_t Spotify_HttpsCallbackHandler(httpd_req_t *req)
{
    char Buf[SMALL_BUF*2];
    if (httpd_req_get_url_query_str(req, Buf, sizeof(Buf)) == ESP_OK)
    {
        if (Spotify_FindCode(Buf, sizeof(Buf)) == true)
        {
            if (xQueueSend(*(HttpLocalServerLocalParam.SendCodeFromHttpToSpotifyTask), Buf, portMAX_DELAY) != pdTRUE)
            {
                ESP_LOGE(TAG, "Sent data with queue failed !");
            }
            ESP_LOGI(TAG, "the CODE found in response");
            httpd_resp_set_type(req, "text/plain");
            httpd_resp_set_status(req, HTTPD_200);
            httpd_resp_send(req, Buf, HTTPD_RESP_USE_STRLEN);
            (*HttpLocalServerLocalParam.status) = AUTHENTICATED;
        }
        else
        {
            ESP_LOGE(TAG, "response does not include code at the beginning ");
            (*HttpLocalServerLocalParam.status) = IDLE;
            return ESP_FAIL;
        }
    }
    else
    {
        httpd_resp_set_type(req, "text/plain");
        httpd_resp_set_status(req, HTTPD_500);
        httpd_resp_send(req, Buf, HTTPD_RESP_USE_STRLEN);
        ESP_LOGW(TAG, "bad arguments - the response does not include correct structure");
        (*HttpLocalServerLocalParam.status) = IDLE;
    }
    return ESP_OK;
}

/**
 * this strcut is http URL handler if receive "/" Spotify_RequestDataAccess getting run
 */
static const httpd_uri_t Spotify_Request_Access_URI = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = Spotify_RequestDataAccess};

/**
 * this strcut is http URL handler if receive "/callback" HttpsUserCallBackFunc getting run
 */
static const httpd_uri_t Spotify_Response_Access_URI = {
    .uri = "/callback/",
    .method = HTTP_GET,
    .handler = Spotify_HttpsCallbackHandler};

/**
 * @brief Setup parameter for starting Http Local server
 * @param[in] HttpLocalServerParam_t HttpLocalServerParam_t
 */
void Spotify_SetupHttpLocalServer(HttpLocalServerParam_t HttpLocalServerParam_t)
{
    HttpLocalServerLocalParam.SendCodeFromHttpToSpotifyTask = HttpLocalServerParam_t.SendCodeFromHttpToSpotifyTask;
    HttpLocalServerLocalParam.status = HttpLocalServerParam_t.status;
}

/**
 * @brief This function starts the web server for handling HTTPS requests.
 * @return Returns the HTTP server handle if it is started successfully, or NULL otherwise.
 */
httpd_handle_t Spotify_StartWebServer()
{
    httpd_handle_t LocalServer = NULL;
    httpd_config_t Config = HTTPD_DEFAULT_CONFIG();
    Config.lru_purge_enable = true;
    ESP_LOGI(TAG, "Starting server on port: '%d'", Config.server_port);
    if (httpd_start(&LocalServer, &Config) == ESP_OK)
    {
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(LocalServer, &Spotify_Request_Access_URI);
        httpd_register_uri_handler(LocalServer, &Spotify_Response_Access_URI);
        return LocalServer;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief This function stops the web server for handling HTTPS requests.
 * @return Returns the HTTP server handle if it is started successfully, or NULL otherwise.
 */
 esp_err_t Spotify_StopSpotifyWebServer(httpd_handle_t server)
{
    return httpd_stop(server);
}

static char *generate_hostname(void);
/**
 * @brief This function starts the mDNS service.
 */
bool Spotify_StartMDNSService()
{
     esp_err_t err = mdns_init();
    // if (err)
    // {
    //     ESP_LOGE(TAG, "MDNS Init failed: %d", err);
    //     return false;
    // }
    // else
    // {
    //     mdns_hostname_set("deskhub");
    //     mdns_instance_name_set("Spotify");
    //     return true;
    // }
    
        
    //mdns_hostname_set("deskhub");
    //mdns_instance_name_set("spotify");
    //mdns_service_add("deskhub", "_http", "_tcp", 80, NULL, 0);

    //structure with TXT records
    mdns_txt_item_t serviceTxtData[3] = {
        {"board", "esp32"},
        {"u", "user"},
        {"p", "password"}
    };

    char *delegated_hostname;
    //char *hostname = malloc(50 * sizeof(char));//generate_hostname();    
    char hostname[50];
    ESP_ERROR_CHECK( mdns_hostname_get(hostname));
    ESP_LOGE(TAG, "MDNS HostName: %s", hostname);
    if (-1 == asprintf(&delegated_hostname, "%s-delegated", hostname)) {
        abort();
    }
    ESP_LOGE(TAG, "MDNS HostName-delegated: %s", delegated_hostname);
    mdns_ip_addr_t addr4, addr6;
    esp_netif_str_to_ip4("127.0.0.1", &addr4.addr.u_addr.ip4);
    addr4.addr.type = ESP_IPADDR_TYPE_V4;
    esp_netif_str_to_ip6("fd11:22::1", &addr6.addr.u_addr.ip6);
    addr6.addr.type = ESP_IPADDR_TYPE_V6;
    addr4.next = &addr6;
    addr6.next = NULL;
    ESP_ERROR_CHECK( mdns_delegate_hostname_add(delegated_hostname, &addr4) );
    ESP_ERROR_CHECK( mdns_service_add_for_host(NULL, "_http", "_tcp", delegated_hostname, 80, serviceTxtData, 3) );
    free(delegated_hostname);    
    //free(hostname);    

    return true;
}

static char *generate_hostname(void)
{
// #ifndef CONFIG_MDNS_ADD_MAC_TO_HOSTNAME
//     return strdup(CONFIG_MDNS_HOSTNAME);
// #else
    uint8_t mac[6];
    char   *hostname;
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    if (-1 == asprintf(&hostname, "%s-%02X%02X%02X", "CONFIG_MDNS_HOSTNAME", mac[3], mac[4], mac[5])) {
        abort();
    }
    return hostname;
//#endif
}