#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#define MPU_ADDR 0x68      // I2C address of the MPU6050 IMU
#define THRESH 0.31        // Gesture detection threshold (in g's)

/**
 * @brief Struct to hold scaled accelerometer data (in g).
 */
typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
} IMU_Data;

/**
 * @brief Enum for detected gestures based on IMU input.
 */
typedef enum {
    GESTURE_NONE,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_LEFT,
    GESTURE_RIGHT
} Gesture;

/**
 * @brief Initializes the IMU (wakes it from sleep mode).
 */
void IMU_init(void);

/**
 * @brief Reads and stores scaled accelerometer data from the IMU.
 * 
 * @param data Pointer to IMU_Data struct to store results.
 */
void IMU_read(IMU_Data* data);

/**
 * @brief Detects simple directional gestures based on acceleration.
 * 
 * @param data Pointer to IMU_Data with current readings.
 * @return Detected gesture.
 */
Gesture detectGesture(const IMU_Data* data);

#endif
