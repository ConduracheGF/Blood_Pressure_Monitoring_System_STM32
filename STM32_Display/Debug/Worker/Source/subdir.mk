################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Worker/Source/acquisition.c \
../Worker/Source/display.c \
../Worker/Source/vitals.c 

OBJS += \
./Worker/Source/acquisition.o \
./Worker/Source/display.o \
./Worker/Source/vitals.o 

C_DEPS += \
./Worker/Source/acquisition.d \
./Worker/Source/display.d \
./Worker/Source/vitals.d 


# Each subdirectory must supply rules for building sources it contributes
Worker/Source/%.o Worker/Source/%.su Worker/Source/%.cyclo: ../Worker/Source/%.c Worker/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Worker-2f-Source

clean-Worker-2f-Source:
	-$(RM) ./Worker/Source/acquisition.cyclo ./Worker/Source/acquisition.d ./Worker/Source/acquisition.o ./Worker/Source/acquisition.su ./Worker/Source/display.cyclo ./Worker/Source/display.d ./Worker/Source/display.o ./Worker/Source/display.su ./Worker/Source/vitals.cyclo ./Worker/Source/vitals.d ./Worker/Source/vitals.o ./Worker/Source/vitals.su

.PHONY: clean-Worker-2f-Source

