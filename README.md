# STM32F4xx Bare-Metal SDK

This project is a library of drivers for the STM32F407xx microcontroller, written from scratch by accessing registers directly. This approach avoids using the standard ST HAL or LL libraries to achieve maximum performance and deep understanding of the hardware.

Project Goal:
I created this SDK to master the ARM Cortex-M4 architecture and understand how software interacts directly with hardware. I believe that to become a capable engineer, one must understand the fundamentals of technology rather than just relying on high-level libraries.

Implemented Peripherals:

* GPIO
* 
* 
* 



Project Structure:

STM32F4xx\_Drivers/

├── Drivers/

│   ├── Inc/

│   │   ├── stm32f407xx.h

│   │   └── stm32f407xx\_gpio\_driver.h

│   └── Src/

│       └── stm32f407xx\_gpio\_driver.c

├── Src/

│   ├── main.c

│   ├── syscalls.c

│   └── sysmem.c

├── Startup/

│   └── startup\_stm32f407vgtx.s

├── .gitignore

├── LICENSE

├── README.md

├── STM32F407VGTX\_FLASH.ld

└── STM32F407VGTX\_RAM.ld

