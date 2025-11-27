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
#include "OscillatorLogger.hpp"
#include "Command.hpp"
#include "CubeUtils.hpp"
#include "lis3dh_init.h"
#include <cstring>

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/

/************************************
 * VARIABLES
 ************************************/
constexpr uint8_t OSCILLATOR_TASK_PERIOD = 100;
extern I2C_HandleTypeDef hi2c2;
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

  sampleInterval = 1000; // log interval in ms
  isOscillatorMsgReady = false;
  loggingStatus = false;

  // flash address start and end
  flashAddress = 0x08010000;
  flashEnd = 0x0803FFFF;
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

// print contents of flash to UART
void OscillatorTask::ReadFlashLog() {
    uint32_t addr = 0x08010000;
    while (addr < flashAddress) {
        OTBLogEntry entry;
        memcpy(&entry, (void*)addr, sizeof(OTBLogEntry));

        SOAR_PRINT("Tick: %llu, ax: %.2f, ay: %.2f, az: %.2f\r\n",
                   entry.tick, entry.ax, entry.ay, entry.az);

        addr += sizeof(OTBLogEntry);
    }
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
    qEvtQueue->Receive(cm, 0);

    // Process the command
    if (cm.GetCommand() == DATA_COMMAND &&
        cm.GetTaskCommand() == EVENT_OSCILLATOR_RX_COMPLETE) {
      HandleUARTMessage((const char*)oscillatorBuffer);
    }

    cm.Reset();
    osDelay(sampleInterval);
  }
}

/**
 * @brief Handles oscillator messages, assumes msg is null terminated
 * @param msg Message to read, must be null termianted
 */
void OscillatorTask::HandleUARTMessage(const char* msg) {
  SOAR_PRINT("UART MSG: %s\n", msg);
  if (strcmp(msg, "start") == 0) {
    SOAR_PRINT("Starting system logging\n");
    loggingStatus = true;
    //OscillatorLogger::Inst().ResetSession();
  }
  else if (strcmp(msg, "stop") == 0){
    SOAR_PRINT("Stopping system logging\n");
    loggingStatus = false;
    OscillatorLogger::Inst().DumpFlash();
  } else if (strcmp(msg, "clear") == 0){
    SOAR_PRINT("Clearing flash entries\n");
    OscillatorLogger::Inst().ResetSession();
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

