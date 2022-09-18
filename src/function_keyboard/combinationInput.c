#include <stdio.h>
#include <combinationInput.h>
#include <uiohook.h>

#include <windows.h>
#include <process.h>

void testingOutput(){
    printf("testing output");
}

short is_caplock(uiohook_event * event){
    if(event->type == EVENT_KEY_PRESSED){
        //check key was released
        if(pressTime == -1){
            pressTime = event->time;
        }
        // double press for function key(caplock)
        if(pressCount >= 1){
            return CAPLOCK_PRESSED;
        }
        return FUNCTION_PRESSED;
    }
    else if(event->type == EVENT_KEY_RELEASED){
        releaseTime = event->time;
        if(releaseTime - pressTime < 200){ //this code make long press not as start of double press
            if(!pressCount){
                check_double_pressed();
            }
            pressCount++;
        }
        pressTime = -1;
    }
    return FUNCTION_IDLE;
}

/**
 * @brief timeout of caplock double pressed
 * 
 * @param arg there is no argument for this
 * @return unsigned 
 */
unsigned _stdcall caplock_timeout(void* arg)
{
    Sleep(200);
    printf("time passed");
    pressCount = 0;
}

/**
 * @brief new thread for counting 200ms
 * 
 */
void check_double_pressed(){
    _beginthreadex(NULL, 0, caplock_timeout, 0, 0, NULL);
}