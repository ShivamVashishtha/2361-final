#include "xc.h"
#include "neopixel.h"
#include "imu.h"
#include "uart.h"
#include <stdio.h>

// CW1: FLASH CONFIGURATION WORD 1
#pragma config ICS = PGx1
#pragma config FWDTEN = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF
#pragma config JTAGEN = OFF

// CW2: FLASH CONFIGURATION WORD 2
#pragma config I2C1SEL = PRI
#pragma config IOL1WAY = OFF
#pragma config OSCIOFNC = ON
#pragma config FCKSM = CSECME
#pragma config FNOSC = FRCPLL

void setup(void)
{
    CLKDIVbits.RCDIV = 0;

    __builtin_write_OSCCONL(OSCCON & 0xBF);  // Unlock PPS
    RPINR20bits.SDA1R = 9;                  // SDA1 on RB9
    RPOR4bits.RP8R = 0b1000;                // SCL1 on RB8
    __builtin_write_OSCCONL(OSCCON | 0x40); // Lock PPS

    neopixel_init();
    setLeds(1);
    setBrightness(50);
    clear();

    I2C1_Init();
    IMU_init();
    UART1_Init();
}

char buffer[64];

int main(void)
{
    IMU_Data imu;
    Gesture g, last = GESTURE_NONE;

    setup();

    while (1) {
        IMU_read(&imu);
        g = detectGesture(&imu);

        if (g != last) {
            last = g;

            switch (g) {
                case GESTURE_UP:    setStrip(0, 255, 0); break;
                case GESTURE_DOWN:  setStrip(255, 0, 0); break;
                case GESTURE_LEFT:  setStrip(0, 0, 255); break;
                case GESTURE_RIGHT: setStrip(255, 255, 0); break;
                default:            setStrip(0, 0, 0); break;
            }

            show(); // Only update LEDs if gesture changed
        }

        sprintf(buffer, "X: %.2f Y: %.2f Z: %.2f\r\n", imu.accel_x, imu.accel_y, imu.accel_z);
        UART1_Print(buffer);

        __delay_ms(50); // Faster response
    }
}
