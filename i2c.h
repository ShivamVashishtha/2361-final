#ifndef I2C_H
#define I2C_H

#include <xc.h>
#include <stdint.h>

#define I2C_SPEED 0x25  // I2C baud rate: 0x25 = 400kHz, 0x9D = 100kHz

/**
 * @brief Initializes I2C1 module and configures pin mapping.
 */
void I2C1_Init(void);

/**
 * @brief Sends a start condition on the I2C bus.
 */
void I2C1_Start(void);

/**
 * @brief Sends a repeated start condition on the I2C bus.
 */
void I2C1_RepeatedStart(void);

/**
 * @brief Sends a stop condition on the I2C bus.
 */
void I2C1_Stop(void);

/**
 * @brief Writes a byte to the I2C bus.
 * @param data Byte to send
 */
void I2C1_Write(uint8_t data);

/**
 * @brief Reads a byte from the I2C bus.
 * @param ack 1 = send ACK after read, 0 = send NACK
 * @return Received byte
 */
uint8_t I2C1_Read(uint8_t ack);

#endif
