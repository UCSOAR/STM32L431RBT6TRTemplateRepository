#include "stm32l4xx_hal.h"
#include "lis3dh_utils.h"


extern I2C_HandleTypeDef hi2c2; // your I2C handle

#define LIS3DH_ADDR      (0x18 << 1)  // or 0x19
#define LIS3DH_OUT_X_L   0x28
#define LIS3DH_WHO_AM_I  0x0F

bool lis3dh_read_xyz(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t raw[6];

    // auto-increment: read X, Y, Z registers in one go
    if(HAL_I2C_Mem_Read(&hi2c2, LIS3DH_ADDR, LIS3DH_OUT_X_L | 0x80,
                        I2C_MEMADD_SIZE_8BIT, raw, 6, 100) != HAL_OK) {
        return false;
    }

    *x = (int16_t)(raw[1] << 8 | raw[0]);
    *y = (int16_t)(raw[3] << 8 | raw[2]);
    *z = (int16_t)(raw[5] << 8 | raw[4]);
    return true;
}
