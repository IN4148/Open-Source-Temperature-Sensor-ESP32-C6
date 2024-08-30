/**
 ********************************************************************************
 * @file    i2c_init.c
 * @author  Eldin
 * @date    2024-05-12
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/

#include "i2c_init.h" 

/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

i2c_config_t i2c_conf = {
    .mode = I2C_MODE_MASTER,
    .sda_io_num = SDA_Pin,         // select SDA GPIO specific to your project
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_io_num = SCL_Pin,         // select SCL GPIO specific to your project
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
    .master.clk_speed = I2C_MASTER_FREQ_HZ,  // select frequency specific to your project
    .clk_flags = 0,                          // optional; you can use I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here
};

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/

/************************************
 * GLOBAL FUNCTIONS
 ************************************/

esp_err_t I2C_Init(void){
    ESP_LOGI("I2C", "I2C Init"); 
    if(i2c_param_config(I2C_NUM_0, &i2c_conf) != ESP_OK){
        ESP_LOGE("I2C", "I2C Init failed");
        return ESP_FAIL;
    }

    if(i2c_driver_install(I2C_NUM_0, i2c_conf.mode, 0, 0, 0) != ESP_OK){ //no buffersize necessary for master/ All interrupts are disabled
        ESP_LOGE("I2C", "I2C Init failed");
        return ESP_FAIL;
    }

    ESP_LOGI("I2C", "I2C init complete"); 
    return ESP_OK;
}

/*end of file*/