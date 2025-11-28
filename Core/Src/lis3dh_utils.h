#pragma once
#include <stdint.h>
#include <stdbool.h>


// Reads X, Y, Z accelerometer data from LIS3DH via I2C
// Returns true if read succeeded, false on error
bool lis3dh_read_xyz(int16_t* x, int16_t* y, int16_t* z);

