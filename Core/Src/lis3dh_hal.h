#ifndef LIS3DH_H
#define LIS3DH_H

#include "stm32l4xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
    float scale;   // mg/LSB
} LIS3DH_Handle_t;


extern LIS3DH_Handle_t accel;


/* Public API */
bool LIS3DH_Init(LIS3DH_Handle_t *dev);
bool LIS3DH_ReadRaw(LIS3DH_Handle_t *dev, int16_t *x, int16_t *y, int16_t *z);
bool LIS3DH_ReadG(LIS3DH_Handle_t *dev, float* x, float* y, float* z);

#endif
