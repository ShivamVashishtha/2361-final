#ifndef WS2812B_H
#define WS2812B_H

void NeoPixel_Init(void);
void NeoPixel_SetColor(int index, uint8_t r, uint8_t g, uint8_t b);
void NeoPixel_Show(void);
void NeoPixel_Clear(void);

#endif
