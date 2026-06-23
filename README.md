# EEPROM_24CWxx_Library
A C-based library for controlling the **24CWxx** series of I2C Serial EEPROM with Software Write Protection Family - 24CW16x/32x/64x/128x developed by Microchip. This library is optimized for **PIC18** microcontrollers using the **XC8** compiler.

The 24CW series features a memory map based on 32-byte pages. This library implements functions for sequential, paginated reading and writing with boundary safety checks.

## Features
* **Compatibility:** Specifically designed for the 24CWxx series (e.g., 24CW640, 24CW1280).
* **Automatic Page Writing:** Intelligent handling for data blocks larger than 32 bytes, preventing page roll-over issues.
* **Portability:** Modular configuration using preprocessor macros to adapt to different memory densities within the series.
* **Optimization:** Developed with a focus on resource-constrained PIC18 microcontrollers.
* **Robustness:** Includes wait-state routines to ensure internal EEPROM write cycles are fully completed before starting a new operation.

## Hardware Connections
Below is the typical wiring diagram for a 24CWxx series chip with a microcontroller (example based on standard I2C pins):

| 24CWxx EEPROM Pin | Description | Connection to MCU |
| :--- | :--- | :--- |
| **VCC** | Power Supply | 1.8V - 5.5V |
| **GND** | Ground | GND |
| **SDA** | I2C Serial Data | MCU SDA Pin (requires pull-up resistor) |
| **SCL** | I2C Serial Clock | MCU SCL Pin (requires pull-up resistor) |

> **Note:** The 24CWxx series uses a fixed base address on the I2C bus (typically 7-bit `0x50`), ensure pull-up resistors are properly configured on the SDA and SCL lines.

## Usage Example
The following code snippet demonstrates basic usage to initialize the memory, write a data block, and read it back for verification:

```c
#include <xc.h>
#include <stdio.h>
#include "EEPROM_24CWxx.h"

void main(void) {
    // Initialize MCU peripherals (I2C, UART, etc.)
    MCU_Initialize();
    UART1_Initialize(115200);
    I2C2_Initialize(I2C2_FOSC_16MHZ, I2C2_SPEED_100KHZ);
    
    printf("Initializing EEPROM 24CWxx test...\r\n");
    
    // Initialize the EEPROM library
    EEPROMExt_Initialize();
    
    uint16_t target_address = 0x0010;
    uint8_t data_to_write[] = "Hello World 24CW";
    uint8_t data_to_read[20] = {0};
    
    // 1. Read previous content before writing
    printf("Reading previous data...\r\n");
    EEPROMExt_ReadData(target_address, data_to_read, sizeof(data_to_write));
    printf("Current content: %s\r\n", data_to_read);
    
    // 2. Write a block of data (automatically handles 32-byte pages)
    printf("Writing new data...\r\n");
    EEPROMExt_WriteData(target_address, data_to_write, sizeof(data_to_write));
    
    // 3. Verify the recorded data using the built-in library verification
    printf("Verifying recorded data...\r\n");
    if(EEPROMExt_Verify(target_address, data_to_write, sizeof(data_to_write)) == 0) {
        printf("Verification successful! Data matches.\r\n");
    } else {
        printf("Verification failed! Data mismatch.\r\n");
    }
    
    // 4. Read back and final print
    EEPROMExt_ReadData(target_address, data_to_read, sizeof(data_to_write));
    printf("Final content: %s\r\n", data_to_read);

    while(1) {
        // Main loop
    }
}
```

## Project Structure
* `src/`: Core library files (`EEPROM_24CWxx.c`, `EEPROM_24CWxx.h`).
* `LICENSE`: MIT License.
* `CHANGELOG`: History of updates and bug fixes.

## Documentation & Tutorial
For a detailed implementation explanation and step-by-step guide, you can review the following examples:
* https://mrchunckuee.blogspot.com/
