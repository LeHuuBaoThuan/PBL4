################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../0_app/0_strPrc.c \
../0_app/task_GPIO.c \
../0_app/task_can.c \
../0_app/task_enc.c \
../0_app/task_motorCtr.c \
../0_app/task_pid.c \
../0_app/task_reLog.c 

OBJS += \
./0_app/0_strPrc.o \
./0_app/task_GPIO.o \
./0_app/task_can.o \
./0_app/task_enc.o \
./0_app/task_motorCtr.o \
./0_app/task_pid.o \
./0_app/task_reLog.o 

C_DEPS += \
./0_app/0_strPrc.d \
./0_app/task_GPIO.d \
./0_app/task_can.d \
./0_app/task_enc.d \
./0_app/task_motorCtr.d \
./0_app/task_pid.d \
./0_app/task_reLog.d 


# Each subdirectory must supply rules for building sources it contributes
0_app/%.o 0_app/%.su 0_app/%.cyclo: ../0_app/%.c 0_app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Core/Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"E:/PROJECT/PBL4/0_firmware/projstm32f303cbt6/0_app/0_inc" -I"E:/PROJECT/PBL4/0_firmware/projstm32f303cbt6/1_driver_SmartPID_V3/0_include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-0_app

clean-0_app:
	-$(RM) ./0_app/0_strPrc.cyclo ./0_app/0_strPrc.d ./0_app/0_strPrc.o ./0_app/0_strPrc.su ./0_app/task_GPIO.cyclo ./0_app/task_GPIO.d ./0_app/task_GPIO.o ./0_app/task_GPIO.su ./0_app/task_can.cyclo ./0_app/task_can.d ./0_app/task_can.o ./0_app/task_can.su ./0_app/task_enc.cyclo ./0_app/task_enc.d ./0_app/task_enc.o ./0_app/task_enc.su ./0_app/task_motorCtr.cyclo ./0_app/task_motorCtr.d ./0_app/task_motorCtr.o ./0_app/task_motorCtr.su ./0_app/task_pid.cyclo ./0_app/task_pid.d ./0_app/task_pid.o ./0_app/task_pid.su ./0_app/task_reLog.cyclo ./0_app/task_reLog.d ./0_app/task_reLog.o ./0_app/task_reLog.su

.PHONY: clean-0_app

