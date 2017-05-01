/*
****************************************************************************************************
*       INCLUDE FILES
****************************************************************************************************
*/

#ifdef ARDUINO_ARCH_ESP8266 
extern "C" {
#include "user_interface.h"
}

#else

#ifdef ARDUINO_ARCH_AVR 
#include <TimerOne.h>
#endif

#ifdef ARDUINO_ARCH_SAM
#include <DueTimer.h>
#endif

#endif

#include <Arduino.h>
#include "timer.h"


/*
****************************************************************************************************
*       INTERNAL MACROS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL CONSTANTS
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL DATA TYPES
****************************************************************************************************
*/


/*
****************************************************************************************************
*       INTERNAL GLOBAL VARIABLES
****************************************************************************************************
*/

#ifdef ARDUINO_ARCH_ESP8266 
os_timer_t espTimer;
#endif

void (*g_callback)(void);


/*
****************************************************************************************************
*       INTERNAL FUNCTIONS
****************************************************************************************************
*/

#ifdef ARDUINO_ARCH_ESP8266 
void espTimer_callback(void *pArg)
{
    os_timer_disarm(&espTimer);
    g_callback();
}
#else
static void timer1_callback(void)
{
    Timer1.stop();
    g_callback();
}
#endif

/*
****************************************************************************************************
*       GLOBAL FUNCTIONS
****************************************************************************************************
*/

void timer_init(void (*callback)(void))
{
#ifdef ARDUINO_ARCH_ESP8266 
    os_timer_setfn(&espTimer, espTimer_callback, NULL);
#else
#ifdef ARDUINO_ARCH_AVR
    Timer1.initialize();
#endif

    Timer1.attachInterrupt(timer1_callback);
    Timer1.stop();
#endif
    g_callback = callback;
}

void timer_set(uint32_t time_ms)
{
#ifdef ARDUINO_ARCH_ESP8266
    os_timer_arm(&espTimer, time_ms, false);
#else
    Timer1.setPeriod(time_ms*1000);
    Timer1.start();
#endif
}

void delay_us(uint32_t time_us)
{
    delayMicroseconds(time_us);
}
