#ifndef __TAKEI_SENSORS__
#define __TAKEI_SENSORS__


#include "Common.h"


// Define cap sensors. Comment those that don't work or are not connected.
//#define CAP1
#define CAP2
//#define CAP3
//#define CAP4
#define CAP5
#define CAP6
//#define CAP7
//#define CAP8

// Cap sensor thresholds
// #define CAP1_MIN 5
// #define CAP1_MAX 15

#define CAP2_MIN 40
#define CAP2_MAX 55 

#define CAP3_MIN 15
#define CAP3_MAX 45
// 
#define CAP4_MIN 4
#define CAP4_MAX 7
// 
#define CAP5_MIN 20
#define CAP5_MAX 50
// 
#define CAP6_MIN 20
#define CAP6_MAX 40
// 
//#define CAP7_MIN 12
//#define CAP7_MAX 18
// 
//#define CAP8_MIN 4
//#define CAP8_MAX 16


// Cap event related functions
void setupCapsListener();
void registerTouchEvent(void (*func) (byte val), byte num);
void touchDispatcher();


// Analog event related functions
void registerAnalogEvent(void (*func) (uint16_t val), byte pinNumber, uint16_t low, uint16_t high);
void analogDispatcher();


#endif // __TAKEI_SENSORS__