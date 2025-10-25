/**
 ********************************************************************************
 * @file    OscillatorTask.hpp
 * @date    Sep 17, 2025
 * @brief
 ********************************************************************************
 */

#ifndef SRC_OSCILLATORTASK_HPP_
#define SRC_OSCILLATORTASK_HPP_

/************************************
 * INCLUDES
 ************************************/
#include "Task.hpp"
#include "SystemDefines.hpp"
#include "main_system.hpp"

/************************************
 * MACROS AND DEFINES
 ************************************/
enum OSCILLATOR_TASK_COMMANDS {
  OSCILLATOR_TASK_COMMAND_NONE = 0,
  EVENT_OSCILLATOR_RX_COMPLETE
};

constexpr uint16_t OSCILLATOR_RX_BUFFER_SZ_BYTES = 16;
/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * CLASS DEFINITIONS
 ************************************/
class OscillatorTask : public Task, public UARTReceiverBase {
 public:
  static OscillatorTask& Inst() {
    static OscillatorTask inst;
    return inst;
  }

  void InitTask();

  // Interrupt receive callback
  void InterruptRxData(uint8_t errors);

 protected:
  static void RunTask(void* pvParams) {
    OscillatorTask::Inst().Run(pvParams);
  }  // Static Task Interface, passes control to the instance Run();

  void Run(void* pvParams);  // Main run code

  void ConfigureUART();
  void HandleUARTMessage(const char* msg);
  // void HandleCommand(Command& cm);

  bool ReceiveData();

  // Helper functions
  static int32_t ExtractIntParameter(const char* msg, uint16_t identifierLen);

  // Member variables
  uint8_t oscillatorBuffer[OSCILLATOR_RX_BUFFER_SZ_BYTES + 1];
  uint8_t oscillatorMsgIdx;
  uint16_t sampleInterval;
  bool isOscillatorMsgReady;
  bool loggingStatus;

  uint8_t oscillatorRxChar;  // Character received from UART Interrupt

  UARTDriver* const usart_;  // UART Driver

 private:
  OscillatorTask();                             // Private constructor
  OscillatorTask(const OscillatorTask&);             // Prevent copy-construction
  OscillatorTask& operator=(const OscillatorTask&);  // Prevent assignment
};
/************************************
 * FUNCTION DECLARATIONS
 ************************************/

#endif /* SRC_OSCILLATORTASK_HPP_ */
