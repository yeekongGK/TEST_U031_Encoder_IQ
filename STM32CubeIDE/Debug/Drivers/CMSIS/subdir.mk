################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/system_stm32u0xx.c 

OBJS += \
./Drivers/CMSIS/system_stm32u0xx.o 

C_DEPS += \
./Drivers/CMSIS/system_stm32u0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/system_stm32u0xx.o: C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/system_stm32u0xx.c Drivers/CMSIS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../../Inc -I../../../../../../../Drivers/BSP/STM32U0xx_Nucleo -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Device/ST/STM32U0xx/Include -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Include -I"C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/STM32CubeIDE/Application/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-CMSIS

clean-Drivers-2f-CMSIS:
	-$(RM) ./Drivers/CMSIS/system_stm32u0xx.cyclo ./Drivers/CMSIS/system_stm32u0xx.d ./Drivers/CMSIS/system_stm32u0xx.o ./Drivers/CMSIS/system_stm32u0xx.su

.PHONY: clean-Drivers-2f-CMSIS

