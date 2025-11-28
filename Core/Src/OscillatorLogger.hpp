/**
 ********************************************************************************
 * @file    OscillatorLogger.hpp
 * @author  Christy
 * @date    Nov 23, 2025
 * @brief
 ********************************************************************************
 */

#ifndef SRC_OSCILLATORLOGGER_HPP_
#define SRC_OSCILLATORLOGGER_HPP_

/************************************
 * INCLUDES
 ************************************/
#include "OscillatorTask.hpp"
#include "Task.hpp"
/************************************
 * MACROS AND DEFINES
 ************************************/
struct OTBLogEntry {
    uint32_t tick;
    int16_t  ax;
    int16_t  ay;
    int16_t  az;
    uint8_t  entryType;
    uint8_t  reserved;
};
/************************************
 * TYPEDEFS
 ************************************/

/************************************
 * CLASS DEFINITIONS
 ************************************/
class OscillatorLogger : public Task {
public:
    static OscillatorLogger& Inst() {
        static OscillatorLogger instance;
        return instance;
    }
    void SaveFlashPtr();

    void InitTask();
    void DumpFlash();
    void ResetSession();
    void LogImmediate(const OTBLogEntry& entry);

    uint32_t flashAddr = 0x08010000;
    static constexpr uint32_t FLASH_LOG_PTR_ADDR = 0x0800FFF0; 
    static constexpr uint32_t LOG_START_ADDR = 0x08010000;

protected:
    OscillatorLogger();
    void Run(void* pvParams);

    static void RunTask(void* pvParams) {
        ((OscillatorLogger*)pvParams)->Run(pvParams);
    }
};

/************************************
 * FUNCTION DECLARATIONS
 ************************************/

#endif /* SRC_OSCILLATORLOGGER_HPP_ */
