#ifndef __TAKEI_COMMON__
#define __TAKEI_COMMON__

#include "Arduino.h"

// #define TAKEI_DEBUG_ON

#ifdef TAKEI_DEBUG_ON 
    #define initLog(speed) Serial.begin(speed)
    #define log(buf) Serial.print(buf)
    #define logLine(buf) Serial.println(buf)
#else
    #define initLog(speed)
    #define log(buf)
    #define logLine(buf)
#endif

#endif // __TAKEI_COMMON__
