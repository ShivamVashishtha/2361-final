#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdint.h>
#include "writePixel.h"

#define BUFFER 255  // Max number of LEDs in the strip

// Initialization
/**
 * @brief Initializes the NeoPixel output pin.
 */
void neopixel_init(void);

/**
 * @brief Sets the number of active LEDs.
 * @param num Number of LEDs to control
 */
void setLeds(uint8_t num);

// LED control
/**
 * @brief Sets brightness (0?100%).
 * @param percent Brightness percentage
 */
void setBrightness(uint8_t percent);

/**
 * @brief Adjusts brightness by a relative value.
 * @param percent Amount to change brightness by
 */
void changeBrightness(int percent);

/**
 * @brief Sets animation speed (0?20).
 * @param value Speed value
 */
void setSpeed(uint8_t value);

/**
 * @brief Adjusts animation speed by a relative value.
 * @param value Amount to change speed by
 */
void changeSpeed(int value);

/**
 * @brief Sets color of a single LED in GRB order.
 * @param index LED index
 * @param g Green component
 * @param r Red component
 * @param b Blue component
 */
void setPixel(uint8_t index, uint8_t g, uint8_t r, uint8_t b);

/**
 * @brief Sets all LEDs to the same color in GRB order.
 * @param g Green component
 * @param r Red component
 * @param b Blue component
 */
void setStrip(uint8_t g, uint8_t r, uint8_t b);

/**
 * @brief Sends GRB color data to the strip.
 * @param g Green
 * @param r Red
 * @param b Blue
 */
void writeColor(uint8_t g, uint8_t r, uint8_t b);

/**
 * @brief Pushes buffered LED data to the strip.
 */
void show(void);

/**
 * @brief Turns off all LEDs.
 */
void clear(void);

// Effects
/**
 * @brief Shifts the whole strip through the rainbow.
 * @param delay_ms Delay between shifts
 */
void rgbColorShiftWhole(uint8_t delay_ms);

/**
 * @brief Animates a rainbow gradient across the strip.
 * @param delay_ms Delay between shifts
 */
void rgbGradientShift(uint8_t delay_ms);

// Timing
/**
 * @brief Custom blocking delay in milliseconds.
 * @param ms Duration to delay
 */
void customDelay(unsigned int ms);

#endif
