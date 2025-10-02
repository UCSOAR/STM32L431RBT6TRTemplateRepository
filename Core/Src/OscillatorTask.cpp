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
#include "OscillatorTask.hpp"
#include "Command.hpp"
#include "CubeUtils.hpp"
#include <cstring>

#include "stm32l4xx_hal.h"
/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * VARIABLES
 ************************************/
constexpr uint8_t OSCILLATOR_TASK_PERIOD = 100;
extern CRC_HandleTypeDef hi2c2;
/************************************
 * FUNCTION DECLARATIONS
 ************************************/

/************************************
 * FUNCTION DEFINITIONS
 ************************************/

/**
 * @brief Constructor, sets all member variables
 */
OscillatorTask::OscillatorTask()
    : Task(TASK_OSCILLATOR_QUEUE_DEPTH_OBJS), usart_(UART::Debug) {
  memset(oscillatorBuffer, 0, sizeof(oscillatorBuffer));
  oscillatorMsgIdx = 0;
  isOscillatorMsgReady = false;
}

/**
 * @brief Init task for RTOS
 */
void OscillatorTask::InitTask() {
  // Make sure the task is not already initialized
  SOAR_ASSERT(rtTaskHandle == nullptr, "Cannot initialize Oscillator task twice");

  // Start the task
  BaseType_t rtValue = xTaskCreate(
      (TaskFunction_t)OscillatorTask::RunTask, (const char*)"OscillatorTask",
      (uint16_t)TASK_OSCILLATOR_STACK_DEPTH_WORDS, (void*)this,
      (UBaseType_t)TASK_OSCILLATOR_PRIORITY, (TaskHandle_t*)&rtTaskHandle);

  // Ensure creation succeeded
  SOAR_ASSERT(rtValue == pdPASS, "OscillatorTask::InitTask - xTaskCreate() failed");
}

// TODO: Only run thread when appropriate GPIO pin pulled HIGH (or by define)
/**
 *    @brief Runcode for the DebugTask
 */
void OscillatorTask::Run(void* pvParams) {
  // Arm the interrupt
  ReceiveData();

  while (1) {
    Command cm;

    // Wait forever for a command
    qEvtQueue->ReceiveWait(cm);

    // Process the command
    if (cm.GetCommand() == DATA_COMMAND &&
        cm.GetTaskCommand() == EVENT_OSCILLATOR_RX_COMPLETE) {
      HandleOscillatorMessage((const char*)oscillatorBuffer);
    }

    cm.Reset();
  }
}

/**
 * @brief Handles oscillator messages, assumes msg is null terminated
 * @param msg Message to read, must be null termianted
 */
void OscillatorTask::HandleOscillatorMessage(const char* msg) {
  //-- SYSTEM / CHAR COMMANDS -- (Must be last)
  if (strcmp(msg, "sysreset") == 0) {
    // Reset the system
    SOAR_ASSERT(false, "System reset requested");
  } else if (strcmp(msg, "sysinfo") == 0) {
    // Print message
    SOAR_PRINT("\n\n-- CUBE SYSTEM --\n");
    SOAR_PRINT("Current System Free Heap: %d Bytes\n", xPortGetFreeHeapSize());
    SOAR_PRINT("Lowest Ever Free Heap: %d Bytes\n",
               xPortGetMinimumEverFreeHeapSize());
    SOAR_PRINT("Debug Task Runtime  \t: %d ms\n\n",
               TICKS_TO_MS(xTaskGetTickCount()));
  } else {
    // Single character command, or unknown command
    switch (msg[0]) {
      default:
        SOAR_PRINT("Debug, unknown command: %s\n", msg);
        break;
    }
  }

  // We've read the data, clear the buffer
  oscillatorMsgIdx = 0;
  isOscillatorMsgReady = false;
}

/**
 * @brief Receive data, currently receives by arming interrupt
 */
bool OscillatorTask::ReceiveData() { return usart_->ReceiveIT(&oscillatorRxChar, this); }

/**
 * @brief Receive data to the buffer
 * @return Whether the debugBuffer is ready or not
 */
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

/**
 * @brief Extracts an integer parameter from a string
 * @brief msg Message to extract from, MUST be at least identifierLen long, and
 * properly null terminated
 * @brief identifierLen Length of the identifier eg. 'rsc ' (Including the
 * space) is 4
 * @return ERRVAL on failure, otherwise the extracted value
 */
int32_t OscillatorTask::ExtractIntParameter(const char* msg,
                                       uint16_t identifierLen) {
  // Handle a command with an int parameter at the end
  if (static_cast<uint16_t>(strlen(msg)) < identifierLen + 1) {
    SOAR_PRINT("Int parameter command insufficient length\r\n");
    return ERRVAL;
  }

  // Extract the value and attempt conversion to integer
  const int32_t val = Utils::StringToLong(&msg[identifierLen]);
  if (val == ERRVAL) {
    SOAR_PRINT("Int parameter command invalid value\r\n");
  }

  return val;
}

