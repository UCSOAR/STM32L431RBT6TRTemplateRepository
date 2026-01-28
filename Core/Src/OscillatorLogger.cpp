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
extern IWDG_HandleTypeDef hiwdg; 
constexpr uint8_t OSCILLATOR_TASK_PERIOD = 100;
#define FLASH_LOG_PTR_ADDR 0x0800F000

/************************************
 * FUNCTION DECLARATIONS
 ************************************/

/************************************
 * FUNCTION DEFINITIONS
 ************************************/
void OscillatorLogger::ResetSession() {
    OscillatorTask::Inst().LoggingStatus() = false;

    // Reset session pointer
    flashAddr = LOG_START_ADDR;

    HAL_FLASH_Unlock();

    // compute number of flash pages to erase
    uint32_t startPage = (LOG_START_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    uint32_t totalPages = (OscillatorTask::Inst().FlashEnd() - LOG_START_ADDR) / FLASH_PAGE_SIZE + 1;

    // erase flash
    for (uint32_t i = 0; i < totalPages; i++) {
        FLASH_EraseInitTypeDef eraseInit{};
        uint32_t pageError = 0;

        eraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
        eraseInit.Banks     = FLASH_BANK_1;
        eraseInit.Page      = startPage + i;
        eraseInit.NbPages   = 1;

        if (HAL_FLASHEx_Erase(&eraseInit, &pageError) != HAL_OK) {
            SOAR_PRINT("Error erasing page %lu\r\n", startPage + i);
            break;
        }

        // refresh watchdog to prevent timeout
        HAL_IWDG_Refresh(&hiwdg); 
    }

    HAL_FLASH_Lock();

    SaveFlashPtr();
    SOAR_PRINT("Flash log cleared successfully.\r\n");
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
            case 1: resetStr = "IWDG"; break;
            case 2: resetStr = "POWER"; break;
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




OscillatorLogger::OscillatorLogger()
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS)
{
    uint32_t savedAddr = *(uint32_t*)FLASH_LOG_PTR_ADDR;
    uint32_t flashEnd  = OscillatorTask::Inst().FlashEnd();

    if (savedAddr >= LOG_START_ADDR && savedAddr < flashEnd)
        flashAddr = savedAddr;
    else
        flashAddr = LOG_START_ADDR;
}

void OscillatorLogger::InitTask()
{
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot init OscillatorLogger twice");

    BaseType_t rtValue = xTaskCreate(
        (TaskFunction_t)OscillatorLogger::RunTask, "OscillatorLogger",
        TASK_OSCILLATOR_STACK_DEPTH_WORDS, this,
        TASK_OSCILLATOR_PRIORITY, &rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "OscillatorLogger::InitTask - xTaskCreate failed");

    lis3dh_init();
}

void OscillatorLogger::Run(void* pvParams)
{
    const uint32_t flashEnd = OscillatorTask::Inst().FlashEnd();

    while (1)
    {
        auto& logging = OscillatorTask::Inst().LoggingStatus();

        if (logging)
        {
            OTBLogEntry entry{};
            entry.tick = HAL_GetTick();

            if (!lis3dh_read_xyz(&entry.ax, &entry.ay, &entry.az))
                entry.ax = entry.ay = entry.az = 0;

            if (flashAddr + sizeof(OTBLogEntry) <= flashEnd)
            {
                HAL_FLASH_Unlock();

                if (((flashAddr - 0x08000000) % FLASH_PAGE_SIZE) == 0)
                {
                    FLASH_EraseInitTypeDef erase{};
                    uint32_t pageError;
                    erase.TypeErase = FLASH_TYPEERASE_PAGES;
                    erase.Banks     = FLASH_BANK_1;
                    erase.Page      = (flashAddr - 0x08000000) / FLASH_PAGE_SIZE;
                    erase.NbPages   = 1;

                    HAL_FLASHEx_Erase(&erase, &pageError);
                }

                uint64_t first64, second64 = 0;
                memcpy(&first64, &entry, 8);
                memcpy(&second64, ((uint8_t*)&entry) + 8, 4);

                HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, first64);
                flashAddr += 8;

                HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddr, second64);
                flashAddr += 8;

                HAL_FLASH_Lock();
                SaveFlashPtr();
                SOAR_PRINT("Logged entry at 0x%08lX\r\n", flashAddr - 16);
            }
            else
            {
                SOAR_PRINT("Flash full, stopping log\n");
                logging = false;
            }
        }
        HAL_IWDG_Refresh(&hiwdg); 
        vTaskDelay(OSCILLATOR_TASK_PERIOD);
    }
}

void OscillatorLogger::SaveFlashPtr()
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase{};
    uint32_t pageError;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Banks     = FLASH_BANK_1;
    erase.Page      = (FLASH_LOG_PTR_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    erase.NbPages   = 1;

    HAL_FLASHEx_Erase(&erase, &pageError);

    uint64_t packed = flashAddr;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_LOG_PTR_ADDR, packed);

    HAL_FLASH_Lock();
}
