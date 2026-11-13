################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Source/app.c 

OBJS += \
./App/Source/app.o 

C_DEPS += \
./App/Source/app.d 


# Each subdirectory must supply rules for building sources it contributes
App/Source/%.o App/Source/%.su App/Source/%.cyclo: ../App/Source/%.c App/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/App" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/FATFS/Target" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display" -I"C:/Users/Condurache Gabriel/Desktop/LICENTA/STM32_Display/Middlewares/Third_Party/FatFs/src" -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Source

clean-App-2f-Source:
	-$(RM) ./App/Source/app.cyclo ./App/Source/app.d ./App/Source/app.o ./App/Source/app.su

.PHONY: clean-App-2f-Source

