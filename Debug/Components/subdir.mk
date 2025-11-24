################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
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
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Components" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Src" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Components

clean-Components:
	-$(RM) ./Components/RunInterface.cyclo ./Components/RunInterface.d ./Components/RunInterface.o ./Components/RunInterface.su ./Components/main_system.cyclo ./Components/main_system.d ./Components/main_system.o ./Components/main_system.su

.PHONY: clean-Components

