################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l431rbtx.s 

S_DEPS += \
./Core/Startup/startup_stm32l431rbtx.d 

OBJS += \
./Core/Startup/startup_stm32l431rbtx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Components" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Src" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32l431rbtx.d ./Core/Startup/startup_stm32l431rbtx.o

.PHONY: clean-Core-2f-Startup

