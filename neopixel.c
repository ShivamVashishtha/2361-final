#include <xc.h>
#include "neopixel.h"

// Default configuration
uint8_t numLeds = 1;            // Number of LEDs to control
uint8_t brightness = 100;       // Brightness (0?100%)
uint8_t speed = 1;              // Animation speed
uint8_t led_strip[BUFFER][3];   // RGB values for each LED

/**
 * @brief Initializes the data pin for NeoPixel output.
 */
void neopixel_init(void) {
    TRISBbits.TRISB6 = 0;
}

/**
 * @brief Sets the number of active LEDs.
 */
void setLeds(uint8_t num) {
    numLeds = num;
}

/**
 * @brief Sets brightness (0?100%).
 */
void setBrightness(uint8_t percent) {
    if (percent > 100) percent = 100;
    if (percent < 0) percent = 0;
    brightness = percent;
}

/**
 * @brief Increments/decrements brightness by a value.
 */
void changeBrightness(int percent) {
    int changed = brightness + percent;
    if (changed > 100) changed = 100;
    if (changed < 0) changed = 0;
    brightness = changed;
}

/**
 * @brief Sets animation speed (0?20).
 */
void setSpeed(uint8_t value) {
    if (value > 20) value = 20;
    if (value < 0) value = 0;
    speed = value;
}

/**
 * @brief Increments/decrements animation speed.
 */
void changeSpeed(int value) {
    int changed = speed + value;
    if (changed > 20) changed = 20;
    if (changed < 0) changed = 0;
    speed = changed;
}

/**
 * @brief Simple blocking delay in milliseconds.
 */
void customDelay(unsigned int ms) {
    for (int i = 0; i < ms; i++) {
        wait_1ms();
    }
}

/**
 * @brief Sends a single RGB color to the LED strip.
 */
void writeColor(uint8_t g, uint8_t r, uint8_t b) {
    for (uint8_t i = 0; i < 8; i++) {
        (g & 0x80) ? write_1() : write_0();
        g <<= 1;
    }
    for (uint8_t i = 0; i < 8; i++) {
        (r & 0x80) ? write_1() : write_0();
        r <<= 1;
    }
    for (uint8_t i = 0; i < 8; i++) {
        (b & 0x80) ? write_1() : write_0();
        b <<= 1;
    }
}

/**
 * @brief Sends all buffered LED values to the strip.
 */
void show() {
    for (uint8_t i = 0; i < numLeds; i++) {
        writeColor(led_strip[i][0], led_strip[i][1], led_strip[i][2]);
    }
    wait_100us();  // Latch
}

/**
 * @brief Sets color of a single LED in the buffer.
 */
void setPixel(uint8_t index, uint8_t g, uint8_t r, uint8_t b) {
    if (index < numLeds) {
        led_strip[index][0] = (g * brightness) / 100;
        led_strip[index][1] = (r * brightness) / 100;
        led_strip[index][2] = (b * brightness) / 100;
    }
}

/**
 * @brief Sets all LEDs to the same color.
 */
void setStrip(uint8_t g, uint8_t r, uint8_t b) {
    for (uint8_t i = 0; i < numLeds; i++) {
        setPixel(i, g, r, b);
    }
}

/**
 * @brief Clears the LED strip (sets all LEDs to off).
 */
void clear() {
    for (uint8_t i = 0; i < BUFFER; i++) {
        writeColor(0, 0, 0);
    }
    show();
}

/**
 * @brief Generates RGB values from a position on the color wheel.
 * 
 * @param pos Wheel position (0?255)
 * @param r Pointer to store red value
 * @param g Pointer to store green value
 * @param b Pointer to store blue value
 */
void colorWheel(uint8_t pos, uint8_t* r, uint8_t* g, uint8_t* b) {
    pos = 255 - pos;
    if (pos < 85) {
        *r = 255 - pos * 3;
        *g = 0;
        *b = pos * 3;
    } else if (pos < 170) {
        pos -= 85;
        *r = 0;
        *g = pos * 3;
        *b = 255 - pos * 3;
    } else {
        pos -= 170;
        *r = pos * 3;
        *g = 255 - pos * 3;
        *b = 0;
    }
}

/**
 * @brief Shifts the entire strip color through the rainbow.
 */
void rgbColorShiftWhole(uint8_t delay_ms) {
    static uint8_t hue = 0;
    uint8_t r, g, b;

    colorWheel(hue, &r, &g, &b);
    setStrip(g, r, b);
    show();
    customDelay(delay_ms);

    hue++;
}

/**
 * @brief Creates a rainbow gradient across the strip that moves over time.
 */
void rgbGradientShift(uint8_t delay_ms) {
    static uint8_t offset = 0;
    uint8_t r, g, b;

    for (uint8_t i = 0; i < numLeds; i++) {
        uint8_t hue = ((i * 256) / numLeds + offset) & 0xFF;
        colorWheel(hue, &r, &g, &b);
        setPixel(i, g, r, b);
    }

    show();
    customDelay(delay_ms);

    offset = offset + speed;
}
