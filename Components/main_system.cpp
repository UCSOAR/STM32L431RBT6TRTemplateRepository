/**
 ******************************************************************************
 * File Name          : main_system.cpp
 * Description        : This file acts as an interface supporting CubeIDE Codegen
    while having a clean interface for development.
 ******************************************************************************
*/
/* Includes -----------------------------------------------------------------*/
#include "SystemDefines.hpp"
#include "UARTDriver.hpp"


// Tasks
#include "CubeTask.hpp"
#include "DebugTask.hpp"
#include "OscillatorTask.hpp"
#include "OscillatorLogger.hpp"

/* Drivers ------------------------------------------------------------------*/
namespace Driver {
    UARTDriver uart1(USART1);
}

/* Interface Functions ------------------------------------------------------------*/
/**
 * @brief Main function interface, called inside main.cpp before os initialization takes place.
*/


void log_reset_reason() {
    uint32_t csr = RCC->CSR;

    OTBLogEntry entry{};
    entry.tick = HAL_GetTick();
    entry.ax = entry.ay = entry.az = 0;

    // Map CSR flags to a reason code
    if (csr & RCC_CSR_SFTRSTF) {
        entry.entryType = 1; // SOFTWARE
    } else if (csr & RCC_CSR_IWDGRSTF) {
        entry.entryType = 2; // IWDG
    } else if ((csr & RCC_CSR_BORRSTF) && (csr & RCC_CSR_PINRSTF)) {
        entry.entryType = 3; // POR + PIN
    } else if (csr & RCC_CSR_WWDGRSTF) {
        entry.entryType = 4; // WWDG
    } else {
        entry.entryType = 0; // Unknown / other
    }

    OscillatorLogger::Inst().LogImmediate(entry);

    RCC->CSR |= RCC_CSR_RMVF; // Clear reset flags
}


void run_main() {
    // Init Tasks
    CubeTask::Inst().InitTask();
    OscillatorTask::Inst().InitTask();
    OscillatorLogger::Inst().InitTask();
    log_reset_reason(); 
//    OscillatorTask::Inst().LoggingStatus() = true;
    // Print System Boot Info : Warning, don't queue more than 10 prints before scheduler starts
    SOAR_PRINT("\n-- CUBE SYSTEM --\n");
    SOAR_PRINT("System Reset Reason: [TODO]\n"); //TODO: System reset reason can be implemented via. Flash storage
    SOAR_PRINT("Current System Free Heap: %d Bytes\n", xPortGetFreeHeapSize());
    SOAR_PRINT("Lowest Ever Free Heap: %d Bytes\n\n", xPortGetMinimumEverFreeHeapSize());

    // Start the Scheduler
    // Guidelines:
    // - Be CAREFUL with race conditions after osKernelStart
    // - All uses of new and delete should be closely monitored after this point
    osKernelStart();

    // Should never reach here
    SOAR_ASSERT(false, "osKernelStart() failed");

    while (1)
    {
        osDelay(100);
        HAL_NVIC_SystemReset();
    }
}
