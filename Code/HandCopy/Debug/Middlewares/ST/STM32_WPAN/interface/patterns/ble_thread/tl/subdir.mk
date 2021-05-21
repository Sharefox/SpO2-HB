################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl.c \
../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl_if.c \
../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl.c \
../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl_if.c \
../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/tl_mbox.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl.o \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl_if.o \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl.o \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl_if.o \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/tl_mbox.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl.d \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/hci_tl_if.d \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl.d \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/shci_tl_if.d \
./Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/tl_mbox.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/%.o: ../Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32WB55xx -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/lpm/tiny_lpm" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/utilities" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/auto" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/template" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Src" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Device/ST/STM32WBxx/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/sequencer"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


