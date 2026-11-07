################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../IO/Source/cardSD_io.c \
../IO/Source/lcd_io.c \
../IO/Source/sys_io.c \
../IO/Source/touchscreen_io.c 

OBJS += \
./IO/Source/cardSD_io.o \
./IO/Source/lcd_io.o \
./IO/Source/sys_io.o \
./IO/Source/touchscreen_io.o 

C_DEPS += \
./IO/Source/cardSD_io.d \
./IO/Source/lcd_io.d \
./IO/Source/sys_io.d \
./IO/Source/touchscreen_io.d 


# Each subdirectory must supply rules for building sources it contributes
IO/Source/%.o IO/Source/%.su IO/Source/%.cyclo: ../IO/Source/%.c IO/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-IO-2f-Source

clean-IO-2f-Source:
	-$(RM) ./IO/Source/cardSD_io.cyclo ./IO/Source/cardSD_io.d ./IO/Source/cardSD_io.o ./IO/Source/cardSD_io.su ./IO/Source/lcd_io.cyclo ./IO/Source/lcd_io.d ./IO/Source/lcd_io.o ./IO/Source/lcd_io.su ./IO/Source/sys_io.cyclo ./IO/Source/sys_io.d ./IO/Source/sys_io.o ./IO/Source/sys_io.su ./IO/Source/touchscreen_io.cyclo ./IO/Source/touchscreen_io.d ./IO/Source/touchscreen_io.o ./IO/Source/touchscreen_io.su

.PHONY: clean-IO-2f-Source

