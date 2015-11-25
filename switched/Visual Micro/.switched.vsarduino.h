//Board = Arduino Uno
#define ARDUINO 103
#define __AVR_ATmega328P__
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {}

//already defined in arduno.h
//already defined in arduno.h
void chaseRGB();
void chaseRainbowsOld();
void chaseFullbrite();
void chaseStrobe();
void chaseUserScroll();
void chaseBen();
void chaseRandom();
void chaseMSGEQ7_1();
void chaseMSGEQ7_2();
void chaseMSGEQ7_3();
void chaseMatrix();
void chaseRainbow();
void chaseBatterylevel();
void chaseFireIce1 ();
void chaseFireIce2 ();
void chaseStandby ();
void chaseTemp ();
int dip ();
void steparray();
void phaseDelay();
int doStrobe();
void setHue(int hue, int localBrightness);
void serialDebug();
void setTemp(int temperature, int localBrightness);
long readTemp();

#include "C:\Program Files (x86)\arduino-1.0.3\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\arduino-1.0.3\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\tom\SkyDrive\github\LEDstrip\switched\switched.ino"
