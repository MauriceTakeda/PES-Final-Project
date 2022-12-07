################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/PES_Final_Project.c \
../source/accelerometer.c \
../source/cbfifo.c \
../source/cmd_processor.c \
../source/i2c.c \
../source/mtb.c \
../source/rgb_led.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/timers.c \
../source/uart.c 

C_DEPS += \
./source/PES_Final_Project.d \
./source/accelerometer.d \
./source/cbfifo.d \
./source/cmd_processor.d \
./source/i2c.d \
./source/mtb.d \
./source/rgb_led.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/timers.d \
./source/uart.d 

OBJS += \
./source/PES_Final_Project.o \
./source/accelerometer.o \
./source/cbfifo.o \
./source/cmd_processor.o \
./source/i2c.o \
./source/mtb.o \
./source/rgb_led.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/timers.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DPRINTF_FLOAT_ENABLE=1 -DSDK_DEBUGCONSOLE_UART -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_DEBUGCONSOLE=1 -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\board" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\source" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\drivers" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\CMSIS" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\utilities" -I"C:\Users\mauri\Documents\MCUXpressoIDE_11.6.0_8187\workspace\PES_Final_Project\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/PES_Final_Project.d ./source/PES_Final_Project.o ./source/accelerometer.d ./source/accelerometer.o ./source/cbfifo.d ./source/cbfifo.o ./source/cmd_processor.d ./source/cmd_processor.o ./source/i2c.d ./source/i2c.o ./source/mtb.d ./source/mtb.o ./source/rgb_led.d ./source/rgb_led.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sysclock.d ./source/sysclock.o ./source/timers.d ./source/timers.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

