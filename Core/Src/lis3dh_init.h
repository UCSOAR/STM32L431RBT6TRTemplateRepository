/*
 * lis3dh_init.h
 *
 *  Created on: Oct 15, 2025
 *      Author: Gillian Habermehl
 */

#ifndef INC_LIS3DH_INIT_H_
#define INC_LIS3DH_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_hal.h"
#include "../Drivers/LIS3DH_Driver/lis3dh_reg.h"

// I2C device address for LIS3DH (7-bit address shifted left by 1 for HAL)
#define LIS3DH_I2C_ADDR  (0x18 << 1)

// ---- External Variables ----
extern stmdev_ctx_t dev_ctx;
extern int16_t data_raw_acceleration[3];
extern float acceleration_mg[3];
extern uint8_t reg_status;
extern uint8_t whoamI;

// ---- Function Prototypes ----

/**
 * @brief Initialize the LIS3DH accelerometer.
 *        Configures the device, checks WHO_AM_I, and sets default parameters.
 */
void lis3dh_init(void);

/**
 * @brief Read the latest acceleration data from the LIS3DH sensor.
 *        Updates global arrays: data_raw_acceleration[] and acceleration_mg[].
 */
void lis3dh_read_data(void);

/**
 * @brief Platform-specific write function used by LIS3DH driver.
 * @param handle Pointer to I2C handle.
 * @param reg Register address.
 * @param bufp Pointer to data buffer.
 * @param len Number of bytes to write.
 * @return 0 on success.
 */
int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len);

/**
 * @brief Platform-specific read function used by LIS3DH driver.
 * @param handle Pointer to I2C handle.
 * @param reg Register address.
 * @param bufp Pointer to data buffer.
 * @param len Number of bytes to read.
 * @return 0 on success.
 */
int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* INC_LIS3DH_INIT_H_ */
