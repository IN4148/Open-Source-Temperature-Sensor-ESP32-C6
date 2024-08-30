/**
 ********************************************************************************
 * @file    i2c_init.h 
 * @author  Eldin
 * @date    2024-05-12
 * @brief   
 ********************************************************************************
 */

#ifndef I2C_INIT_H
#define I2C_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

#include "pin_definitions.h"
#include "driver/i2c.h" 
#include "esp_log.h"

/************************************
 * MACROS AND DEFINES
 ************************************/

#define I2C_MASTER_FREQ_HZ 400000

#define I2C_TIMEOUT	(1000/portTICK_PERIOD_MS) // I2C command timeout

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
 * @brief initializes I2C and installs I2C driver
 * 
 */
esp_err_t I2C_Init(void);

#ifdef __cplusplus
}
#endif

#endif 

/*end of file*/