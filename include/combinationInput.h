#ifndef COMBINATIONINPUT_H
#define COMBINATIONINPUT_H


#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <uiohook.h>

#define FUNCTION_IDLE 0
#define FUNCTION_PRESSED 1
#define CAPLOCK_PRESSED 2
#define CAPLOCK_RELEASED 3

static long long int pressTime;
static bool is_pressed;
static short pressCount;

void testingOutput();

short is_caplock(uiohook_event * event);

void check_double_pressed();

unsigned _stdcall caplock_timeout(void* arg);

#endif