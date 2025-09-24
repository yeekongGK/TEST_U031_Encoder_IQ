################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/main.c \
C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/stm32u0xx_hal_msp.c \
C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/stm32u0xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c \
../Application/User/tracsens.c \
../Application/User/tracsens_io.c 

OBJS += \
./Application/User/main.o \
./Application/User/stm32u0xx_hal_msp.o \
./Application/User/stm32u0xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o \
./Application/User/tracsens.o \
./Application/User/tracsens_io.o 

C_DEPS += \
./Application/User/main.d \
./Application/User/stm32u0xx_hal_msp.d \
./Application/User/stm32u0xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d \
./Application/User/tracsens.d \
./Application/User/tracsens_io.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/main.o: C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../../Inc -I../../../../../../../Drivers/BSP/STM32U0xx_Nucleo -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Device/ST/STM32U0xx/Include -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Include -I"C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/STM32CubeIDE/Application/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/stm32u0xx_hal_msp.o: C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/stm32u0xx_hal_msp.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../../Inc -I../../../../../../../Drivers/BSP/STM32U0xx_Nucleo -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Device/ST/STM32U0xx/Include -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Include -I"C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/STM32CubeIDE/Application/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/stm32u0xx_it.o: C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/Src/stm32u0xx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../../Inc -I../../../../../../../Drivers/BSP/STM32U0xx_Nucleo -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Device/ST/STM32U0xx/Include -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Include -I"C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/STM32CubeIDE/Application/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Application/User/%.o Application/User/%.su Application/User/%.cyclo: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../../Inc -I../../../../../../../Drivers/BSP/STM32U0xx_Nucleo -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Device/ST/STM32U0xx/Include -IC:/Users/yee-kong.chong/STM32Cube/Repository/STM32Cube_FW_U0_V1.3.0/Drivers/CMSIS/Include -I"C:/Users/yee-kong.chong/Documents/Development/STM32_ExampleCode/STM32Cube_FW_U0_V1.3.0/Projects/NUCLEO-U031R8/Examples/PWR/PWR_SHUTDOWN_Copy/STM32CubeIDE/Application/User" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/main.cyclo ./Application/User/main.d ./Application/User/main.o ./Application/User/main.su ./Application/User/stm32u0xx_hal_msp.cyclo ./Application/User/stm32u0xx_hal_msp.d ./Application/User/stm32u0xx_hal_msp.o ./Application/User/stm32u0xx_hal_msp.su ./Application/User/stm32u0xx_it.cyclo ./Application/User/stm32u0xx_it.d ./Application/User/stm32u0xx_it.o ./Application/User/stm32u0xx_it.su ./Application/User/syscalls.cyclo ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/syscalls.su ./Application/User/sysmem.cyclo ./Application/User/sysmem.d ./Application/User/sysmem.o ./Application/User/sysmem.su ./Application/User/tracsens.cyclo ./Application/User/tracsens.d ./Application/User/tracsens.o ./Application/User/tracsens.su ./Application/User/tracsens_io.cyclo ./Application/User/tracsens_io.d ./Application/User/tracsens_io.o ./Application/User/tracsens_io.su

.PHONY: clean-Application-2f-User

