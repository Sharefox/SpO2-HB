################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/sequencer/stm32_seq.c 

OBJS += \
./Utilities/sequencer/stm32_seq.o 

C_DEPS += \
./Utilities/sequencer/stm32_seq.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/sequencer/%.o: ../Utilities/sequencer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32WB55xx -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/lpm/tiny_lpm" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/utilities" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/auto" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/template" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Src" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Device/ST/STM32WBxx/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/sequencer"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


