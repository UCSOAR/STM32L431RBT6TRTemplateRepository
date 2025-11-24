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
    int16_t ax;
    int16_t ay;
    int16_t az;
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

    void InitTask();
    void DumpFlash();
    uint32_t flashAddr = 0x08010000;

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
