# STM32F4xx Bare-Metal SDK

This project is a library of drivers for the STM32F407xx microcontroller, written from scratch by accessing registers directly. This approach avoids using the standard ST HAL or LL libraries to achieve maximum performance and deep understanding of the hardware.

## Project Goal  
I created this SDK to master the ARM Cortex-M4 architecture and understand how software interacts directly with hardware. I believe that to become a capable engineer, one must understand the fundamentals of technology rather than just relying on high-level libraries.

## Implemented Peripherals
* GPIO
* SPI *(in progress)*
* I2C *(in progress)*
* UART/USART *(in progress)*

## How to Run the Demos
This project also includes a folder with demo apps for each peripheral supported by the custom driver layer.
To run a demo correctly:
1. Ensure that the default `main.c` is **excluded from build**.
2. Ensure the desired demo file (e.g., `GPIODemoApp.c`) is **included** in the project's source path.

If you would like to test/use the SDK yourself, I have included a template `main.c` in `STM32F4xx_Drivers/Src` which, by default, is added to the project's source path.

## Project Structure 
```text
STM32F4xx_BareMetal_SDK/
├── .gitignore
├── LICENSE
├── README.md
└── STM32F4xx_Drivers/
    ├── .cproject
    ├── .project
    ├── STM32F407VGTX_FLASH.ld
    ├── STM32F407VGTX_RAM.ld
    ├── Drivers/
    │   ├── Inc/
    │   │   ├── stm32f407xx.h
    │   │   └── stm32f407xx_gpio_driver.h
    │   └── Src/
    │       └── stm32f407xx_gpio_driver.c
    ├── Src/
    │   ├── DemoApps/
    │   │   └── GPIODemoApp.c
    │   ├── main.c
    │   ├── syscalls.c
    │   └── sysmem.c
    └── Startup/
        └── startup_stm32f407vgtx.s
