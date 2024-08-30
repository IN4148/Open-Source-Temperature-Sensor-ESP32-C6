/**
 * @file pin_definitions.h
 * @author Eldin
 * @brief Pin definitions for rt1
 * @version 0.1
 * @date 2024-04-01
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PIN_DEFINITIONS_H
#define PIN_DEFINITIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/

/************************************
 * MACROS AND DEFINES
 ************************************/

//Pin Numbers
//#define TESTPUNKT_Pin GPIO_NUM_15 
#define LED_GREEN_Pin GPIO_NUM_15   //Green LED
#define SDA_Pin GPIO_NUM_6      	//I2C SDA
#define SCL_Pin GPIO_NUM_7          //I2C SCL


//for config Bit Masks
#define PinMaskBase     1

//bit masks for pins
#define TESTPUNKT_Mask    (PinMaskBase<<TESTPUNKT_Pin)
#define LED_GREEN_Mask      (PinMaskBase<<LED_GREEN_Pin) 
#define SDA_Mask            (PinMaskBase<<SDA_Pin)
#define SCL_Mask            (PinMaskBase<<SCL_Pin)

/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/

#ifdef __cplusplus
}
#endif

#endif 

/*end of file*/
