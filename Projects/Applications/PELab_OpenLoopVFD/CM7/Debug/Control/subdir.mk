################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DinController.c \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DioController.c \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DoutController.c \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/Inverter3Ph.c 

OBJS += \
./Control/DinController.o \
./Control/DioController.o \
./Control/DoutController.o \
./Control/Inverter3Ph.o 

C_DEPS += \
./Control/DinController.d \
./Control/DioController.d \
./Control/DoutController.d \
./Control/Inverter3Ph.d 


# Each subdirectory must supply rules for building sources it contributes
Control/DinController.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DinController.c Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Common -I../../CM7 -I../../ADCs -I../../../../../Control -I../../Drivers/ModifiedDrivers -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Control/DioController.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DioController.c Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Common -I../../CM7 -I../../ADCs -I../../../../../Control -I../../Drivers/ModifiedDrivers -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Control/DoutController.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/DoutController.c Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Common -I../../CM7 -I../../ADCs -I../../../../../Control -I../../Drivers/ModifiedDrivers -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Control/Inverter3Ph.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Control/Inverter3Ph.c Control/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Common -I../../CM7 -I../../ADCs -I../../../../../Control -I../../Drivers/ModifiedDrivers -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

