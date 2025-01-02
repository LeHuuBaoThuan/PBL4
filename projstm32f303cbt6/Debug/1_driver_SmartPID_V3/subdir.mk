################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../1_driver_SmartPID_V3/lib_encoder.c \
../1_driver_SmartPID_V3/motor_control.c \
../1_driver_SmartPID_V3/moving_average_int_16.c \
../1_driver_SmartPID_V3/pid.c 

OBJS += \
./1_driver_SmartPID_V3/lib_encoder.o \
./1_driver_SmartPID_V3/motor_control.o \
./1_driver_SmartPID_V3/moving_average_int_16.o \
./1_driver_SmartPID_V3/pid.o 

C_DEPS += \
./1_driver_SmartPID_V3/lib_encoder.d \
./1_driver_SmartPID_V3/motor_control.d \
./1_driver_SmartPID_V3/moving_average_int_16.d \
./1_driver_SmartPID_V3/pid.d 


# Each subdirectory must supply rules for building sources it contributes
1_driver_SmartPID_V3/%.o 1_driver_SmartPID_V3/%.su 1_driver_SmartPID_V3/%.cyclo: ../1_driver_SmartPID_V3/%.c 1_driver_SmartPID_V3/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"E:/PROJECT/PBL4/0_firmware/projstm32f303cbt6/0_app/0_inc" -I"E:/PROJECT/PBL4/0_firmware/projstm32f303cbt6/1_driver_SmartPID_V3/0_include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-1_driver_SmartPID_V3

clean-1_driver_SmartPID_V3:
	-$(RM) ./1_driver_SmartPID_V3/lib_encoder.cyclo ./1_driver_SmartPID_V3/lib_encoder.d ./1_driver_SmartPID_V3/lib_encoder.o ./1_driver_SmartPID_V3/lib_encoder.su ./1_driver_SmartPID_V3/motor_control.cyclo ./1_driver_SmartPID_V3/motor_control.d ./1_driver_SmartPID_V3/motor_control.o ./1_driver_SmartPID_V3/motor_control.su ./1_driver_SmartPID_V3/moving_average_int_16.cyclo ./1_driver_SmartPID_V3/moving_average_int_16.d ./1_driver_SmartPID_V3/moving_average_int_16.o ./1_driver_SmartPID_V3/moving_average_int_16.su ./1_driver_SmartPID_V3/pid.cyclo ./1_driver_SmartPID_V3/pid.d ./1_driver_SmartPID_V3/pid.o ./1_driver_SmartPID_V3/pid.su

.PHONY: clean-1_driver_SmartPID_V3

