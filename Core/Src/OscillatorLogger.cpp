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
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS), flashAddr(0x08010000) {} // Start flash logging here

void OscillatorLogger::InitTask() {
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot init OscillatorLogger twice");

    BaseType_t rtValue = xTaskCreate(
        (TaskFunction_t)OscillatorLogger::RunTask, (const char*)"OscillatorLogger",
        (uint16_t)TASK_OSCILLATOR_STACK_DEPTH_WORDS, (void*)this,
        (UBaseType_t)TASK_OSCILLATOR_PRIORITY, (TaskHandle_t*)&rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "OscillatorLogger::InitTask - xTaskCreate() failed");
}

void OscillatorLogger::Run(void* pvParams) {
    const TickType_t delay = pdMS_TO_TICKS(5000);
    const uint32_t flashEnd = OscillatorTask::Inst().FlashEnd();

    while (1) {
        auto& logging = OscillatorTask::Inst().LoggingStatus();

        if (logging) {
            OTBLogEntry entry;
            entry.tick = HAL_GetTick();

            if(!lis3dh_read_xyz(&entry.ax, &entry.ay, &entry.az)) {
                entry.ax = entry.ay = entry.az = 0;
            }

            if (this->flashAddr + sizeof(OTBLogEntry) <= flashEnd) {
                HAL_FLASH_Unlock();

                // erase page if at start
                if (((this->flashAddr - 0x08000000) % FLASH_PAGE_SIZE) == 0) {
                    FLASH_EraseInitTypeDef eraseInit{};
                    uint32_t pageError = 0;

                    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
                    eraseInit.Banks     = FLASH_BANK_1; 
                    eraseInit.Page      = (this->flashAddr - 0x08000000) / FLASH_PAGE_SIZE;
                    eraseInit.NbPages   = 1;

                    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
                        SOAR_PRINT("Error erasing flash page\r\n");
                        HAL_FLASH_Lock();
                        logging = false;
                        continue;
                    }
                }

                // Write first 8 bytes (tick + ax + ay)
                uint64_t first64;
                memcpy(&first64, &entry, 8);
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, this->flashAddr, first64) != HAL_OK) {
                    SOAR_PRINT("Error writing flash\r\n");
                    HAL_FLASH_Lock();
                    logging = false;
                    continue;
                }
                this->flashAddr += 8;

                // Write remaining 4 bytes (az + padding)
                uint64_t second64 = 0;
                memcpy(&second64, ((uint8_t*)&entry)+8, 4); // 4 bytes: az + 2 bytes padding
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, this->flashAddr, second64) != HAL_OK) {
                    SOAR_PRINT("Error writing flash\r\n");
                    HAL_FLASH_Lock();
                    logging = false;
                    continue;
                }
                this->flashAddr += 8;

                HAL_FLASH_Lock();
                
                SOAR_PRINT("Logged entry at 0x%08lX\r\n", this->flashAddr - 16);
            } else {
                SOAR_PRINT("Flash full, stopping log\r\n");
                logging = false;
            }
        }
        vTaskDelay(delay);
    }
}

void OscillatorLogger::DumpFlash() {
    SOAR_PRINT("Dumping flash from 0x%08lX to 0x%08lX\r\n", 0x08010000, this->flashAddr);
    
    uint32_t addr = 0x08010000;
    uint32_t entryCount = 0;
    
    while (addr < this->flashAddr) {
        OTBLogEntry entry;
        uint64_t first64, second64;

        // Read first 8 bytes
        memcpy(&first64, (void*)addr, 8);
        addr += 8;

        // Read next 8 bytes
        memcpy(&second64, (void*)addr, 8);
        addr += 8;

        // Reconstruct entry
        memcpy(&entry, &first64, 8);          // tick + ax + ay
        memcpy(((uint8_t*)&entry)+8, &second64, 4); // az + padding

        SOAR_PRINT("Entry %lu - Tick: %lu, ax: %d, ay: %d, az: %d\r\n",
                   entryCount++, entry.tick, entry.ax, entry.ay, entry.az);
    }
    
    SOAR_PRINT("Total entries dumped: %lu\r\n", entryCount);
}
