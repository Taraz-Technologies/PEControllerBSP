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
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/CM4" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/ADCs" -I"C:/TarazWorkDir/TempLoc/PEControllerBSP/Projects/Applications/PELab_OpenLoopVFD/Common" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

