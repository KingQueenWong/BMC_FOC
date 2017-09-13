################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spl/CMSIS/core_cm3.c \
../spl/CMSIS/system_stm32f10x.c 

OBJS += \
./spl/CMSIS/core_cm3.o \
./spl/CMSIS/system_stm32f10x.o 

C_DEPS += \
./spl/CMSIS/core_cm3.d \
./spl/CMSIS/system_stm32f10x.d 


# Each subdirectory must supply rules for building sources it contributes
spl/CMSIS/%.o: ../spl/CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D_USART_H_ -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"D:\Eclipse Project\BMC_FOC_1.0" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\CMSIS" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


