#ifndef __TAKEI_SENSORS__
#define __TAKEI_SENSORS__


#include "Common.h"


// Define cap sensors. Comment those that don't work or are not connected.
#define CAP1
#define CAP2
#define CAP3
#define CAP4
#define CAP5
#define CAP6
#define CAP7
#define CAP8

// Cap sensor thresholds
#define CAP1_MIN 10
#define CAP1_MAX 33

#define CAP2_MIN 30
#define CAP2_MAX 70

#define CAP3_MIN 10
#define CAP3_MAX 60
// // 
// #define CAP4_MIN 4
// #define CAP4_MAX 7
// // 
// #define CAP5_MIN 5
// #define CAP5_MAX 15
// // 
// #define CAP6_MIN 5
// #define CAP6_MAX 15
// // 
// #define CAP7_MIN 14
// #define CAP7_MAX 16
// // 
// #define CAP8_MIN 4
// #define CAP8_MAX 14


// Cap event related functions
void setupCapsListener();
void registerTouchEvent(void (*func) (byte val), byte num);
void touchDispatcher();


// Analog event related functions
void registerAnalogEvent(void (*func) (uint16_t val), byte pinNumber, uint16_t low, uint16_t high);
void analogDispatcher();


#endif // __TAKEI_SENSORS__