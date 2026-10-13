################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../UI/Source/screens.c \
../UI/Source/widgets.c 

OBJS += \
./UI/Source/screens.o \
./UI/Source/widgets.o 

C_DEPS += \
./UI/Source/screens.d \
./UI/Source/widgets.d 


# Each subdirectory must supply rules for building sources it contributes
UI/Source/%.o UI/Source/%.su UI/Source/%.cyclo: ../UI/Source/%.c UI/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UI-2f-Source

clean-UI-2f-Source:
	-$(RM) ./UI/Source/screens.cyclo ./UI/Source/screens.d ./UI/Source/screens.o ./UI/Source/screens.su ./UI/Source/widgets.cyclo ./UI/Source/widgets.d ./UI/Source/widgets.o ./UI/Source/widgets.su

.PHONY: clean-UI-2f-Source

