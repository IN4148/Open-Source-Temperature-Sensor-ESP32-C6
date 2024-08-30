/**
 ********************************************************************************
 * @file    i2c_init.c
 * @author  Eldin
 * @date    2024-05-12
 * @brief
 * @copyright Copyright (c) 2024   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "measure_sensor.h"

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

uint8_t calculate_crc(const uint8_t *data, int length) {
    uint8_t crc = 0xFF;  // Initial value for CRC
    for (int i = 0; i < length; i++) {
        crc ^= data[i];  // XOR-in the next input byte
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {  // If the MSB is set
                crc = (crc << 1) ^ 0x31;  // Left shift and XOR with polynomial
            } else {
                crc <<= 1;  // Left shift only
            }
        }
    }
    return crc;
}

measurementResult_t getHighPrecisionMeasurement(uint8_t i2c_num){
    uint8_t data[] = {
        0xFD
    };
    //writei2c(start measurement);
    if(i2c_master_write_to_device(i2c_num, SHT_ADDRESS, &data[0], sizeof(data), I2C_TIMEOUT) != ESP_OK){
        ESP_LOGE("SHT", "Write Error");
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
    uint8_t readBuffer[6];
    if (i2c_master_read_from_device(i2c_num, SHT_ADDRESS, &readBuffer[0], sizeof(readBuffer), I2C_TIMEOUT) != ESP_OK){
        ESP_LOGE("SHT", "Read Error");
    }

    //data = readi2c(result register);
    measurementResult_t sensorData = {
        .rawTemp    = (uint16_t)readBuffer[0] << 8 | readBuffer[1],
        .rawRH      = (uint16_t)readBuffer[3] << 8 | readBuffer[4],
        .temp       = 0,    //split up for better text readability
        .humidity   = 0,    //split up for better text readability
        .checksumErr = 0
    };
    //parse data into measurementResult_t

    if (calculate_crc(readBuffer, 2) != readBuffer[2]) {
        ESP_LOGE("Checksum", "CRC error in temperature data");
        sensorData.checksumErr = true;
    }
    else if (calculate_crc(readBuffer + 3, 2) != readBuffer[5]) {
        ESP_LOGE("Checksum", "CRC error in humidity data");
        sensorData.checksumErr = true;
    }
    else {
    sensorData.temp = -45 + 175*sensorData.rawTemp / (65535);       // Nominator 2^16 -1
    sensorData.temp = sensorData.temp - 5;                          // Offset -5°C
    sensorData.humidity = -6 + 125*sensorData.rawRH / (65535);      // Nominator 2^16 -1
    if (sensorData.temp > 100){
        sensorData.temp = 100; 
    } 
    if (sensorData.temp < 0){
         sensorData.temp = 0;
    }
    sensorData.checksumErr = false;
    }


    ESP_LOGD("SHT", "%d, %d", sensorData.temp, sensorData.humidity);
    return sensorData;
}

/*end of file*/