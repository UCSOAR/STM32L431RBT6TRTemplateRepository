# UART commands
1. "start" to start system logging
2. "stop" to stop logging *and* dump flash entries onto terminal via UART
3. "clear" to clear flash log

## Logging interval
- Currently set to log once on every startup, then on receving 'start' command at interval specified by ```OSCILLATOR_TASK_PERIOD``` in OscillatorLogger.cpp (currently 10Hz)
