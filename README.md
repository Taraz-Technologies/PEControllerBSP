# PEControllerBSP
Board Support Package for PEController module made by Taraz Technologies

## Components
1. **Microcontroller** STM32H745BIT6 (Dual Core CM4/CM7) @ 480MHz
2. **Debug Interface**
     - STLINK-v2
     - SWD
3. **Display**
     - **AFY800480A0-5.0INTH-C / AFY800480B0-5.0N12NTM-C** 5" TFT with RGB interface (800 x 480 resolution) with Capacitive Touch Panel
4. **Communication Protocols**
     - **Ethernet** 10/100Mb (RMII Interface) via RJ45
     - **USB-HS 2.0 OTG** via USB-A Type connector
     - **CAN Interface** via RJ45
     - **Fiber Optic Transmitter Receiver**
     - **Rs485 Interface** via RJ45
     - **SPI**
     - **USART**
     - **I2C**
     - **SD Card Slot**
5. **PWM IOs** 24 Channels
6. **Analog IOs** 18 Channels with 2 on-board MAX11046 ADCs each providing 8 channels

## Supported IDEs
1. STM32CubeIDE

## Examples
1. **PELab_OpenLoopVFD**
     - Location: Projects/Applications/
     - Description- Example for the PELab in 6 Phase Configuration where both the inverters are run simultaneously @ 25Hz with an initial acceleration. The example provides the basic understating to operate the PWM Outputs and accessing the analog data acquired. Refer to the example of an Open Loop VFD Controller in Projects/Applications/PELab_OpenLoopVFD/ for further information
