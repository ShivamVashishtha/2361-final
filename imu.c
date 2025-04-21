#include "imu.h"
#include "i2c.h"  // Your I2C driver
#include <stdint.h>

#define MPU_ADDR 0x68
#define ACCEL_XOUT_H 0x3B
#define PWR_MGMT_1   0x6B

void IMU_init(void) {
    I2C_start();
    I2C_write(MPU_ADDR << 1);
    I2C_write(PWR_MGMT_1);
    I2C_write(0x00);  // Wake up sensor
    I2C_stop();
}

void IMU_read(IMU_Data* data) {
    uint8_t raw[6];

    I2C_start();
    I2C_write(MPU_ADDR << 1);
    I2C_write(ACCEL_XOUT_H);
    I2C_restart();
    I2C_write((MPU_ADDR << 1) | 1);

    for (int i = 0; i < 6; i++) {
        raw[i] = I2C_read(i < 5);
    }
    I2C_stop();

    int16_t ax = (raw[0] << 8) | raw[1];
    int16_t ay = (raw[2] << 8) | raw[3];
    int16_t az = (raw[4] << 8) | raw[5];

    data->accel_x = ax / 16384.0f;
    data->accel_y = ay / 16384.0f;
    data->accel_z = az / 16384.0f;
}

Gesture detectGesture(const IMU_Data* d) {
    float T = 0.7f;
    if (d->accel_y > T) return GESTURE_UP;
    if (d->accel_y < -T) return GESTURE_DOWN;
    if (d->accel_x > T) return GESTURE_RIGHT;
    if (d->accel_x < -T) return GESTURE_LEFT;
    return GESTURE_NONE;
}
