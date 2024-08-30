/**
 ********************************************************************************
 * @file    i2c_init.c
 * @author  Eldin
 * @date    2024-05-12
 * @brief   
 ********************************************************************************
 */

#ifndef MEASURE_SENS_H
#define MEASURE_SENS_H

#ifdef __cplusplus
extern "C" {
#endif

/************************************
 * INCLUDES
 ************************************/
#include "main.h"

/************************************
 * MACROS AND DEFINES
 ************************************/
#define SHT_ADDRESS 0x44

/************************************
 * TYPEDEFS
 ************************************/
typedef struct
{
    uint16_t rawTemp;
    uint16_t rawRH;
    uint8_t temp;
    uint8_t humidity;
    bool checksumErr;
} measurementResult_t;


/************************************
 * EXPORTED VARIABLES
 ************************************/

/************************************
 * GLOBAL FUNCTION PROTOTYPES
 ************************************/
measurementResult_t getHighPrecisionMeasurement(uint8_t i2c_num);

#ifdef __cplusplus
}
#endif

#endif 

/*end of file*/
