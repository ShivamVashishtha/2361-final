#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdint.h>
#include "writePixel.h"

#define BUFFER 255  // Max number of LEDs in the strip

// Initialization
void neopixel_init(void);                      // Initialize NeoPixel pin
void setLeds(uint8_t num);                    // Set the number of LEDs

// LED control
void setBrightness(uint8_t percent);                           // Sets global brightness in percentage
void setPixel(uint8_t index, uint8_t g, uint8_t r, uint8_t b); // Set color of a single LED (GRB order)
void setStrip(uint8_t g, uint8_t r, uint8_t b);                // Set entire strip to a single color
void writeColor(uint8_t g, uint8_t r, uint8_t b);              // Send GRB data to NeoPixel
void show(void);                                               // Update strip with current buffer data
void clear(void);                                              // Turn off all LEDs

// Effects
void rgbColorShiftWhole(uint8_t delay_ms);     // Shift entire strip through rainbow colors
void rgbGradientShift(uint8_t delay_ms);       // Animate a gradient rainbow across the strip

// Timing
void customDelay(unsigned int ms);             // Custom delay in milliseconds

#endif
