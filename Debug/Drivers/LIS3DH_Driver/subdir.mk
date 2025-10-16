################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LIS3DH_Driver/lis3dh_reg.c 

C_DEPS += \
./Drivers/LIS3DH_Driver/lis3dh_reg.d 

OBJS += \
./Drivers/LIS3DH_Driver/lis3dh_reg.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LIS3DH_Driver/%.o Drivers/LIS3DH_Driver/%.su Drivers/LIS3DH_Driver/%.cyclo: ../Drivers/LIS3DH_Driver/%.c Drivers/LIS3DH_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L431xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Drivers" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Libraries/embedded-template-library/include" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarOS/Core/Inc" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Components" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Src" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/Core/Startup" -I"C:/Users/Chris/STM32CubeIDE/workspace_1.16.1/STM32L431RBT6TR_Template_Repository/SoarDebug/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LIS3DH_Driver

clean-Drivers-2f-LIS3DH_Driver:
	-$(RM) ./Drivers/LIS3DH_Driver/lis3dh_reg.cyclo ./Drivers/LIS3DH_Driver/lis3dh_reg.d ./Drivers/LIS3DH_Driver/lis3dh_reg.o ./Drivers/LIS3DH_Driver/lis3dh_reg.su

.PHONY: clean-Drivers-2f-LIS3DH_Driver

