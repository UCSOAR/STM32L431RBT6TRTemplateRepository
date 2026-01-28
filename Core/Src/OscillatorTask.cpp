/**
 ********************************************************************************
 * @file    OscillatorTask.cpp
 * @date    Sep 17, 2025
 * @brief
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include <lis3dh_hal.h>
#include "OscillatorTask.hpp"
#include "OscillatorLogger.hpp"
#include "Command.hpp"
#include "CubeUtils.hpp"
#include <cstring>

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
#define FLASH_LOG_STATUS_ADDR  0x0800F800

/************************************
 * VARIABLES
 ************************************/

extern I2C_HandleTypeDef hi2c2;
/************************************
 * FUNCTION DECLARATIONS
 ************************************/

/************************************
 * FUNCTION DEFINITIONS
 ************************************/

void OscillatorTask::Run(void* pvParams) {
  // Arm the interrupt
  ReceiveData();

  while (1) {

    Command cm;

    // Wait forever for a command
    qEvtQueue->Receive(cm, 0);

    // Process the command
    if (cm.GetCommand() == DATA_COMMAND &&
        cm.GetTaskCommand() == EVENT_OSCILLATOR_RX_COMPLETE) {
      HandleUARTMessage((const char*)oscillatorBuffer);
    }

    cm.Reset();
  }
}

// /**
//  * @brief Receive data, currently receives by arming interrupt
// */
bool OscillatorTask::ReceiveData() { 
  return usart_->ReceiveIT(&oscillatorRxChar, this); 
}

// /**
//  * @brief Receive data to the buffer
//  * @return Whether the debugBuffer is ready or not
//  */
void OscillatorTask::InterruptRxData(uint8_t errors) {
  // If we already have an unprocessed debug message, ignore this byte
  if (!isOscillatorMsgReady) {
    // Check byte for end of message - note if using termite you must turn on
    // append CR
    if (oscillatorRxChar == '\r' || oscillatorMsgIdx == OSCILLATOR_RX_BUFFER_SZ_BYTES) {
      // Null terminate and process
      oscillatorBuffer[oscillatorMsgIdx++] = '\0';
      isOscillatorMsgReady = true;

      // Notify the debug task
      Command cm(DATA_COMMAND, EVENT_OSCILLATOR_RX_COMPLETE);
      bool res = qEvtQueue->SendFromISR(cm);

      // If we failed to send the event, we should reset the buffer, that way
      // OscillatorTask doesn't stall
      if (res == false) {
        oscillatorMsgIdx = 0;
        isOscillatorMsgReady = false;
      }
    } else {
      oscillatorBuffer[oscillatorMsgIdx++] = oscillatorRxChar;
    }
  }

  // Re-arm the interrupt
  ReceiveData();
}



OscillatorTask::OscillatorTask()
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS), usart_(UART::Debug)
{
    memset(oscillatorBuffer, 0, sizeof(oscillatorBuffer));
    oscillatorMsgIdx = 0;

    isOscillatorMsgReady = false;

    // ---- Load persistent logging flag ----
    uint32_t flag = *(uint32_t*)FLASH_LOG_STATUS_ADDR;
    if (flag == 0xAAAAAAAA)
        loggingStatus = true;
    else
        loggingStatus = false;

    flashAddress = 0x08010000;
    flashEnd = 0x0803FFFF;
}

static void SaveLoggingStatus(bool enabled)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase{};
    uint32_t pageError;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Banks     = FLASH_BANK_1;
    erase.Page      = (FLASH_LOG_STATUS_ADDR - 0x08000000) / FLASH_PAGE_SIZE;
    erase.NbPages   = 1;

    HAL_FLASHEx_Erase(&erase, &pageError);

    uint64_t word = enabled ? 0xAAAAAAAA : 0x00000000;
    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, FLASH_LOG_STATUS_ADDR, word);

    HAL_FLASH_Lock();
}

void OscillatorTask::InitTask()
{
    SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize Oscillator task twice");

    BaseType_t rtValue = xTaskCreate(
        (TaskFunction_t)OscillatorTask::RunTask, "OscillatorTask",
        TASK_OSCILLATOR_STACK_DEPTH_WORDS, this,
        TASK_OSCILLATOR_PRIORITY, &rtTaskHandle);

    SOAR_ASSERT(rtValue == pdPASS, "OscillatorTask::InitTask - xTaskCreate() failed");
}

void OscillatorTask::HandleUARTMessage(const char* msg)
{
    SOAR_PRINT("UART MSG: %s\n", msg);

    if (strcmp(msg, "start") == 0) {
        SOAR_PRINT("Starting system logging\n");
        loggingStatus = true;
        SaveLoggingStatus(true);
    }
    else if (strcmp(msg, "stop") == 0) {
        SOAR_PRINT("Stopping system logging\n");
        loggingStatus = false;
        SaveLoggingStatus(false);
        OscillatorLogger::Inst().DumpFlash();
    }
    else if (strcmp(msg, "clear") == 0) {
        SOAR_PRINT("Clearing flash entries\n");
        OscillatorLogger::Inst().ResetSession();
    }

    oscillatorMsgIdx = 0;
    isOscillatorMsgReady = false;
}
