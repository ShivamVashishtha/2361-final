#include <xc.h>
#include <stdio.h>
#include "uart.h"

void UART1_Init(void) {
    U1MODE = 0;
    U1BRG = 25; // For FCY=16MHz, 9600 baud
    U1STA = 0x0400;
    U1MODEbits.UARTEN = 1;
}

void UART1_Print(const char* str) {
    while (*str) {
        while (U1STAbits.UTXBF); // Wait if buffer full
        U1TXREG = *str++;
    }
}
