#include "lis3dh_hal.h"

/* Registers */
#define LIS3DH_WHO_AM_I     0x0F
#define LIS3DH_CTRL_REG1   0x20
#define LIS3DH_OUT_X_L     0x28

#define LIS3DH_WHOAMI_VAL  0x33

LIS3DH_Handle_t accel;

static HAL_StatusTypeDef write_reg(LIS3DH_Handle_t *dev, uint8_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(dev->hi2c,
                             dev->address,
                             reg,
                             I2C_MEMADD_SIZE_8BIT,
                             &val,
                             1,
                             HAL_MAX_DELAY);
}

static HAL_StatusTypeDef read_regs(LIS3DH_Handle_t *dev, uint8_t reg, uint8_t *buf, uint8_t len)
{
    return HAL_I2C_Mem_Read(dev->hi2c,
                            dev->address,
                            reg | 0x80,   // auto-increment
                            I2C_MEMADD_SIZE_8BIT,
                            buf,
                            len,
                            HAL_MAX_DELAY);
}

bool LIS3DH_Init(LIS3DH_Handle_t *dev)
{
    uint8_t whoami;

    if (HAL_I2C_Mem_Read(dev->hi2c, dev->address,
                         LIS3DH_WHO_AM_I, I2C_MEMADD_SIZE_8BIT,
                         &whoami, 1, HAL_MAX_DELAY) != HAL_OK)
        return false;

    if (whoami != LIS3DH_WHOAMI_VAL)
        return false;

    /* 100 Hz, XYZ enabled */
    if (write_reg(dev, LIS3DH_CTRL_REG1, 0x57) != HAL_OK)
        return false;

    /* ±2g default → 1 mg/LSB */
    dev->scale = 0.001f;

    return true;
}

bool LIS3DH_ReadRaw(LIS3DH_Handle_t *dev, int16_t* x, int16_t* y, int16_t* z)
{
    uint8_t raw[6];

    if (read_regs(dev, LIS3DH_OUT_X_L, raw, 6) != HAL_OK)
        return false;

    *x = (int16_t)(raw[1] << 8 | raw[0]);
    *y = (int16_t)(raw[3] << 8 | raw[2]);
    *z = (int16_t)(raw[5] << 8 | raw[4]);

    return true;
}

bool LIS3DH_ReadG(LIS3DH_Handle_t *dev, float *x, float *y, float *z)
{
    int16_t rx, ry, rz;

    if (!LIS3DH_ReadRaw(dev, &rx, &ry, &rz))
        return false;

    *x = rx * dev->scale;
    *y = ry * dev->scale;
    *z = rz * dev->scale;

    return true;
}
