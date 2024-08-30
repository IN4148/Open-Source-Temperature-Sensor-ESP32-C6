/**
 * @file gpio_init.c
 * @author Eldin
 * @brief init esp32 gpio pins
 * @version 0.1
 * @date 2024-04-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/************************************
 * INCLUDES
 ************************************/

#include "gpio_init.h"

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

//config structs
gpio_config_t LED_GREEN_Pin_Config;
gpio_config_t TESTPUNKT_Pin_Config;
gpio_config_t INT_TOF_Pin_Config;
gpio_config_t SDA_Pin_Config;
gpio_config_t INT_IO_Pin_Config;

gpio_config_t CHARGE_STAT_Pin_Config;
gpio_config_t SCK_Pin_Config;
gpio_config_t SDO_Pin_Config;
gpio_config_t DC_Pin_Config;
gpio_config_t SCL_Pin_Config;
gpio_config_t CS_Pin_Config;

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

esp_err_t RGB_CONTROL_Init(void);


/************************************
 * STATIC FUNCTIONS
 ************************************/

esp_err_t LED_GREEN_Init(void){
    LED_GREEN_Pin_Config.pin_bit_mask = LED_GREEN_Mask;
    LED_GREEN_Pin_Config.mode = GPIO_MODE_INPUT_OUTPUT;
    LED_GREEN_Pin_Config.pull_up_en = GPIO_PULLUP_DISABLE;
    LED_GREEN_Pin_Config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    LED_GREEN_Pin_Config.intr_type = GPIO_INTR_DISABLE; 
    if(gpio_config(&LED_GREEN_Pin_Config)!=ESP_OK){
        return ESP_FAIL;
    }
    return gpio_set_level(LED_GREEN_Pin, 0);
}


//esp_err_t CS_Init(void){}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

esp_err_t IO_Init(void){
    ESP_LOGI("GPIO", "GPIO init");

    if(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1)!=ESP_OK)return ESP_FAIL;

    if(LED_GREEN_Init()!=ESP_OK)return ESP_FAIL;

    ESP_LOGI("GPIO", "GPIO init complete");
    return ESP_OK;
}

/*end of file*/