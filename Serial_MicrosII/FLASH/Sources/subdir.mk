################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/main.c" \
"../Sources/serial.c" \

C_SRCS += \
../Sources/main.c \
../Sources/serial.c \

OBJS += \
./Sources/main_c.obj \
./Sources/serial_c.obj \

OBJS_QUOTED += \
"./Sources/main_c.obj" \
"./Sources/serial_c.obj" \

C_DEPS += \
./Sources/main_c.d \
./Sources/serial_c.d \

C_DEPS_QUOTED += \
"./Sources/main_c.d" \
"./Sources/serial_c.d" \

OBJS_OS_FORMAT += \
./Sources/main_c.obj \
./Sources/serial_c.obj \


# Each subdirectory must supply rules for building sources it contributes
Sources/main_c.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/main.args" -ObjN="Sources/main_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/serial_c.obj: ../Sources/serial.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: HCS08 Compiler'
	"$(HC08ToolsEnv)/chc08" -ArgFile"Sources/serial.args" -ObjN="Sources/serial_c.obj" "$<" -Lm="$(@:%.obj=%.d)" -LmCfg=xilmou
	@echo 'Finished building: $<'
	@echo ' '


