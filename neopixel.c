#include <xc.h>
#include "neopixel.h"

uint8_t numLeds = 1;            // Number of leds to control
uint8_t brightness = 100;       // Default brightness (max)
uint8_t led_strip[BUFFER][3];   // Buffer to hold led info

void neopixel_init(void) {
    TRISBbits.TRISB6 = 0;
}

void setLeds(uint8_t num) {
    numLeds = num;
}

void setBrightness(uint8_t percent) {
    if (percent > 100) percent = 100;
    brightness = percent;
}

void customDelay(unsigned int ms) {
    for (int i = 0; i < ms; i++) {
        wait_1ms();
    }
}

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

void show() {
    for (uint8_t i = 0; i < numLeds; i++) {
        writeColor(led_strip[i][0], led_strip[i][1], led_strip[i][2]);
    }
    wait_100us();
}

void setPixel(uint8_t index, uint8_t g, uint8_t r, uint8_t b) {
    if (index < numLeds) {
        led_strip[index][0] = (g * brightness) / 100;
        led_strip[index][1] = (r * brightness) / 100;
        led_strip[index][2] = (b * brightness) / 100;
    }
}

void setStrip(uint8_t g, uint8_t r, uint8_t b) {
    for (uint8_t i = 0; i < numLeds; i++) {
        setPixel(i, g, r, b);
    }
}

void clear() {
    for (uint8_t i = 0; i < BUFFER; i++) {
        writeColor(0, 0, 0);
    }
    show();
}

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

void rgbColorShiftWhole(uint8_t delay_ms) {
    static uint8_t hue = 0;
    uint8_t r, g, b;

    colorWheel(hue, &r, &g, &b);
    setStrip(g, r, b);
    show();
    customDelay(delay_ms);

    hue++;
}

void rgbGradientShift(uint8_t delay_ms) {
    static uint8_t offset = 0;
    uint8_t r, g, b;

    for (uint8_t i = 0; i < numLeds; i++) {
        uint8_t hue = (i * 256 / numLeds + offset) & 0xFF;
        colorWheel(hue, &r, &g, &b);
        setPixel(i, g, r, b);
    }

    show();
    customDelay(delay_ms);

    offset++;
}
