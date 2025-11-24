/**
 ********************************************************************************
 * @file    OscillatorLogger.cpp
 * @author  Christy
 * @date    Nov 23, 2025
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "OscillatorTask.hpp"
#include "OscillatorLogger.hpp"
#include "CubeUtils.hpp"
#include "lis3dh_init.h"
#include <cstring>

/************************************
 * VARIABLES
 ************************************/

/************************************
 * FUNCTION DECLARATIONS
 ************************************/

/************************************
 * FUNCTION DEFINITIONS
 ************************************/
OscillatorLogger::OscillatorLogger()
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS) {}

void OscillatorLogger::InitTask() {
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot init OscillatorLogger twice");

    BaseType_t rtValue = xTaskCreate(
        (TaskFunction_t)OscillatorLogger::RunTask, (const char*)"OscillatorLogger",
        (uint16_t)TASK_OSCILLATOR_STACK_DEPTH_WORDS, (void*)this,
        (UBaseType_t)TASK_OSCILLATOR_PRIORITY, (TaskHandle_t*)&rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "OscillatorLogger::InitTask - xTaskCreate() failed");
}

void OscillatorLogger::Run(void* pvParams) {
    // log frequency
    const TickType_t delay = pdMS_TO_TICKS(500);

    while (1) {
        auto& flashAddr = OscillatorTask::Inst().FlashAddress();
        auto flashEnd   = OscillatorTask::Inst().FlashEnd();
        auto& logging   = OscillatorTask::Inst().LoggingStatus();

        if (logging) {
            uint16_t tick16 = static_cast<uint16_t>(HAL_GetTick() & 0xFFFF);

            if (flashAddr + 8 <= flashEnd) {
                HAL_FLASH_Unlock();

                // calculate page index
                uint32_t page = (flashAddr - 0x08000000) / FLASH_PAGE_SIZE;

                // erase flash page if start of page
                if (((flashAddr - 0x08000000) % FLASH_PAGE_SIZE) == 0) {
                    FLASH_EraseInitTypeDef eraseInit{};
                    uint32_t pageError = 0;

                    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
                    eraseInit.Banks     = FLASH_BANK_1; 
                    eraseInit.Page      = page;
                    eraseInit.NbPages   = 1;

                    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
                        SOAR_PRINT("Error erasing flash page\r\n");
                        HAL_FLASH_Lock();
                        logging = false;
                        continue;
                    }
                }

                // Pack tick16 into a 64-bit doubleword
                uint64_t data64 = tick16;

                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, data64) != HAL_OK) {
                    SOAR_PRINT("Error writing to flash\r\n");
                    HAL_FLASH_Lock();
                    logging = false;
                } else {
                    flashAddr += 8;  // increment by doubleword size
                }

                HAL_FLASH_Lock();
            } else {
                SOAR_PRINT("Flash full, stopping log\r\n");
                logging = false;
            }
        }

        vTaskDelay(delay);
    }
}

void OscillatorLogger::DumpFlash() {
    uint32_t addr = 0x08010000; // start of flash
    while (addr < OscillatorTask::Inst().FlashAddress()) {
        uint64_t data64;
        memcpy(&data64, (void*)addr, sizeof(uint64_t));

        // extract 16-bit tick value
        uint16_t tick16 = static_cast<uint16_t>(data64 & 0xFFFF);
        SOAR_PRINT("Tick: %u\r\n", tick16);

        addr += 8;
    }
}
