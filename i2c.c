#include "i2c.h"

void I2C1_Init(void) {
    I2C1BRG = 37;               // For 100kHz at 16MHz FCY
    I2C1CONbits.I2CEN = 1;      // Enable I2C1 module
}

void I2C1_Start(void) {
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN);
}

void I2C1_RepeatedStart(void) {
    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN);
}

void I2C1_Stop(void) {
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN);
}

void I2C1_Write(uint8_t data) {
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT);    // Wait for transmission
    while (I2C1STATbits.ACKSTAT);   // Wait for ACK
}

uint8_t I2C1_Read(uint8_t ack) {
    I2C1CONbits.RCEN = 1;
    while (!I2C1STATbits.RBF);      // Wait for data
    uint8_t val = I2C1RCV;

    I2C1CONbits.ACKDT = ack ? 0 : 1; // 0 = ACK, 1 = NACK
    I2C1CONbits.ACKEN = 1;
    while (I2C1CONbits.ACKEN);

    return val;
}
