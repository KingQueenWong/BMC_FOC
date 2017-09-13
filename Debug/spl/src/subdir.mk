################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spl/src/misc.c \
../spl/src/stm32f10x_adc.c \
../spl/src/stm32f10x_bkp.c \
../spl/src/stm32f10x_can.c \
../spl/src/stm32f10x_cec.c \
../spl/src/stm32f10x_crc.c \
../spl/src/stm32f10x_dac.c \
../spl/src/stm32f10x_dbgmcu.c \
../spl/src/stm32f10x_dma.c \
../spl/src/stm32f10x_exti.c \
../spl/src/stm32f10x_flash.c \
../spl/src/stm32f10x_fsmc.c \
../spl/src/stm32f10x_gpio.c \
../spl/src/stm32f10x_i2c.c \
../spl/src/stm32f10x_iwdg.c \
../spl/src/stm32f10x_pwr.c \
../spl/src/stm32f10x_rcc.c \
../spl/src/stm32f10x_rtc.c \
../spl/src/stm32f10x_sdio.c \
../spl/src/stm32f10x_spi.c \
../spl/src/stm32f10x_tim.c \
../spl/src/stm32f10x_usart.c \
../spl/src/stm32f10x_wwdg.c 

OBJS += \
./spl/src/misc.o \
./spl/src/stm32f10x_adc.o \
./spl/src/stm32f10x_bkp.o \
./spl/src/stm32f10x_can.o \
./spl/src/stm32f10x_cec.o \
./spl/src/stm32f10x_crc.o \
./spl/src/stm32f10x_dac.o \
./spl/src/stm32f10x_dbgmcu.o \
./spl/src/stm32f10x_dma.o \
./spl/src/stm32f10x_exti.o \
./spl/src/stm32f10x_flash.o \
./spl/src/stm32f10x_fsmc.o \
./spl/src/stm32f10x_gpio.o \
./spl/src/stm32f10x_i2c.o \
./spl/src/stm32f10x_iwdg.o \
./spl/src/stm32f10x_pwr.o \
./spl/src/stm32f10x_rcc.o \
./spl/src/stm32f10x_rtc.o \
./spl/src/stm32f10x_sdio.o \
./spl/src/stm32f10x_spi.o \
./spl/src/stm32f10x_tim.o \
./spl/src/stm32f10x_usart.o \
./spl/src/stm32f10x_wwdg.o 

C_DEPS += \
./spl/src/misc.d \
./spl/src/stm32f10x_adc.d \
./spl/src/stm32f10x_bkp.d \
./spl/src/stm32f10x_can.d \
./spl/src/stm32f10x_cec.d \
./spl/src/stm32f10x_crc.d \
./spl/src/stm32f10x_dac.d \
./spl/src/stm32f10x_dbgmcu.d \
./spl/src/stm32f10x_dma.d \
./spl/src/stm32f10x_exti.d \
./spl/src/stm32f10x_flash.d \
./spl/src/stm32f10x_fsmc.d \
./spl/src/stm32f10x_gpio.d \
./spl/src/stm32f10x_i2c.d \
./spl/src/stm32f10x_iwdg.d \
./spl/src/stm32f10x_pwr.d \
./spl/src/stm32f10x_rcc.d \
./spl/src/stm32f10x_rtc.d \
./spl/src/stm32f10x_sdio.d \
./spl/src/stm32f10x_spi.d \
./spl/src/stm32f10x_tim.d \
./spl/src/stm32f10x_usart.d \
./spl/src/stm32f10x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
spl/src/%.o: ../spl/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -D_USART_H_ -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -I"D:\Eclipse Project\BMC_FOC_1.0" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\CMSIS" -I"D:\Eclipse Project\BMC_FOC_1.0\spl\inc" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


