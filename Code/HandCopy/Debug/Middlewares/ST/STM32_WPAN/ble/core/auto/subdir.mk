################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.c \
../Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.c 

OBJS += \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.o \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.o 

C_DEPS += \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gap_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_gatt_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hal_aci.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_hci_le.d \
./Middlewares/ST/STM32_WPAN/ble/core/auto/ble_l2cap_aci.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_WPAN/ble/core/auto/%.o: ../Middlewares/ST/STM32_WPAN/ble/core/auto/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSE_HAL_DRIVER -DSTM32WB55xx -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/STM32WBxx_HAL_Driver/Inc/Legacy" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/lpm/tiny_lpm" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/tl" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/interface/patterns/ble_thread/shci" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/utilities" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/auto" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/core/template" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Inc" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble/svc/Src" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Device/ST/STM32WBxx/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Middlewares/ST/STM32_WPAN/ble" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Drivers/CMSIS/Include" -I"E:/Glenn/OneDrive/Bureaublad/Programming/STM32/HandCopy/Utilities/sequencer"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


