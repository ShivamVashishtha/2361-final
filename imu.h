#ifndef IMU_H
#define IMU_H

#include <stdint.h>

// === Data Structures ===
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
} IMU_Data;

typedef enum {
    GESTURE_NONE,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_LEFT,
    GESTURE_RIGHT
} Gesture;

// === Public API ===
void IMU_init(void);                     // Initializes MPU6050
void IMU_read(IMU_Data* data);           // Reads raw accelerometer and gyroscope data
void IMU_calibrate(void);                // Calibrates sensor for offsets
Gesture detectGesture(const IMU_Data*);  // Returns a gesture based on input data
void IMU_applyFilter(IMU_Data* data);    // Applies smoothing or noise filtering

#endif
