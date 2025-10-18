################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Components/RunInterface.cpp \
../Components/main_system.cpp 

OBJS += \
./Components/RunInterface.o \
./Components/main_system.o 

CPP_DEPS += \
./Components/RunInterface.d \
./Components/main_system.d 


# Each subdirectory must supply rules for building sources it contributes
Components/%.o Components/%.su Components/%.cyclo: ../Components/%.cpp Components/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Core" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Drivers" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Components" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core/Src" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core/Startup" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components

clean-Components:
	-$(RM) ./Components/RunInterface.cyclo ./Components/RunInterface.d ./Components/RunInterface.o ./Components/RunInterface.su ./Components/main_system.cyclo ./Components/main_system.d ./Components/main_system.o ./Components/main_system.su

.PHONY: clean-Components

