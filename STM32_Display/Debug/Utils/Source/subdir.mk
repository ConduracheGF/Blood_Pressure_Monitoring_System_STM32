################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utils/Source/cardFS.c \
../Utils/Source/cardSD.c \
../Utils/Source/lcd.c \
../Utils/Source/touchscreen.c 

OBJS += \
./Utils/Source/cardFS.o \
./Utils/Source/cardSD.o \
./Utils/Source/lcd.o \
./Utils/Source/touchscreen.o 

C_DEPS += \
./Utils/Source/cardFS.d \
./Utils/Source/cardSD.d \
./Utils/Source/lcd.d \
./Utils/Source/touchscreen.d 


# Each subdirectory must supply rules for building sources it contributes
Utils/Source/%.o Utils/Source/%.su Utils/Source/%.cyclo: ../Utils/Source/%.c Utils/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Utils-2f-Source

clean-Utils-2f-Source:
	-$(RM) ./Utils/Source/cardFS.cyclo ./Utils/Source/cardFS.d ./Utils/Source/cardFS.o ./Utils/Source/cardFS.su ./Utils/Source/cardSD.cyclo ./Utils/Source/cardSD.d ./Utils/Source/cardSD.o ./Utils/Source/cardSD.su ./Utils/Source/lcd.cyclo ./Utils/Source/lcd.d ./Utils/Source/lcd.o ./Utils/Source/lcd.su ./Utils/Source/touchscreen.cyclo ./Utils/Source/touchscreen.d ./Utils/Source/touchscreen.o ./Utils/Source/touchscreen.su

.PHONY: clean-Utils-2f-Source

