#include "i2c.h"

/**
 * @brief Initializes I2C1 module with PPS and sets baud rate.
 */
void I2C1_Init(void) {
    __builtin_write_OSCCONL(OSCCON & 0xBF);      // Unlock PPS
    RPINR20bits.SDI1R1 = 0x9;                    // RB9 = SDA1 (input)
    RPOR4bits.RP8R = 0b1000;                     // RB8 = SCL1 (output)
    __builtin_write_OSCCONL(OSCCON | 0x40);      // Lock PPS
    
    I2C1BRG = I2C_SPEED;                         // Set baud rate (e.g., 100kHz)
    I2C1CONbits.I2CEN = 1;                       // Enable I2C1 module
}

/**
 * @brief Sends an I2C start condition.
 */
void I2C1_Start(void) {
    I2C1CONbits.SEN = 1;
    while (I2C1CONbits.SEN);                     // Wait until start completes
}

/**
 * @brief Sends an I2C repeated start condition.
 */
void I2C1_RepeatedStart(void) {
    I2C1CONbits.RSEN = 1;
    while (I2C1CONbits.RSEN);                    // Wait until repeat start completes
}

/**
 * @brief Sends an I2C stop condition.
 */
void I2C1_Stop(void) {
    I2C1CONbits.PEN = 1;
    while (I2C1CONbits.PEN);                     // Wait until stop completes
}

/**
 * @brief Writes a byte over I2C and waits for ACK.
 * @param data Byte to send
 */
void I2C1_Write(uint8_t data) {
    I2C1TRN = data;
    while (I2C1STATbits.TRSTAT);                 // Wait for transmission to finish
    while (I2C1STATbits.ACKSTAT);                // Wait for ACK (ACKSTAT == 0 if acknowledged)
}

/**
 * @brief Reads a byte from I2C and sends ACK or NACK.
 * @param ack 1 = send ACK, 0 = send NACK
 * @return Received byte
 */
uint8_t I2C1_Read(uint8_t ack) {
    I2C1CONbits.RCEN = 1;
    while (!I2C1STATbits.RBF);                   // Wait for byte received
    uint8_t val = I2C1RCV;

    I2C1CONbits.ACKDT = ack ? 0 : 1;             // ACKDT: 0 = ACK, 1 = NACK
    I2C1CONbits.ACKEN = 1;                       // Start ACK/NACK sequence
    while (I2C1CONbits.ACKEN);                   // Wait until ACK/NACK completes

    return val;
}
