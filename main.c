#include "xc.h"
#include "neopixel.h"
#include "imu.h"
#include "i2c.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void setup(void)
{
    CLKDIVbits.RCDIV = 0;  // 1:1 clock division (16 MHz FCY)

    __builtin_write_OSCCONL(OSCCON & 0xBF);      // Unlock PPS
    RPINR20bits.SDI1R1 = 9;                       // RB9 = SDA1 (input)
    RPOR4bits.RP8R = 0b1000;                     // RB8 = SCL1 (output)
    __builtin_write_OSCCONL(OSCCON | 0x40);      // Lock PPS


    neopixel_init();
    setLeds(15);
    setBrightness(50);
    clear();

    I2C1_Init();    // Assuming I2C1_Init() is defined elsewhere
    IMU_init();
}

char buffer[64];

int main(void)
{
    IMU_Data imu;
    Gesture g;
    setup();

    while (1) {
        IMU_read(&imu);
        g = detectGesture(&imu);

        switch (g) {
            case GESTURE_UP: setStrip(0, 255, 0); customDelay(500); break;
            case GESTURE_DOWN: setStrip(255, 0, 0); customDelay(500); break;
            case GESTURE_LEFT: setStrip(0, 0, 255); customDelay(500); break;
            case GESTURE_RIGHT: setStrip(255, 255, 0); customDelay(500); break;
            // default: setStrip(0, 0, 0); break;
        }
        show();
        customDelay(10);
    }
}