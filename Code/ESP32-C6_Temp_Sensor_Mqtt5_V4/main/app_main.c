/**
 ********************************************************************************
 * @file    i2c_init.c
 * @author  Eldin
 * @date    2024-05-12
 * @brief   
 ********************************************************************************
 */

#include "main.h"

bool g_mqtt_scanning = false;

void toggleLED(){
    gpio_set_level(LED_GREEN_Pin, !gpio_get_level(LED_GREEN_Pin));
    ESP_LOGI("MAIN", "blink");
}

void app_main(void)
{
    ESP_LOGI("MAIN", "Startup..");

    ESP_ERROR_CHECK(nvs_flash_init());

    //init io
    ESP_ERROR_CHECK(IO_Init());

    //init i2c
    ESP_ERROR_CHECK(I2C_Init());

    /*esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);*/


    ESP_ERROR_CHECK(wifi_connect()); 

    ESP_LOGW("MAIN", "Using URL: %s", CONFIG_BROKER_URL);

    mqtt5_app_start();

    bool timerStarted = false;
    TimerHandle_t blinkTimer = xTimerCreate(
        "BlinkTimer", 
        150/portTICK_PERIOD_MS, 
        true, 
        0, 
        toggleLED
    );

    while(1){
        if(g_mqtt_scanning){
            if(!timerStarted) { xTimerStart(blinkTimer, 0); timerStarted=true; }
        }
        else{
            if(timerStarted) { xTimerStop(blinkTimer, 0); timerStarted=false; }
            gpio_set_level(LED_GREEN_Pin, 0);
        }

        vTaskDelay(25/portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL); //terminate this task
}
