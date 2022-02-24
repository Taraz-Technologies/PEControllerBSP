# PEControllerBSP
Board Support Package for PEController module made by Taraz Technologies

## Documentation
https://www.taraztechnologies.com/Downloads/Software/PEControllerBSP/index.html

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

## BSP Structure
1. **Drivers:**
	- *BSP:* Contains the Board Support Drivers provided by Taraz Technologies
		- *PEController:* Contains the drivers for PEController board
			- *ADC:* Contains the drivers for ADC 
			- *Common:* Contains the commonly used files
			- *Components:* Contains the other components used by the BSP
			- *DigitalPins:* Contains the drivers for the digital Pins
			- *Display:* Contains the display drivers
			- *intelliSENS:* Contains the drivers for integration with intelliSENS software
			- *PWM:* Contains the PWM drivers
	- *CMSIS:* Contains the drivers provided by CMSIS 
	- *STM32H7xx_HAL_Driver:* Contains the drivers provided by ST
2. **Middleware**
	- *ST:* Helper libraries provided by ST
	- *Taraz:* Helper libraries provided by Taraz Technologies
		- *ControlLib:* Control System library containing different coordinate systems and transformations
	- *Third_Party:* Third party libraries
3. **Projects**
	- *PEController:* 
		- *Applications:* 
			- *PEController_Template:*  Template project for creating new quick projects
			- *PELab_OpenLoopVFD:* Basic Implementation of Open Loop V/f Control Implemented for different variants of PELab


## Making new project from template project
To make a new project for personal development follow the following steps
1. Create a copy of the template folder "PEController_Template" in location Projects/PEController/Applications
2. Rename the copied folder to a custom name such as "TestProject"
3. Open Notepad++
4. Select find in files and select Projects/PEController/Applications/TestProject as directory 
5. Replace all instances of "PEController_Template" with "TestProject"
6. Rename the following files
	- PEController_Template.ioc to TestProject.ioc in Projects/PEController/Applications/TestProject
	- PEController_Template_CM7.launch to TestProject_CM7.launch in Projects/PEController/Applications/TestProject/CM7
	- PEController_Template_CM4.launch to TestProject_CM4.launch in Projects/PEController/Applications/TestProject/CM4
7. Open the .project file for editing