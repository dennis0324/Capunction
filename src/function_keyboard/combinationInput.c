#include <stdio.h>
#include <stdlib.h>
#include <combinationInput.h>
#include <uiohook.h>


#if TRAY_APPINDICATOR
    #include <unistd.h>
    #include <pthread.h>
#elif TRAY_APPKIT
#elif TRAY_WINAPI
    #include <windows.h>
    #include <process.h>
#endif




void sendKey(uint16_t keyCode){
    uint16_t functionCode; 
    int len = sizeof(keyCode_to_functionCode) / sizeof(keyCode_to_functionCode[0]);

    // finding pairs of key binds
    for(int i = 0; i < len; i++){
        if(keyCode_to_functionCode[i][0] == keyCode)
            functionCode = keyCode_to_functionCode[i][1];
    }

    uiohook_event * event = (uiohook_event *)malloc(sizeof(uiohook_event));
    // default init
    event->time = 0;

    // sending key press
    event->type = EVENT_KEY_PRESSED;
    event->mask = 0;
    event->data.keyboard.keycode = functionCode;

    hook_post_event(event);
    // sending key released
    event->type = EVENT_KEY_RELEASED;
    event->mask = 0;
    event->data.keyboard.keycode = functionCode;
    hook_post_event(event);

    free(event);
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


#ifdef TRAY_WINAPI
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
#endif

#ifdef TRAY_APPINDICATOR

void * caplock_timeout(void * arg){
    pthread_detach(pthread_self());
    usleep(200 * 1000);
    printf("time passed\n");
    pressCount = 0;
}

void check_double_pressed(){
    pthread_t thread;
    pthread_create(&thread, NULL, caplock_timeout, NULL);
}


#endif