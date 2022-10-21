
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <uiohook.h>
#include <wchar.h>


#include <stdlib.h>
#include <combinationInput.h>

#include <tray.h>
#include <hook.h>

static short is_doubled = FUNCTION_IDLE;

bool logger_proc(unsigned int level, const char *format, ...) {
    bool status = false;
    
    va_list args;
    switch (level) {
        case LOG_LEVEL_INFO:
            va_start(args, format);
            status = vfprintf(stdout, format, args) >= 0;
            va_end(args);
            break;

        case LOG_LEVEL_WARN:
        case LOG_LEVEL_ERROR:
            va_start(args, format);
            status = vfprintf(stderr, format, args) >= 0;
            va_end(args);
            break;
    }
    
    return status;
}

void dispatch_proc(uiohook_event * const event) {
    char buffer[256] = { 0 };
    
    switch (event->type) {
        case EVENT_KEY_PRESSED:
            if(event->data.keyboard.keycode == VC_CAPS_LOCK){
                is_doubled = is_caplock(event);
                if(is_doubled != CAPLOCK_PRESSED){
                    event->reserved = 0x1;
                }
            }
            else { //all of key except caplock and escape 
                switch (event->data.keyboard.keycode)
                {
                case VC_1: //f1
                case VC_2: //f2
                case VC_3: //f3 
                case VC_4: //f4
                case VC_5: //f5
                case VC_6: //f6
                case VC_7: //f7
                case VC_8: //f8
                case VC_9: //f9
                case VC_0: //f10
                case VC_MINUS: //f11
                case VC_EQUALS: //f12
                case VC_BACK_SLASH: // insert
                case VC_BACKSPACE: //delete

                case VC_LEFT: //home
                case VC_KP_LEFT: //this is for laptop arrows

                case VC_RIGHT: //end
                case VC_KP_RIGHT: //this is for laptop arrows

                case VC_UP: //pgup
                case VC_KP_UP: //this is for laptop arrows

                case VC_DOWN: //pgdown
                case VC_KP_DOWN: //this is for laptop arrows
                    
                    // check function key(caplock) is pressed
                    if(is_doubled == FUNCTION_PRESSED){
                        printf("function key output\n");
                        sendKey(event->data.keyboard.keycode);
                        event->reserved = 0x1;
                    }
                    break;
                
                default:
                    break;
                }
            }

        case EVENT_KEY_RELEASED:
            if(event->data.keyboard.keycode == VC_CAPS_LOCK){
                if(is_doubled != CAPLOCK_PRESSED ){
                    event->reserved = 0x1;
                }
                is_doubled = is_caplock(event);
            }
            break;

        default:
            break;
    }
}


void hook_program_run(){
    int status = hook_run();
    switch (status) {
        case UIOHOOK_SUCCESS:
            // Everything is ok.
            break;

        // System level errors.
        case UIOHOOK_ERROR_OUT_OF_MEMORY:
            logger_proc(LOG_LEVEL_ERROR, "Failed to allocate memory. (%#X)", status);
            break;


        // X11 specific errors.
        case UIOHOOK_ERROR_X_OPEN_DISPLAY:
            logger_proc(LOG_LEVEL_ERROR, "Failed to open X11 display. (%#X)", status);
            break;

        case UIOHOOK_ERROR_X_RECORD_NOT_FOUND:
            logger_proc(LOG_LEVEL_ERROR, "Unable to locate XRecord extension. (%#X)", status);
            break;

        case UIOHOOK_ERROR_X_RECORD_ALLOC_RANGE:
            logger_proc(LOG_LEVEL_ERROR, "Unable to allocate XRecord range. (%#X)", status);
            break;

        case UIOHOOK_ERROR_X_RECORD_CREATE_CONTEXT:
            logger_proc(LOG_LEVEL_ERROR, "Unable to allocate XRecord context. (%#X)", status);
            break;

        case UIOHOOK_ERROR_X_RECORD_ENABLE_CONTEXT:
            logger_proc(LOG_LEVEL_ERROR, "Failed to enable XRecord context. (%#X)", status);
            break;

            
        // Windows specific errors.
        case UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX:
            logger_proc(LOG_LEVEL_ERROR, "Failed to register low level windows hook. (%#X)", status);
            break;


        // Darwin specific errors.
        case UIOHOOK_ERROR_AXAPI_DISABLED:
            logger_proc(LOG_LEVEL_ERROR, "Failed to enable access for assistive devices. (%#X)", status);
            break;

        case UIOHOOK_ERROR_CREATE_EVENT_PORT:
            logger_proc(LOG_LEVEL_ERROR, "Failed to create apple event port. (%#X)", status);
            break;

        case UIOHOOK_ERROR_CREATE_RUN_LOOP_SOURCE:
            logger_proc(LOG_LEVEL_ERROR, "Failed to create apple run loop source. (%#X)", status);
            break;

        case UIOHOOK_ERROR_GET_RUNLOOP:
            logger_proc(LOG_LEVEL_ERROR, "Failed to acquire apple run loop. (%#X)", status);
            break;

        case UIOHOOK_ERROR_CREATE_OBSERVER:
            logger_proc(LOG_LEVEL_ERROR, "Failed to create apple run loop observer. (%#X)", status);
            break;

        // Default error.
        case UIOHOOK_FAILURE:
        default:
            logger_proc(LOG_LEVEL_ERROR, "An unknown hook error occurred. (%#X)", status);
            break;
    }
}


void hook_program_stop(){
    int status = hook_stop();
    switch (status) {
        case UIOHOOK_SUCCESS:
            // Everything is ok.
            break;

        // System level errors.
        case UIOHOOK_ERROR_OUT_OF_MEMORY:
            logger_proc(LOG_LEVEL_ERROR, "Failed to allocate memory. (%#X)", status);
            break;

        case UIOHOOK_ERROR_X_RECORD_GET_CONTEXT:
            // NOTE This is the only platform specific error that occurs on hook_stop().
            logger_proc(LOG_LEVEL_ERROR, "Failed to get XRecord context. (%#X)", status);
            break;

        // Default error.
        case UIOHOOK_FAILURE:
        default:
            logger_proc(LOG_LEVEL_ERROR, "An unknown hook error occurred. (%#X)", status);
            break;
    }
}