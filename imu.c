#include "imu.h"
#include "i2c.h"

/**
 * @brief Wakes up the MPU6050 IMU using I2C.
 */
void IMU_init(void) {
    I2C1_Start();
    I2C1_Write(MPU_ADDR << 1);
    I2C1_Write(0x6B);     // PWR_MGMT_1
    I2C1_Write(0x00);     // Wake up
    I2C1_Stop();
}

/**
 * @brief Reads and scales accelerometer data from MPU6050.
 * 
 * @param data Pointer to store scaled accel values (in g).
 */
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

/**
 * @brief Detects gesture based on accel direction and threshold.
 * 
 * @param data Pointer to accel data.
 * @return Detected gesture enum.
 */
Gesture detectGesture(const IMU_Data* data) {    
    float absAccelX = (data->accel_x >= 0) ? data->accel_x : -data->accel_x;
    float absAccelY = (data->accel_y >= 0) ? data->accel_y : -data->accel_y;

    float maxAccel = (absAccelX > absAccelY) ? absAccelX : absAccelY;
    
    if (maxAccel < THRESH) return GESTURE_NONE;

    if (absAccelY == maxAccel) {
        if (data->accel_y > 0) return GESTURE_UP;
        if (data->accel_y < 0) return GESTURE_DOWN;
    } else if (absAccelX == maxAccel) {
        if (data->accel_x > 0) return GESTURE_RIGHT;
        if (data->accel_x < 0) return GESTURE_LEFT;
    }
    
    return GESTURE_NONE;
}
