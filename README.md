# STM32F4xx Bare-Metal SDK

This project is a library of drivers for the STM32F407xx microcontroller, written from scratch by accessing registers directly. This approach avoids using the standard ST HAL or LL libraries to achieve maximum performance and deep understanding of the hardware.

## Project Goal  
I developed this bare-metal SDK to truly master the embedded systems and bridge the gap between low-level software and custom physical hardware. By writing register-level drivers from scratch and intentionally bypassing abstractions like ST's HAL or LL, this project serves as a deep dive into embedded fundamentals. My core belief is that a highly capable engineer doesn't just rely on high-level APIs, but understands the exact silicon behavior, interrupt handling, and hardware timing required to make custom PCBs function flawlessly.

## Implemented Peripherals
* GPIO
* SPI 
* I2C 
* UART/USART

## How to Run the Demos
This project also includes a folder with demo apps for each peripheral supported by the custom driver layer.
To run a demo correctly:
1. Ensure that the default `main.c` is **excluded from build**.
2. Ensure the desired demo file (e.g. `GPIODemoApp.c`) is **included** in the project's source path.

If you would like to test/use the SDK yourself, I have included a template `main.c` in `STM32F4xx_Drivers/Src` which, by default, is added to the project's source path.

Feel free to check the *Media* section to see some photos/videos regarding the demo apps.

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
    ├── BSP/
    │   ├── ds1307.c
    │   ├── ds1307.h
    │   ├── lcd1602.c
    │   └── lcd1602.h
    ├── Drivers/
    │   ├── Inc/
    │   │   ├── stm32f407xx.h
    │   │   ├── stm32f407xx_gpio_driver.h
    │   │   ├── stm32f407xx_i2c_driver.h
    │   │   ├── stm32f407xx_rcc_driver.h
    │   │   ├── stm32f407xx_spi_driver.h
    │   │   └── stm32f407xx_usart_driver.h
    │   └── Src/
    │       ├── stm32f407xx_gpio_driver.c
    │       ├── stm32f407xx_i2c_driver.c
    │       ├── stm32f407xx_rcc_driver.c
    │       ├── stm32f407xx_spi_driver.c
    │       └── stm32f407xx_usart_driver.c
    ├── Src/
    │   ├── DemoApps/
    │   │   ├── GPIODemoApp.c
    │   │   └── SPIDemoApp.c
    │   ├── main.c
    │   ├── syscalls.c
    │   └── sysmem.c
    └── Startup/
        └── startup_stm32f407vgtx.s
```

## Media
Here you can see how the driver works in the real world.

### GPIO Demo:
<table>
  <tr>
    <td>
      <video src="https://github.com/user-attachments/assets/0eaaf08d-a02e-4c46-b223-fc03fe36dc49" controls="controls" muted="muted"></video>
    </td>
  </tr>
</table>


### SPI Demo:
<table>
  <tr>
    <td>
      <video src="https://github.com/user-attachments/assets/bd4ce241-62c8-4e86-b4d0-b417185c5484" controls="controls" muted="muted"></video>
    </td>
    <td>
        <img width="400"  alt="display_pcb" src="https://github.com/user-attachments/assets/cddabe86-a051-41b4-92d3-b9613fe8ece7"/>
    </td>
  </tr>
</table>







