################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../append_func.cpp \
../disasm_util.cpp \
../elf_util.cpp \
../main.cpp 

OBJS += \
./append_func.o \
./disasm_util.o \
./elf_util.o \
./main.o 

CPP_DEPS += \
./append_func.d \
./disasm_util.d \
./elf_util.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


