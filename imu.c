#include "imu.h"
#include "i2c.h"
#define MPU_ADDR 0x68

void IMU_init(void) {
    I2C1_Start();                       // Start I2C communication
    I2C1_Write(MPU_ADDR << 1);         // Write address (0xD0)
    I2C1_Write(0x6B);                  // Register: PWR_MGMT_1
    I2C1_Write(0x00);                  // Value: wake up the MPU6050
    I2C1_Stop();                       // End I2C transaction
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

Gesture detectGesture(const IMU_Data* data) {
    const float THRESH = 0.8;
    if (data->accel_y > THRESH) return GESTURE_UP;
    if (data->accel_y < -THRESH) return GESTURE_DOWN;
    if (data->accel_x > THRESH) return GESTURE_RIGHT;
    if (data->accel_x < -THRESH) return GESTURE_LEFT;
    return GESTURE_NONE;
}
