################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/Common/SharedMemory.c 

OBJS += \
./Common/SharedMemory.o 

C_DEPS += \
./Common/SharedMemory.d 


# Each subdirectory must supply rules for building sources it contributes
Common/SharedMemory.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/Common/SharedMemory.c Common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Control" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/Drivers/ModifiedDrivers" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/ADCs" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/Common" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/CM7" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

