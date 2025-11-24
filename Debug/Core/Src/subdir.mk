################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/OscillatorLogger.cpp \
../Core/Src/OscillatorTask.cpp \
../Core/Src/lis3dh_utils.cpp 

C_SRCS += \
../Core/Src/freertos.c \
../Core/Src/lis3dh_init.c \
../Core/Src/main.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_hal_timebase_tim.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c 

C_DEPS += \
./Core/Src/freertos.d \
./Core/Src/lis3dh_init.d \
./Core/Src/main.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_hal_timebase_tim.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d 

OBJS += \
./Core/Src/OscillatorLogger.o \
./Core/Src/OscillatorTask.o \
./Core/Src/freertos.o \
./Core/Src/lis3dh_init.o \
./Core/Src/lis3dh_utils.o \
./Core/Src/main.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_hal_timebase_tim.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o 

CPP_DEPS += \
./Core/Src/OscillatorLogger.d \
./Core/Src/OscillatorTask.d \
./Core/Src/lis3dh_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++20 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Components" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Src" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Components" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Src" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/OscillatorLogger.cyclo ./Core/Src/OscillatorLogger.d ./Core/Src/OscillatorLogger.o ./Core/Src/OscillatorLogger.su ./Core/Src/OscillatorTask.cyclo ./Core/Src/OscillatorTask.d ./Core/Src/OscillatorTask.o ./Core/Src/OscillatorTask.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/lis3dh_init.cyclo ./Core/Src/lis3dh_init.d ./Core/Src/lis3dh_init.o ./Core/Src/lis3dh_init.su ./Core/Src/lis3dh_utils.cyclo ./Core/Src/lis3dh_utils.d ./Core/Src/lis3dh_utils.o ./Core/Src/lis3dh_utils.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_hal_timebase_tim.cyclo ./Core/Src/stm32l4xx_hal_timebase_tim.d ./Core/Src/stm32l4xx_hal_timebase_tim.o ./Core/Src/stm32l4xx_hal_timebase_tim.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su

.PHONY: clean-Core-2f-Src

