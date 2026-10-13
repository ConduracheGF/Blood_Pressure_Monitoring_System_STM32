################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Gfx/Source/font.c \
../Gfx/Source/gfx.c 

OBJS += \
./Gfx/Source/font.o \
./Gfx/Source/gfx.o 

C_DEPS += \
./Gfx/Source/font.d \
./Gfx/Source/gfx.d 


# Each subdirectory must supply rules for building sources it contributes
Gfx/Source/%.o Gfx/Source/%.su Gfx/Source/%.cyclo: ../Gfx/Source/%.c Gfx/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Gfx-2f-Source

clean-Gfx-2f-Source:
	-$(RM) ./Gfx/Source/font.cyclo ./Gfx/Source/font.d ./Gfx/Source/font.o ./Gfx/Source/font.su ./Gfx/Source/gfx.cyclo ./Gfx/Source/gfx.d ./Gfx/Source/gfx.o ./Gfx/Source/gfx.su

.PHONY: clean-Gfx-2f-Source

