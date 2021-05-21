################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/MAX30102.c \
../Src/MAXCalc.c \
../Src/app_ble.c \
../Src/app_debug.c \
../Src/app_entry.c \
../Src/custom_app.c \
../Src/custom_stm.c \
../Src/fonts.c \
../Src/hw_ipcc.c \
../Src/hw_timerserver.c \
../Src/main.c \
../Src/p2p_server_app.c \
../Src/st7735.c \
../Src/stm32_lpm_if.c \
../Src/stm32wbxx_hal_msp.c \
../Src/stm32wbxx_it.c \
../Src/syscalls.c \
../Src/system_stm32wbxx.c 

OBJS += \
./Src/MAX30102.o \
./Src/MAXCalc.o \
./Src/app_ble.o \
./Src/app_debug.o \
./Src/app_entry.o \
./Src/custom_app.o \
./Src/custom_stm.o \
./Src/fonts.o \
./Src/hw_ipcc.o \
./Src/hw_timerserver.o \
./Src/main.o \
./Src/p2p_server_app.o \
./Src/st7735.o \
./Src/stm32_lpm_if.o \
./Src/stm32wbxx_hal_msp.o \
./Src/stm32wbxx_it.o \
./Src/syscalls.o \
./Src/system_stm32wbxx.o 

C_DEPS += \
./Src/MAX30102.d \
./Src/MAXCalc.d \
./Src/app_ble.d \
./Src/app_debug.d \
./Src/app_entry.d \
./Src/custom_app.d \
./Src/custom_stm.d \
./Src/fonts.d \
./Src/hw_ipcc.d \
./Src/hw_timerserver.d \
./Src/main.d \
./Src/p2p_server_app.d \
./Src/st7735.d \
./Src/stm32_lpm_if.d \
./Src/stm32wbxx_hal_msp.d \
./Src/stm32wbxx_it.d \
./Src/syscalls.d \
./Src/system_stm32wbxx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32WB55xx -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/lpm/tiny_lpm" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/utilities" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/auto" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/template" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Src" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Device/ST/STM32WBxx/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/sequencer"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


