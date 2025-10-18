################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SoarOS/Core/Command.cpp \
../SoarOS/Core/CubeUtils.cpp \
../SoarOS/Core/Mutex.cpp \
../SoarOS/Core/Queue.cpp \
../SoarOS/Core/Task.cpp \
../SoarOS/Core/Timer.cpp 

OBJS += \
./SoarOS/Core/Command.o \
./SoarOS/Core/CubeUtils.o \
./SoarOS/Core/Mutex.o \
./SoarOS/Core/Queue.o \
./SoarOS/Core/Task.o \
./SoarOS/Core/Timer.o 

CPP_DEPS += \
./SoarOS/Core/Command.d \
./SoarOS/Core/CubeUtils.d \
./SoarOS/Core/Mutex.d \
./SoarOS/Core/Queue.d \
./SoarOS/Core/Task.d \
./SoarOS/Core/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
SoarOS/Core/%.o SoarOS/Core/%.su SoarOS/Core/%.cyclo: ../SoarOS/Core/%.cpp SoarOS/Core/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/Components" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/Core" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/Core/Src" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"/Users/gillianhabermehl/Documents/SOAR/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SoarOS-2f-Core

clean-SoarOS-2f-Core:
	-$(RM) ./SoarOS/Core/Command.cyclo ./SoarOS/Core/Command.d ./SoarOS/Core/Command.o ./SoarOS/Core/Command.su ./SoarOS/Core/CubeUtils.cyclo ./SoarOS/Core/CubeUtils.d ./SoarOS/Core/CubeUtils.o ./SoarOS/Core/CubeUtils.su ./SoarOS/Core/Mutex.cyclo ./SoarOS/Core/Mutex.d ./SoarOS/Core/Mutex.o ./SoarOS/Core/Mutex.su ./SoarOS/Core/Queue.cyclo ./SoarOS/Core/Queue.d ./SoarOS/Core/Queue.o ./SoarOS/Core/Queue.su ./SoarOS/Core/Task.cyclo ./SoarOS/Core/Task.d ./SoarOS/Core/Task.o ./SoarOS/Core/Task.su ./SoarOS/Core/Timer.cyclo ./SoarOS/Core/Timer.d ./SoarOS/Core/Timer.o ./SoarOS/Core/Timer.su

.PHONY: clean-SoarOS-2f-Core

