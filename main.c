#include "xc.h"
#include "neopixel.h"
#include "imu.h"
#include "i2c.h"
#include <math.h>

// CW1
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF

// CW2
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL

void setup(void) {
    CLKDIVbits.RCDIV = 0;

    neopixel_init();
    setLeds(15);
    setBrightness(50);
    setSpeed(2);
    clear();

    I2C1_Init();
    IMU_init();

    // Setup RB4 as button input with pull-up
    TRISBbits.TRISB4 = 1;          // Input
    CNPU1bits.CN1PUE = 1;          // Enable internal pull-up
}

int clamp(int val) {
    if (val < 0) return 0;
    if (val > 255) return 255;
    return val;
}

int main(void) {
    IMU_Data imu;
    Gesture g;
    int r = 0, g_col = 0, b = 0;
    int target_r = 0, target_g = 0, target_b = 0;
    int mode = 0;  // 0 = rainbow + gestures, 1 = RGB tilt
    int prevButtonState = 1;

    setup();

    while (1) {
        IMU_read(&imu);

        // Detect button press (active LOW)
        int button = PORTBbits.RB4;
        if (prevButtonState == 1 && button == 0) {
            mode ^= 1;  // Toggle mode
            customDelay(300);  // Debounce delay
        }
        prevButtonState = button;

        if (mode == 1) {
            // Mode 1: RGB tilt mode
            target_r = clamp((int)((imu.accel_x + 1.0) * 127.5));
            target_g = clamp((int)((imu.accel_y + 1.0) * 127.5));
            target_b = clamp((int)((imu.accel_z + 1.0) * 127.5));

            // Smooth transition
            r += (target_r - r) / 4;
            g_col += (target_g - g_col) / 4;
            b += (target_b - b) / 4;

            setStrip(r, g_col, b);
            show();
        } else {
            // Mode 0: Rainbow gradient + gestures
            g = detectGesture(&imu);
            switch (g) {
                case GESTURE_UP:    changeBrightness(5);  customDelay(400); break;
                case GESTURE_DOWN:  changeBrightness(-5); customDelay(400); break;
                case GESTURE_LEFT:  changeSpeed(-1);      customDelay(400); break;
                case GESTURE_RIGHT: changeSpeed(1);       customDelay(400); break;
            }

            rgbGradientShift(1);
            show();
        }

        customDelay(50);
    }
}
