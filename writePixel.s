.include "xc.inc"

.text                       ; BP (put the following data in ROM(program memory))

; Exported symbols
.global _write_0, _write_1, _wait_100us, _wait_1ms

; LATB, bit 6 = NeoPixel output
; 20 total cycles = 1.25us
_write_0:               ; 2 cycle call
    ; 6 cycle high
    bset    LATB, #6    ; 1 cycle
    repeat  #3          ; 1 cycle
    nop                 ; 3 + 1 cycles

    ; 14 cycle low
    bclr    LATB, #6    ; 1 cycle
    repeat  #6          ; 1 cycle
    nop                 ; 6 + 1 cycles

    return              ; 3 cycles to return

; 20 total cycles = 1.25us
_write_1:               ; 2 cycle call
    ; 13 cycle high
    bset    LATB, #6    ; 1 cycle
    repeat  #10         ; 1 cycle
    nop                 ; 10 + 1 cycles

    ; 7 cycle low
    bclr    LATB, #6    ; 1 cycle
    nop                 ; 1 cycle
    return              ; 3 cycles to return

; Delay ~100us (adjust for FCY = 16MHz)
_wait_100us:            ; 2 cycle call
    repeat  #1593       ; 1 cycle
    nop                 ; 1593 + 1 cycles
    return              ; 3 cycles to return

; Delay ~1ms (adjust for FCY = 16MHz)
_wait_1ms:              ; 2 cycle call
    repeat  #15993      ; 1 cycle
    nop                 ; 15993 + 1 cycles
    return              ; 3 cycles to return
