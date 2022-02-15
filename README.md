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
1. **PEController_Template**
     - Location: Projects/PEController/Applications/
	 - Description- Template project for creating new quick projects
2. **PELab_OpenLoopVFD**
     - Location: Projects/PEController/Applications/
     - Description- Example for the PELab in 6 Phase Configuration where both the inverters are run simultaneously @ 25Hz with an initial acceleration. The example provides the basic understating to operate the PWM Outputs and accessing the analog data acquired. Refer to the example of an Open Loop VFD Controller in Projects/Applications/PELab_OpenLoopVFD/ for further information

## Making new project from template project
To make a new project for personal development follow the following steps
1. Create a copy of the template folder "PEController_Template" in location Projects/PEController/Applications
2. Rename the copied folder to a custom name such as "TestProject"
3. Select the folder as workspace in Notepad++
4. Select find in files and replace all instances of "PEController_Template" with "TestProject"
5. Rename the following files
	- PEController_Template.ioc to TestProject.ioc in Projects/PEController/Applications/TestProject
	- PEController_Template_CM7.launch to TestProject_CM7.launch in Projects/PEController/Applications/TestProject/CM7
	- PEController_Template_CM4.launch to TestProject_CM4.launch in Projects/PEController/Applications/TestProject/CM4
6. Open the .project file for editing