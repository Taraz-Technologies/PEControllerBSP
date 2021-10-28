Open Loop VFD Control Example:

This example uses both CM4 and CM7 core simultaneously. The CM4 core starts and then goes to sleep. The CM7 core waits for CM4 to go to sleep and then initializes the system. Once initialized CM7 wakesup the the CM4 core.

CM4:
1. Displays the LOGO on screen using the LTDC module.
2. Configures the peripherals to acquire the ADC data periodically. The period can be controlled by the CM7 during initializaiton by the statement "sharedData->m7Tom4.periodUs = 20;" where 20 is the acquistion period in Usecs.
3. The ADC data is shared between both cores. The ADC data variable "sharedData->lastDataPointer" contains the most recent data for all the ADC channels respectively. The scaling factors for these channels are configured according to the PELab 6 Phase configuration in the file MAX11046App.c

CM7:
1. Provides Open Loop VFD Control on 2 VFDs independently.
	- VFD1 PWMs Available on GPIO1-GPIO6 
	- VFD1 PWMs Available on GPIO7-GPIO12


Important Note:
The project consists of two individual projects.  (PELab_OpenLoopVFD_CM4 and PELab_OpenLoopVFD_CM7). According to configuration the CM4 project is just used to produce a binary file whereas the CM7 core is used to burn both binaries of CM4 and CM7. Make sure that for debugging and runnning target the project used is CM7. This is done to provide the capability of simultaneous debugging of both cores in one environment for STM32CubeIDE. The following links can be used for further understanding debugging in STM32H7 dual core controllers
1. https://www.youtube.com/watch?v=k3mXhPZSasw
2. https://www.youtube.com/watch?v=jI1k6p-fduE
