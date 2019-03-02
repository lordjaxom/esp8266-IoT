#include "Logger.hpp"

#if defined( LOGGER_SERIAL )
LoggerSerial Logger;
#else
LoggerNoop Logger;
#endif