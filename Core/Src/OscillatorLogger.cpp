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
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS) {
        uint32_t savedAddr = *(uint32_t*)FLASH_LOG_PTR_ADDR;
        uint32_t flashEnd  = OscillatorTask::Inst().FlashEnd();

        if (savedAddr >= LOG_START_ADDR && savedAddr < flashEnd) {
            flashAddr = savedAddr;
        } else {
            flashAddr = LOG_START_ADDR;
        }
    }

void OscillatorLogger::InitTask() {
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot init OscillatorLogger twice");

    BaseType_t rtValue = xTaskCreate(
        (TaskFunction_t)OscillatorLogger::RunTask, (const char*)"OscillatorLogger",
        (uint16_t)TASK_OSCILLATOR_STACK_DEPTH_WORDS, (void*)this,
        (UBaseType_t)TASK_OSCILLATOR_PRIORITY, (TaskHandle_t*)&rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "OscillatorLogger::InitTask - xTaskCreate() failed");

    lis3dh_init();

}

void OscillatorLogger::ResetSession() {
    // Reset session pointer
    flashAddr = LOG_START_ADDR;

    // Erase all log pages
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInit{};
    uint32_t pageError = 0;
    uint32_t nbPages = (OscillatorTask::Inst().FlashEnd() - LOG_START_ADDR) / FLASH_PAGE_SIZE + 1;
    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks     = FLASH_BANK_1;
    eraseInit.Page      = (LOG_START_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    eraseInit.NbPages   = nbPages;
    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
        SOAR_PRINT("Error erasing flash pages\r\n");
    }
    HAL_FLASH_Lock();

    // Save pointer so a fresh session starts from LOG_START_ADDR
    SaveFlashPtr();
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

            if (flashAddr + sizeof(OTBLogEntry) <= flashEnd) {
                HAL_FLASH_Unlock();

                // Erase page if on boundary
                if (((flashAddr - 0x08000000) % FLASH_PAGE_SIZE) == 0) {
                    FLASH_EraseInitTypeDef eraseInit{};
                    uint32_t pageError = 0;

                    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
                    eraseInit.Banks     = FLASH_BANK_1;
                    eraseInit.Page      = (flashAddr - 0x08000000) / FLASH_PAGE_SIZE;
                    eraseInit.NbPages   = 1;

                    if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
                        SOAR_PRINT("Error erasing flash page\r\n");
                        HAL_FLASH_Lock();
                        logging = false;
                        continue;
                    }
                }

                // Write first 8 bytes
                uint64_t first64;
                memcpy(&first64, &entry, 8);
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, first64) != HAL_OK) {
                    SOAR_PRINT("Error writing flash\r\n");
                    HAL_FLASH_Lock();
                    logging = false;
                    continue;
                }
                flashAddr += 8;

                // Write next 4 bytes
                uint64_t second64 = 0;
                memcpy(&second64, ((uint8_t*)&entry) + 8, 4);
                if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, second64) != HAL_OK) {
                    SOAR_PRINT("Error writing flash\r\n");
                    HAL_FLASH_Lock();
                    logging = false;
                    continue;
                }
                flashAddr += 8;

                HAL_FLASH_Lock();

                // Save updated pointer persistently
                SaveFlashPtr();

                SOAR_PRINT("Logged entry at 0x%08lX\r\n", flashAddr - 16);

            } else {
                SOAR_PRINT("Flash full, stopping log\r\n");
                logging = false;
            }
        }

        vTaskDelay(delay);
    }
}


void OscillatorLogger::SaveFlashPtr() {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef eraseInit{};
    uint32_t pageError = 0;

    eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInit.Banks     = FLASH_BANK_1;
    eraseInit.Page      = (FLASH_LOG_PTR_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    eraseInit.NbPages   = 1;

    HAL_FLASHEx_Erase(&eraseInit, &pageError);

    uint64_t packed = flashAddr;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_LOG_PTR_ADDR, packed);

    HAL_FLASH_Lock();
}

void OscillatorLogger::DumpFlash() {
    SOAR_PRINT("Tick,ax,ay,az\r\n");

    uint32_t addr = LOG_START_ADDR;
    uint32_t entryCount = 0;

    while (addr < flashAddr) {
        OTBLogEntry entry;
        uint64_t first64, second64;

        memcpy(&first64, (void*)addr, 8);
        addr += 8;

        memcpy(&second64, (void*)addr, 8);
        addr += 8;

        memcpy(&entry, &first64, 8);
        memcpy(((uint8_t*)&entry) + 8, &second64, 4);

        const char* resetStr = "UNKNOWN";
switch(entry.entryType) {
    case 0: resetStr = "NORMAL"; break;
    case 1: resetStr = "SOFTWARE"; break;
    case 2: resetStr = "IWDG"; break;
    case 3: resetStr = "POR+PIN"; break;
    case 4: resetStr = "WWDG"; break;
}
SOAR_PRINT("%lu,%d,%d,%d,%s\r\n",
           entry.tick, entry.ax, entry.ay, entry.az, resetStr);


        entryCount++;
    }

    SOAR_PRINT("Total entries dumped: %lu\r\n", entryCount);
}

void OscillatorLogger::LogImmediate(const OTBLogEntry& entry) {
    uint32_t flashEnd = OscillatorTask::Inst().FlashEnd();

    if (flashAddr + sizeof(OTBLogEntry) > flashEnd) return;

    HAL_FLASH_Unlock();

    // page erase if needed
    if (((flashAddr - 0x08000000) % FLASH_PAGE_SIZE) == 0) {
        FLASH_EraseInitTypeDef eraseInit{};
        uint32_t pageError;
        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.Banks     = FLASH_BANK_1;
        eraseInit.Page      = (flashAddr - 0x08000000) / FLASH_PAGE_SIZE;
        eraseInit.NbPages   = 1;
        HAL_FLASHEx_Erase(&eraseInit, &pageError);
    }

    // Write as two doublewords like before
    uint64_t first64, second64 = 0;
    memcpy(&first64, &entry, 8);
    memcpy(&second64, ((uint8_t*)&entry)+8, sizeof(OTBLogEntry)-8);

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, first64);
    flashAddr += 8;

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, second64);
    flashAddr += 8;

    HAL_FLASH_Lock();
    SaveFlashPtr();
}
