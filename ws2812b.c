#include "ws2812b.h"
#include <xc.h>
#include <libpic30.h>

#define DATA_PIN LATBbits.LATB0
#define TRIS_PIN TRISBbits.TRISB0

uint8_t led[3] = {0};

void sendBit(int bitVal) {
    if (bitVal) {
        DATA_PIN = 1;
        __delay_us(0.8);
        DATA_PIN = 0;
        __delay_us(0.45);
    } else {
        DATA_PIN = 1;
        __delay_us(0.4);
        DATA_PIN = 0;
        __delay_us(0.85);
    }
}

void sendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        sendBit((byte >> i) & 1);
    }
}

void NeoPixel_Init(void) {
    TRIS_PIN = 0;
    DATA_PIN = 0;
}

void NeoPixel_SetColor(int index, uint8_t r, uint8_t g, uint8_t b) {
    led[0] = g;
    led[1] = r;
    led[2] = b;
}

void NeoPixel_Show(void) {
    for (int i = 0; i < 3; i++) {
        sendByte(led[i]);
    }
    __delay_us(50); // Reset
}

void NeoPixel_Clear(void) {
    NeoPixel_SetColor(0, 0, 0, 0);
    NeoPixel_Show();
}
