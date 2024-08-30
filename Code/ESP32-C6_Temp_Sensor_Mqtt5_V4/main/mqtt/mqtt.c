/**
 * @file mqtt.c
 * @author Eldin
 * @brief 
 * @version 0.1
 * @date 2024-05-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/************************************
 * INCLUDES
 ************************************/

#include "mqtt.h"

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

static const char *TAG = "MQTT";

static esp_mqtt5_user_property_item_t user_property_arr[] = {
    {"board", "esp32"},
    {"u", "user"},
    {"p", "password"}
};

#define USE_PROPERTY_ARR_SIZE   sizeof(user_property_arr)/sizeof(esp_mqtt5_user_property_item_t)

static esp_mqtt5_publish_property_config_t publish_property = {
    .payload_format_indicator = 1,
    .message_expiry_interval = 1000,
    .topic_alias = 0,
    .response_topic = "/ServerRoom/SensorStatus",
    .correlation_data = "Connected",
    .correlation_data_len = 9,
};

static esp_mqtt5_subscribe_property_config_t subscribe_property = {
    .subscribe_id = 25555,
    .no_local_flag = false,
    .retain_as_published_flag = false,
    .retain_handle = 0,
    .is_share_subscribe = true,
    .share_name = "group1",
};

static esp_mqtt5_subscribe_property_config_t subscribe1_property = {
    .subscribe_id = 25555,
    .no_local_flag = true,
    .retain_as_published_flag = false,
    .retain_handle = 0,
};

static esp_mqtt5_unsubscribe_property_config_t unsubscribe_property = {
    .is_share_subscribe = true,
    .share_name = "group1",
};

static esp_mqtt5_disconnect_property_config_t disconnect_property = {
    .session_expiry_interval = 120,
    .disconnect_reason = 0,
};

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

/************************************
 * STATIC FUNCTIONS
 ************************************/

/*static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}



static void print_user_property(mqtt5_user_property_handle_t user_property)
{
    if (user_property) {
        uint8_t count = esp_mqtt5_client_get_user_property_count(user_property);
        if (count) {
            esp_mqtt5_user_property_item_t *item = malloc(count * sizeof(esp_mqtt5_user_property_item_t));
            if (esp_mqtt5_client_get_user_property(user_property, item, &count) == ESP_OK) {
                for (int i = 0; i < count; i ++) {
                    esp_mqtt5_user_property_item_t *t = &item[i];
                    ESP_LOGI(TAG, "key is %s, value is %s", t->key, t->value);
                    free((char *)t->key);
                    free((char *)t->value);
                }
            }
            free(item);
        }
    }
}*/

void measurementTask(void *pvParameters){
    esp_mqtt_client_handle_t client = (esp_mqtt_client_handle_t)pvParameters;
    int msg_id;
    char dataToSend[20]; // Adjust buffer size as per your requirement
    
    while(true){
        gpio_set_level(LED_GREEN_Pin, 1);
        measurementResult_t result = getHighPrecisionMeasurement(I2C_NUM_0);
        ESP_LOGI(TAG, "Temperature: %d dC; Humidity: %d rH", result.temp, result.humidity);

        if(result.checksumErr == false){

            int len = snprintf(dataToSend, sizeof(dataToSend), "%d", result.temp);
            if (len < 0 || len >= sizeof(dataToSend)) {
                ESP_LOGE(TAG, "Failed to format temperature data");
            } else {
                // Publish temperature data
                msg_id = esp_mqtt_client_publish(client, "/ServerRoom/Temperature", dataToSend, 0, 0, 1); //QoS = 0
                if (msg_id < 0) {
                    ESP_LOGE(TAG, "Failed to publish temperature data");
                    // Handle error if publish fails
                    // You might want to retry or take other appropriate action
                } else {
                    ESP_LOGI(TAG, "Sent publish successful, msg_id=%d", msg_id);
                }
            }

            len = snprintf(dataToSend, sizeof(dataToSend), "%d", result.humidity);
            if (len < 0 || len >= sizeof(dataToSend)) {
                ESP_LOGE(TAG, "Failed to format humidity data");
            } else {
                msg_id = esp_mqtt_client_publish(client, "/ServerRoom/Humidity", dataToSend, 0, 0, 1); //QoS = 0
                if (msg_id < 0) {
                    ESP_LOGE(TAG, "Failed to publish humidity data");
                } else {
                    ESP_LOGI(TAG, "Sent publish successful, msg_id=%d", msg_id);
                }
            }
        }

        gpio_set_level(LED_GREEN_Pin, 0);
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

void mqtt5_app_start(void)
{
    esp_mqtt5_connection_property_config_t connect_property = {
        .session_expiry_interval = 10,
        .maximum_packet_size = 1024,
        .receive_maximum = 65535,
        .topic_alias_maximum = 2,
        .request_resp_info = true,
        .request_problem_info = true,
        .will_delay_interval = 10,
        .payload_format_indicator = true,
        .message_expiry_interval = 10,
        .response_topic = "/ServerRoom/Temperature",
        .correlation_data = "Temperature",
        .correlation_data_len = 11,
    };

    esp_mqtt_client_config_t mqtt5_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
        .session.protocol_ver = MQTT_PROTOCOL_V_5,
        .network.disable_auto_reconnect = false,
        .credentials.username = "Esp32-C6",
        .credentials.authentication.password = "SensorNr1",
        .session.last_will.topic = "/ServerRoom/SensorStatus",
        .session.last_will.msg = "Disconnected",
        .session.last_will.msg_len = 12,
        .session.last_will.qos = 2, //QoS = 2
        .session.last_will.retain = true,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt5_cfg);

    /* Set connection properties and user properties */
    esp_mqtt5_client_set_user_property(&connect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_user_property(&connect_property.will_user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
    esp_mqtt5_client_set_connect_property(client, &connect_property);

    /* If you call esp_mqtt5_client_set_user_property to set user properties, DO NOT forget to delete them.
     * esp_mqtt5_client_set_connect_property will malloc buffer to store the user_property and you can delete it after
     */
    esp_mqtt5_client_delete_user_property(connect_property.user_property);
    esp_mqtt5_client_delete_user_property(connect_property.will_user_property);

    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt5_event_handler, NULL);
    esp_mqtt_client_start(client);

    g_mqtt_scanning = true;
}


TaskHandle_t task_handle = NULL;
static void mqtt5_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    
    ESP_LOGD(TAG, "free heap size is %" PRIu32 ", minimum %" PRIu32, esp_get_free_heap_size(), esp_get_minimum_free_heap_size());
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        g_mqtt_scanning = false;

        /*print_user_property(event->property->user_property);
        esp_mqtt5_client_set_user_property(&publish_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
        esp_mqtt5_client_set_publish_property(client, &publish_property);*/
        msg_id = esp_mqtt_client_publish(client, "/ServerRoom/SensorStatus", "Connected", 0, 1, 1);
        //esp_mqtt5_client_delete_user_property(publish_property.user_property);
        //publish_property.user_property = NULL;
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        //we now create the measurement task
        xTaskCreate(measurementTask,
                "MeasurementTask",
                4000,
                client,                   
                tskIDLE_PRIORITY,
                &task_handle
        );

        /*esp_mqtt5_client_set_user_property(&subscribe_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
        esp_mqtt5_client_set_subscribe_property(client, &subscribe_property);
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        esp_mqtt5_client_delete_user_property(subscribe_property.user_property);
        subscribe_property.user_property = NULL;
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        esp_mqtt5_client_set_user_property(&subscribe1_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
        esp_mqtt5_client_set_subscribe_property(client, &subscribe1_property);
        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 2);
        esp_mqtt5_client_delete_user_property(subscribe1_property.user_property);
        subscribe1_property.user_property = NULL;
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        esp_mqtt5_client_set_user_property(&unsubscribe_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
        esp_mqtt5_client_set_unsubscribe_property(client, &unsubscribe_property);
        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos0");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        esp_mqtt5_client_delete_user_property(unsubscribe_property.user_property);
        unsubscribe_property.user_property = NULL;*/
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        g_mqtt_scanning = true;
        
        //kill the task
        if(task_handle != NULL){
            vTaskDelete(task_handle);
            ESP_LOGI(TAG, "Task killed");
        }
        
        //print_user_property(event->property->user_property);
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        /*print_user_property(event->property->user_property);
        esp_mqtt5_client_set_publish_property(client, &publish_property);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);*/
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        /*print_user_property(event->property->user_property);
        esp_mqtt5_client_set_user_property(&disconnect_property.user_property, user_property_arr, USE_PROPERTY_ARR_SIZE);
        esp_mqtt5_client_set_disconnect_property(client, &disconnect_property);
        esp_mqtt5_client_delete_user_property(disconnect_property.user_property);
        disconnect_property.user_property = NULL;
        esp_mqtt_client_disconnect(client);*/
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        //print_user_property(event->property->user_property);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        /*print_user_property(event->property->user_property);
        ESP_LOGI(TAG, "payload_format_indicator is %d", event->property->payload_format_indicator);
        ESP_LOGI(TAG, "response_topic is %.*s", event->property->response_topic_len, event->property->response_topic);
        ESP_LOGI(TAG, "correlation_data is %.*s", event->property->correlation_data_len, event->property->correlation_data);
        ESP_LOGI(TAG, "content_type is %.*s", event->property->content_type_len, event->property->content_type);
        ESP_LOGI(TAG, "TOPIC=%.*s", event->topic_len, event->topic);
        ESP_LOGI(TAG, "DATA=%.*s", event->data_len, event->data);*/
        break;
    case MQTT_EVENT_ERROR:
        g_mqtt_scanning = true;
        ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
        


        /*print_user_property(event->property->user_property);
        ESP_LOGI(TAG, "MQTT5 return code is %d", event->error_handle->connect_return_code);
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }*/
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        g_mqtt_scanning = true;
        break;
    }
}

/*end of file*/