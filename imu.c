#include "imu.h"
#include "i2c.h"

#define MPU_ADDR 0x68
#define ALPHA 0.3  // Smoothing factor

static IMU_Data smoothed = {0};

void IMU_init(void) {
    I2C1_Start();
    I2C1_Write(MPU_ADDR << 1);
    I2C1_Write(0x6B);     // PWR_MGMT_1
    I2C1_Write(0x00);     // Wake up
    I2C1_Stop();
}

void IMU_read(IMU_Data* data) {
    uint8_t raw[6];
    I2C1_Start();
    I2C1_Write(MPU_ADDR << 1);
    I2C1_Write(0x3B); // ACCEL_XOUT_H
    I2C1_RepeatedStart();
    I2C1_Write((MPU_ADDR << 1) | 1);

    for (int i = 0; i < 6; i++) {
        raw[i] = I2C1_Read(i < 5);
    }
    I2C1_Stop();

    int16_t ax = (raw[0] << 8) | raw[1];
    int16_t ay = (raw[2] << 8) | raw[3];
    int16_t az = (raw[4] << 8) | raw[5];

    data->accel_x = ax / 16384.0f;
    data->accel_y = ay / 16384.0f;
    data->accel_z = az / 16384.0f;
}

void IMU_applyFilter(IMU_Data* data) {
    smoothed.accel_x = smoothed.accel_x * (1 - ALPHA) + data->accel_x * ALPHA;
    smoothed.accel_y = smoothed.accel_y * (1 - ALPHA) + data->accel_y * ALPHA;
    smoothed.accel_z = smoothed.accel_z * (1 - ALPHA) + data->accel_z * ALPHA;

    data->accel_x = smoothed.accel_x;
    data->accel_y = smoothed.accel_y;
    data->accel_z = smoothed.accel_z;
}

Gesture detectGesture(const IMU_Data* data) {
    const float THRESH = 1.0;

    if (data->accel_y > THRESH) return GESTURE_UP;
    if (data->accel_y < -THRESH) return GESTURE_DOWN;
    if (data->accel_x > THRESH) return GESTURE_RIGHT;
    if (data->accel_x < -THRESH) return GESTURE_LEFT;

    return GESTURE_NONE;
}
