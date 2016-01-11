################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MThread.cpp \
../src/TCPMessengerServer.cpp \
../src/TCPMsnDispatcher.cpp \
../src/TCPMsnServer.cpp \
../src/TCPSocket.cpp \
../src/main.cpp 

OBJS += \
./src/MThread.o \
./src/TCPMessengerServer.o \
./src/TCPMsnDispatcher.o \
./src/TCPMsnServer.o \
./src/TCPSocket.o \
./src/main.o 

CPP_DEPS += \
./src/MThread.d \
./src/TCPMessengerServer.d \
./src/TCPMsnDispatcher.d \
./src/TCPMsnServer.d \
./src/TCPSocket.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


