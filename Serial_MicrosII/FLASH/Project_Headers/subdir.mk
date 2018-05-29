################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Headers/serial.c" \

C_SRCS += \
../Project_Headers/serial.c \

OBJS += \
./Project_Headers/serial_c.obj \

OBJS_QUOTED += \
"./Project_Headers/serial_c.obj" \

C_DEPS += \
./Project_Headers/serial_c.d \

C_DEPS_QUOTED += \
"./Project_Headers/serial_c.d" \

OBJS_OS_FORMAT += \
./Project_Headers/serial_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Project_Headers/serial_c.obj: ../Project_Headers/serial.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Project_Headers/serial.args" -ObjN="Project_Headers/serial_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Project_Headers/%.d: ../Project_Headers/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '


