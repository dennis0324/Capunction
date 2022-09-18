#include <stdio.h>
#include <combinationInput.h>
#include <uiohook.h>

// this
#include <windows.h>
#include <process.h>

void testingOutput(){
    printf("testing output");
}

short is_caplock(uiohook_event * event){
    if(event->type == EVENT_KEY_PRESSED){
        is_pressed = true;
        printf("%d\n",pressCount);
        if(pressCount >= 1){
            printf("double clicked\n");
            return CAPLOCK_PRESSED;
        }

    }
    else if(event->type == EVENT_KEY_RELEASED){
        if(!pressCount){
            check_double_pressed();
        }
        is_pressed = false;
        pressCount++;
    }
    return FUNCTION_IDLE;
}

/**
 * @brief 
 * 
 * @param arg 
 * @return unsigned 
 */
unsigned _stdcall caplock_timeout(void* arg)
{
    Sleep(300);
    printf("time passed");
    pressCount = 0;
    is_pressed = false;
}


void check_double_pressed(){
    _beginthreadex(NULL, 0, caplock_timeout, 0, 0, NULL);
}