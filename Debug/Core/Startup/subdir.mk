################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
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
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Core" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Drivers/Inc" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Drivers" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarOS/Core/Inc" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Components" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core/Src" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core/Inc" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/Core/Startup" -I"C:/Users/Owner/Desktop/SOAR/STM32L431RBT6TRTemplateRepository/SoarDebug/Inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32l431rbtx.d ./Core/Startup/startup_stm32l431rbtx.o

.PHONY: clean-Core-2f-Startup

