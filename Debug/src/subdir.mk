################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/ledcontrol.c \
../src/main.c \
../src/platform.c \
../src/smpte2022_control.c \
../src/vplat.c 

OBJS += \
./src/ledcontrol.o \
./src/main.o \
./src/platform.o \
./src/smpte2022_control.o \
./src/vplat.o 

C_DEPS += \
./src/ledcontrol.d \
./src/main.d \
./src/platform.d \
./src/smpte2022_control.d \
./src/vplat.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -DDEBUG -D__SIM -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../bsp_1/microblaze_0/include -mlittle-endian -mcpu=v9.6 -mxl-soft-mul -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


