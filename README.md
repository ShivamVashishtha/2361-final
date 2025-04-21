# 2361-final


Peripheral | Function | Pin Name (PIC24FJ64GA002) | Direction | Notes
NeoPixel | Data out | RB6 | Output | Controlled via write_0/write_1
MPU6050 | I2C SDA | RC4 (SDA1) | Bidirectional | Default I2C1 SDA
MPU6050 | I2C SCL | RC3 (SCL1) | Output | Default I2C1 SCL
UART1 TX | Serial output | RB2 (U1TX) | Output | Sends data to PC for debug
UART1 RX | Serial input | RB1 (U1RX) | Input | Not used in this project





Example Wiring Summary:


Component | To PIC24 Pin | Other Connections
MPU6050 SDA | RC4 | 4.7kΩ pull-up to 3.3V
MPU6050 SCL | RC3 | 4.7kΩ pull-up to 3.3V
WS2812B Data | RB6 | Use level shifter if 5V LED
UART TX (debug) | RB2 | Connect to USB-Serial adapter
GND | GND | Shared among all devices
VCC | 3.3V (MPU), 5V (NeoPixel) | From regulated source
