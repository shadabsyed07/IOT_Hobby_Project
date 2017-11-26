################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BMP180.cpp \
../src/BMPsensor.cpp \
../src/I2CDevice.cpp 

OBJS += \
./src/BMP180.o \
./src/BMPsensor.o \
./src/I2CDevice.o 

CPP_DEPS += \
./src/BMP180.d \
./src/BMPsensor.d \
./src/I2CDevice.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-gnueabihf-g++ -I/usr/include/arm-linux-gnueabihf/c++/6.3.0 -I/usr/include/arm-linux-gnueabihf/c++/6 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


