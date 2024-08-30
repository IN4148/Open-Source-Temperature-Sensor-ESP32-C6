/**
 * @file gpio_init.h
 * @author Eldin
 * @brief init esp32 gpio pins
 * @version 0.1
 * @date 2024-04-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

#include "pin_definitions.h"
#include "driver/gpio.h"
#include "esp_log.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

/**
 * @brief init all gpio pins
 * 
 */
esp_err_t IO_Init(void);

#ifdef __cplusplus
}
#endif

#endif 

/*end of file*/
