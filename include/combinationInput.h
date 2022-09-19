#ifndef COMBINATIONINPUT_H
#define COMBINATIONINPUT_H


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <uiohook.h>

/** this define is for key state when you press caplock */
#define FUNCTION_IDLE 0
#define FUNCTION_PRESSED 1
#define CAPLOCK_PRESSED 2
#define CAPLOCK_RELEASED 3


static const uint16_t keyCode_to_functionCode[][2] = {
    {VC_1, VC_F1}, //f1
    {VC_2, VC_F2}, //f2
    {VC_3, VC_F3}, //f3
    {VC_4, VC_F4}, //f4 
    {VC_5, VC_F5}, //f5
    {VC_6, VC_F6}, //f6
    {VC_7, VC_F7}, //f7
    {VC_8, VC_F8}, //f8
    {VC_9, VC_F9}, //f9
    {VC_0, VC_F10}, //f10
    {VC_MINUS, VC_F11}, //f11
    {VC_EQUALS, VC_F12}, //f12

    //this is for desktop keyboard
    {VC_LEFT, VC_HOME}, //home
    {VC_RIGHT, VC_END}, //end
    {VC_UP, VC_PAGE_UP}, //pgup
    {VC_DOWN, VC_PAGE_DOWN}, //pgdn
    
    //this is for laptop keyboard
    {VC_KP_LEFT, VC_HOME}, //home
    {VC_KP_RIGHT, VC_END}, //end
    {VC_KP_UP, VC_PAGE_UP}, //pgup
    {VC_KP_DOWN, VC_PAGE_DOWN}, //pgdn
};

static long long int pressTime = -1;
static long long int releaseTime;
static short pressCount;

short is_caplock(uiohook_event * event);

void check_double_pressed();

unsigned _stdcall caplock_timeout(void* arg);

void sendKey(uint16_t keyCode);

#endif