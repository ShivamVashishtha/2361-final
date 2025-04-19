#include "imu.h"
#include "ws2812b.h"

int main(void) {
    IMU_Data data;
    IMU_init();
    NeoPixel_Init();
    NeoPixel_Clear();

    while (1) {
        IMU_read(&data);
        Gesture g = detectGesture(&data);

        switch (g) {
            case GESTURE_UP:
                NeoPixel_SetColor(0, 0, 255, 0);  // Green
                break;
            case GESTURE_DOWN:
                NeoPixel_SetColor(0, 255, 0, 0);  // Red
                break;
            case GESTURE_LEFT:
                NeoPixel_SetColor(0, 0, 0, 255);  // Blue
                break;
            case GESTURE_RIGHT:
                NeoPixel_SetColor(0, 255, 255, 0); // Yellow
                break;
            default:
                NeoPixel_SetColor(0, 0, 0, 0);    // Off
                break;
        }

        NeoPixel_Show();
        __delay_ms(100);
    }
}
