################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Drivers/ModifiedDrivers/PWM11_16_Drivers.c \
C:/TarazWorkDir/TempLoc/PEControllerBSP/Drivers/ModifiedDrivers/PWM1_10_Drivers.c 

OBJS += \
./Drivers/ModifiedDrivers/PWM11_16_Drivers.o \
./Drivers/ModifiedDrivers/PWM1_10_Drivers.o 

C_DEPS += \
./Drivers/ModifiedDrivers/PWM11_16_Drivers.d \
./Drivers/ModifiedDrivers/PWM1_10_Drivers.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/ModifiedDrivers/PWM11_16_Drivers.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Drivers/ModifiedDrivers/PWM11_16_Drivers.c Drivers/ModifiedDrivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/ModifiedDrivers/PWM1_10_Drivers.o: C:/TarazWorkDir/TempLoc/PEControllerBSP/Drivers/ModifiedDrivers/PWM1_10_Drivers.c Drivers/ModifiedDrivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

