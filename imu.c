#include "imu.h"
#include "i2c.h"      // Assume you have I2C functions like I2C_write(), I2C_read()
#include <math.h>

// MPU6050 Registers
#define MPU_ADDR       0x68
#define PWR_MGMT_1     0x6B
#define ACCEL_XOUT_H   0x3B

// Calibration offsets (to be tuned or calculated)
static float accel_offset_x = 0;
static float accel_offset_y = 0;
static float accel_offset_z = 0;

void IMU_init(void) {
    I2C_start();
    I2C_write(MPU_ADDR << 1);     // Write mode
    I2C_write(PWR_MGMT_1);
    I2C_write(0x00);              // Wake up MPU6050
    I2C_stop();
}

void IMU_read(IMU_Data* data) {
    uint8_t raw[14];
    I2C_start();
    I2C_write(MPU_ADDR << 1);
    I2C_write(ACCEL_XOUT_H);
    I2C_restart();
    I2C_write((MPU_ADDR << 1) | 1);
    for (int i = 0; i < 14; i++) {
        raw[i] = I2C_read(i < 13);
    }
    I2C_stop();

    // Convert raw to physical values (assuming ±2g and ±250°/s ranges)
    data->accel_x = ((int16_t)(raw[0] << 8 | raw[1])) / 16384.0f - accel_offset_x;
    data->accel_y = ((int16_t)(raw[2] << 8 | raw[3])) / 16384.0f - accel_offset_y;
    data->accel_z = ((int16_t)(raw[4] << 8 | raw[5])) / 16384.0f - accel_offset_z;

    data->gyro_x = ((int16_t)(raw[8] << 8 | raw[9])) / 131.0f;
    data->gyro_y = ((int16_t)(raw[10] << 8 | raw[11])) / 131.0f;
    data->gyro_z = ((int16_t)(raw[12] << 8 | raw[13])) / 131.0f;
}

void IMU_calibrate(void) {
    // Average multiple samples for offset calibration
    IMU_Data sum = {0};
    for (int i = 0; i < 100; i++) {
        IMU_Data temp;
        IMU_read(&temp);
        sum.accel_x += temp.accel_x;
        sum.accel_y += temp.accel_y;
        sum.accel_z += temp.accel_z;
        __delay_ms(10);  // Add delay as needed
    }
    accel_offset_x = sum.accel_x / 100.0f;
    accel_offset_y = sum.accel_y / 100.0f;
    accel_offset_z = sum.accel_z / 100.0f;
}

void IMU_applyFilter(IMU_Data* data) {
    // Example: Simple moving average filter or exponential smoothing
    // Implement if needed based on noise level
    // You could also use a low-pass filter here
}

Gesture detectGesture(const IMU_Data* data) {
    // Thresholds for gesture detection (tune these)
    const float THRESHOLD = 0.8f;

    if (data->accel_y > THRESHOLD) return GESTURE_UP;
    if (data->accel_y < -THRESHOLD) return GESTURE_DOWN;
    if (data->accel_x > THRESHOLD) return GESTURE_RIGHT;
    if (data->accel_x < -THRESHOLD) return GESTURE_LEFT;

    return GESTURE_NONE;
}
