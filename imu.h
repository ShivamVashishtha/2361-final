#ifndef IMU_H
#define IMU_H

#include <stdint.h>

typedef struct {
    float accel_x;
    float accel_y;
    float accel_z;
} IMU_Data;

typedef enum {
    GESTURE_NONE,
    GESTURE_UP,
    GESTURE_DOWN,
    GESTURE_LEFT,
    GESTURE_RIGHT
} Gesture;

void IMU_init(void);
void IMU_read(IMU_Data* data);
Gesture detectGesture(const IMU_Data* data);

#endif
