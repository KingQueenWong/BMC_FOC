################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

S_UPPER_SRCS += \
../startup_stm32f10x_md.S 

OBJS += \
./main.o \
./startup_stm32f10x_md.o 

S_UPPER_DEPS += \
./startup_stm32f10x_md.d 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D_USART_H_ -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"D:\Eclipse Project\BMC_FOC_1.0" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\CMSIS" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -D_USART_H_ -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"D:\Eclipse Project\BMC_FOC_1.0" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\CMSIS" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\inc" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


