/*
 * lis3dh_init.c
 *
 *  Created on: Oct 15, 2025
 *      Author: gillianhabermehl
 */


#include "../Drivers/LIS3DH_Driver/lis3dh_reg.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_i2c.h"
#include <string.h>


extern I2C_HandleTypeDef hi2c2;

#define LIS3DH_I2C_ADDR  (0x18 << 1)

int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
  HAL_I2C_Mem_Write(&hi2c2, LIS3DH_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)bufp, len, 1000);
  return 0;
}

int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
  HAL_I2C_Mem_Read(&hi2c2, LIS3DH_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
  return 0;
}


stmdev_ctx_t dev_ctx;
uint8_t whoamI = 0;
lis3dh_reg_t reg;

void lis3dh_init(void)
{
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg  = platform_read;
  dev_ctx.handle    = &hi2c2;

  // Check device ID
  lis3dh_device_id_get(&dev_ctx, &whoamI);
  if (whoamI != LIS3DH_ID)
  {
    // handle error (sensor not detected)
    Error_Handler();
  }

  // Reset to default
  lis3dh_reset_set(&dev_ctx, PROPERTY_ENABLE);
  HAL_Delay(10);

  // Enable X, Y, Z axes and set data rate
  lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_10Hz);
  lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g);
  lis3dh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit);
}


int16_t data_raw_acceleration[3];
float acceleration_mg[3];
uint8_t reg_status;

void lis3dh_read_data(void)
{
  lis3dh_status_reg_t status;
  lis3dh_status_get(&dev_ctx, &status);

  if (status.zyxda)
  {
      lis3dh_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
      acceleration_mg[0] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[0]);
      acceleration_mg[1] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[1]);
      acceleration_mg[2] = lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[2]);
  }
}




