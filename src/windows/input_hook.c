/* libUIOHook: Cross-platform keyboard hooking from userland.
 * Copyright (C) 2006-2022 Alexander Barker.  All Rights Reserved.
 * https://github.com/kwhat/libuiohook/
 *
 * libUIOHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libUIOHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <inttypes.h>
#include <uiohook.h>
#include <windows.h>

#include "input_helper.h"
#include "logger.h"

// Thread and hook handles.
static DWORD hook_thread_id = 0;
static HHOOK keyboard_event_hhook = NULL;
static HWINEVENTHOOK win_event_hhook = NULL;

// The handle to the DLL module pulled in DllMain on DLL_PROCESS_ATTACH.
extern HINSTANCE hInst;

// Modifiers for tracking key masks.
static unsigned short int current_modifiers = 0x0000;

// Click count globals.
static unsigned short click_count = 0;
static DWORD click_time = 0;
static POINT last_click;

// Static event memory.
static uiohook_event event;

// Event dispatch callback.
static dispatcher_t dispatcher = NULL;

UIOHOOK_API void hook_set_dispatch_proc(dispatcher_t dispatch_proc) {
    logger(LOG_LEVEL_DEBUG, "%s [%u]: Setting new dispatch callback to %#p.\n",
            __FUNCTION__, __LINE__, dispatch_proc);

    dispatcher = dispatch_proc;
}

// Send out an event if a dispatcher was set.
static inline void dispatch_event(uiohook_event *const event) {
    if (dispatcher != NULL) {
        logger(LOG_LEVEL_DEBUG, "%s [%u]: Dispatching event type %u.\n",
                __FUNCTION__, __LINE__, event->type);

        dispatcher(event);
    } else {
        logger(LOG_LEVEL_WARN, "%s [%u]: No dispatch callback set!\n",
                __FUNCTION__, __LINE__);
    }
}


// Set the native modifier mask for future events.
static inline void set_modifier_mask(unsigned short int mask) {
    current_modifiers |= mask;
}

// Unset the native modifier mask for future events.
static inline void unset_modifier_mask(unsigned short int mask) {
    current_modifiers &= ~mask;
}

// Get the current native modifier mask state.
static inline unsigned short int get_modifiers() {
    return current_modifiers;
}

// Initialize the modifier mask to the current modifiers.
static void initialize_modifiers() {
    current_modifiers = 0x0000;

    // NOTE We are checking the high order bit, so it will be < 0 for a singed short.
    if (GetKeyState(VK_LSHIFT)   < 0) { set_modifier_mask(MASK_SHIFT_L);     }
    if (GetKeyState(VK_RSHIFT)   < 0) { set_modifier_mask(MASK_SHIFT_R);     }
    if (GetKeyState(VK_LCONTROL) < 0) { set_modifier_mask(MASK_CTRL_L);      }
    if (GetKeyState(VK_RCONTROL) < 0) { set_modifier_mask(MASK_CTRL_R);      }
    if (GetKeyState(VK_LMENU)    < 0) { set_modifier_mask(MASK_ALT_L);       }
    if (GetKeyState(VK_RMENU)    < 0) { set_modifier_mask(MASK_ALT_R);       }
    if (GetKeyState(VK_LWIN)     < 0) { set_modifier_mask(MASK_META_L);      }
    if (GetKeyState(VK_RWIN)     < 0) { set_modifier_mask(MASK_META_R);      }

    if (GetKeyState(VK_LBUTTON)  < 0) { set_modifier_mask(MASK_BUTTON1);     }
    if (GetKeyState(VK_RBUTTON)  < 0) { set_modifier_mask(MASK_BUTTON2);     }
    if (GetKeyState(VK_MBUTTON)  < 0) { set_modifier_mask(MASK_BUTTON3);     }
    if (GetKeyState(VK_XBUTTON1) < 0) { set_modifier_mask(MASK_BUTTON4);     }
    if (GetKeyState(VK_XBUTTON2) < 0) { set_modifier_mask(MASK_BUTTON5);     }

    if (GetKeyState(VK_NUMLOCK)  < 0) { set_modifier_mask(MASK_NUM_LOCK);    }
    if (GetKeyState(VK_CAPITAL)  < 0) { set_modifier_mask(MASK_CAPS_LOCK);   }
    if (GetKeyState(VK_SCROLL)   < 0) { set_modifier_mask(MASK_SCROLL_LOCK); }
}


void unregister_running_hooks() {
    // Stop the event hook and any timer still running.
    if (win_event_hhook != NULL) {
        UnhookWinEvent(win_event_hhook);
        win_event_hhook = NULL;
    }

    // Destroy the native hooks.
    if (keyboard_event_hhook != NULL) {
        UnhookWindowsHookEx(keyboard_event_hhook);
        keyboard_event_hhook = NULL;
    }

}

void hook_start_proc() {
    // Initialize native input helper functions.
    load_input_helper();

    // Get the local system time in UNIX epoch form.
    uint64_t timestamp = GetMessageTime();

    // Populate the hook start event.
    event.time = timestamp;
    event.reserved = 0x00;

    event.type = EVENT_HOOK_ENABLED;
    event.mask = 0x00;

    // Fire the hook start event.
    dispatch_event(&event);
}

void hook_stop_proc() {
    // Get the local system time in UNIX epoch form.
    uint64_t timestamp = GetMessageTime();

    // Populate the hook stop event.
    event.time = timestamp;
    event.reserved = 0x00;

    event.type = EVENT_HOOK_DISABLED;
    event.mask = 0x00;

    // Fire the hook stop event.
    dispatch_event(&event);

    // Deinitialize native input helper functions.
    unload_input_helper();
}

static void process_key_pressed(KBDLLHOOKSTRUCT *kbhook) {
    // Check and setup modifiers.
    if      (kbhook->vkCode == VK_LSHIFT)   { set_modifier_mask(MASK_SHIFT_L);     }
    else if (kbhook->vkCode == VK_RSHIFT)   { set_modifier_mask(MASK_SHIFT_R);     }
    else if (kbhook->vkCode == VK_LCONTROL) { set_modifier_mask(MASK_CTRL_L);      }
    else if (kbhook->vkCode == VK_RCONTROL) { set_modifier_mask(MASK_CTRL_R);      }
    else if (kbhook->vkCode == VK_LMENU)    { set_modifier_mask(MASK_ALT_L);       }
    else if (kbhook->vkCode == VK_RMENU)    { set_modifier_mask(MASK_ALT_R);       }
    else if (kbhook->vkCode == VK_LWIN)     { set_modifier_mask(MASK_META_L);      }
    else if (kbhook->vkCode == VK_RWIN)     { set_modifier_mask(MASK_META_R);      }
    else if (kbhook->vkCode == VK_NUMLOCK)  { set_modifier_mask(MASK_NUM_LOCK);    }
    else if (kbhook->vkCode == VK_CAPITAL)  { set_modifier_mask(MASK_CAPS_LOCK);   }
    else if (kbhook->vkCode == VK_SCROLL)   { set_modifier_mask(MASK_SCROLL_LOCK); }

    // Populate key pressed event.
    event.time = kbhook->time;
    event.reserved = 0x00;

    event.type = EVENT_KEY_PRESSED;
    event.mask = get_modifiers();

    event.data.keyboard.keycode = keycode_to_scancode(kbhook->vkCode, kbhook->flags);
    event.data.keyboard.rawcode = (uint16_t) kbhook->vkCode;
    event.data.keyboard.keychar = CHAR_UNDEFINED;

    logger(LOG_LEVEL_DEBUG, "%s [%u]: Key %#X pressed. (%#X)\n",
            __FUNCTION__, __LINE__, event.data.keyboard.keycode, event.data.keyboard.rawcode);

    // Populate key pressed event.
    dispatch_event(&event);

    // If the pressed event was not consumed...
    if (event.reserved ^ 0x01) {
        // Buffer for unicode typed chars. No more than 2 needed.
        WCHAR buffer[2]; // = { WCH_NONE };

        // If the pressed event was not consumed and a unicode char exists...
        SIZE_T count = keycode_to_unicode(kbhook->vkCode, buffer, sizeof(buffer));
        for (unsigned int i = 0; i < count; i++) {
            // Populate key typed event.
            event.time = kbhook->time;
            event.reserved = 0x00;

            event.type = EVENT_KEY_TYPED;
            event.mask = get_modifiers();

            event.data.keyboard.keycode = VC_UNDEFINED;
            event.data.keyboard.rawcode = (uint16_t) kbhook->vkCode;
            event.data.keyboard.keychar = buffer[i];

            logger(LOG_LEVEL_DEBUG, "%s [%u]: Key %#X typed. (%lc)\n",
                    __FUNCTION__, __LINE__, event.data.keyboard.keycode, (wint_t) event.data.keyboard.keychar);

            // Fire key typed event.
            dispatch_event(&event);
        }
    }
}

static void process_key_released(KBDLLHOOKSTRUCT *kbhook) {
    // Check and setup modifiers.
    if      (kbhook->vkCode == VK_LSHIFT)   { unset_modifier_mask(MASK_SHIFT_L);     }
    else if (kbhook->vkCode == VK_RSHIFT)   { unset_modifier_mask(MASK_SHIFT_R);     }
    else if (kbhook->vkCode == VK_LCONTROL) { unset_modifier_mask(MASK_CTRL_L);      }
    else if (kbhook->vkCode == VK_RCONTROL) { unset_modifier_mask(MASK_CTRL_R);      }
    else if (kbhook->vkCode == VK_LMENU)    { unset_modifier_mask(MASK_ALT_L);       }
    else if (kbhook->vkCode == VK_RMENU)    { unset_modifier_mask(MASK_ALT_R);       }
    else if (kbhook->vkCode == VK_LWIN)     { unset_modifier_mask(MASK_META_L);      }
    else if (kbhook->vkCode == VK_RWIN)     { unset_modifier_mask(MASK_META_R);      }
    else if (kbhook->vkCode == VK_NUMLOCK)  { unset_modifier_mask(MASK_NUM_LOCK);    }
    else if (kbhook->vkCode == VK_CAPITAL)  { unset_modifier_mask(MASK_CAPS_LOCK);   }
    else if (kbhook->vkCode == VK_SCROLL)   { unset_modifier_mask(MASK_SCROLL_LOCK); }

    // Populate key pressed event.
    event.time = kbhook->time;
    event.reserved = 0x00;

    event.type = EVENT_KEY_RELEASED;
    event.mask = get_modifiers();

    event.data.keyboard.keycode = keycode_to_scancode(kbhook->vkCode, kbhook->flags);
    event.data.keyboard.rawcode = (uint16_t) kbhook->vkCode;
    event.data.keyboard.keychar = CHAR_UNDEFINED;

    logger(LOG_LEVEL_DEBUG, "%s [%u]: Key %#X released. (%#X)\n",
            __FUNCTION__, __LINE__, event.data.keyboard.keycode, event.data.keyboard.rawcode);

    // Fire key released event.
    dispatch_event(&event);
}

LRESULT CALLBACK keyboard_hook_event_proc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT *kbhook = (KBDLLHOOKSTRUCT *) lParam;
    switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            process_key_pressed(kbhook);
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            process_key_released(kbhook);
            break;

        default:
            // In theory this *should* never execute.
            logger(LOG_LEVEL_DEBUG, "%s [%u]: Unhandled Windows keyboard event: %#X.\n",
                    __FUNCTION__, __LINE__, (unsigned int) wParam);
            break;
    }

    LRESULT hook_result = -1;
    if (nCode < 0 || event.reserved ^ 0x01) {
        hook_result = CallNextHookEx(keyboard_event_hhook, nCode, wParam, lParam);
    } else {
        logger(LOG_LEVEL_DEBUG, "%s [%u]: Consuming the current event. (%li)\n",
                __FUNCTION__, __LINE__, (long) hook_result);
    }

    return 1;
}


// Callback function that handles events.
void CALLBACK win_hook_event_proc(HWINEVENTHOOK hook, DWORD event, HWND hWnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime) {
    switch (event) {
        case EVENT_OBJECT_NAMECHANGE:
            logger(LOG_LEVEL_DEBUG, "%s [%u]: Restarting Windows input hook on window event: %#X.\n",
                    __FUNCTION__, __LINE__, event);

            // Remove any keyboard hooks that are still running.
            if (keyboard_event_hhook != NULL) {
                UnhookWindowsHookEx(keyboard_event_hhook);
            }


            // Restart the event hooks.
            keyboard_event_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_event_proc, hInst, 0);

            // Re-initialize modifier masks.
            initialize_modifiers();

            // FIXME We should compare the modifier mask before and after the restart
            // to determine if we should synthesize missing events.

            // Check for event hook error.
            if (keyboard_event_hhook == NULL) {
                logger(LOG_LEVEL_ERROR, "%s [%u]: SetWindowsHookEx() failed! (%#lX)\n",
                        __FUNCTION__, __LINE__, (unsigned long) GetLastError());
            }
            break;

        default:
            logger(LOG_LEVEL_DEBUG, "%s [%u]: Unhandled Windows window event: %#X.\n",
                    __FUNCTION__, __LINE__, event);
    }
}


UIOHOOK_API int hook_run() {
    int status = UIOHOOK_FAILURE;

    // Set the thread id we want to signal later.
    hook_thread_id = GetCurrentThreadId();

    // Spot check the hInst in case the library was statically linked and DllMain
    // did not receive a pointer on load.
    if (hInst == NULL) {
        logger(LOG_LEVEL_WARN, "%s [%u]: hInst was not set by DllMain().\n",
                __FUNCTION__, __LINE__);

        hInst = GetModuleHandle(NULL);
        if (hInst != NULL) {
            // Initialize native input helper functions.
            load_input_helper();
        } else {
            logger(LOG_LEVEL_ERROR, "%s [%u]: Could not determine hInst for SetWindowsHookEx()! (%#lX)\n",
                    __FUNCTION__, __LINE__, (unsigned long) GetLastError());

            status = UIOHOOK_ERROR_GET_MODULE_HANDLE;
        }
    }

    // Create the native hooks.
    keyboard_event_hhook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_hook_event_proc, hInst, 0);

    // Create a window event hook to listen for capture change.
    win_event_hhook = SetWinEventHook(
            EVENT_OBJECT_NAMECHANGE, EVENT_OBJECT_NAMECHANGE,
            NULL,
            win_hook_event_proc,
            0, 0,
            WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);

    // If we did not encounter a problem, start processing events.
    if (keyboard_event_hhook != NULL) {
        if (win_event_hhook == NULL) {
            logger(LOG_LEVEL_WARN, "%s [%u]: SetWinEventHook() failed!\n",
                    __FUNCTION__, __LINE__);
        }

        logger(LOG_LEVEL_DEBUG, "%s [%u]: SetWindowsHookEx() successful.\n",
                __FUNCTION__, __LINE__);

        // Check and setup modifiers.
        initialize_modifiers();

        // Set the exit status.
        status = UIOHOOK_SUCCESS;

        // Windows does not have a hook start event or callback so we need to
        // manually fake it.
        hook_start_proc();

        // Block until the thread receives an WM_QUIT request.
        MSG message;
        while (GetMessage(&message, (HWND) NULL, 0, 0) > 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    } else {
        logger(LOG_LEVEL_ERROR, "%s [%u]: SetWindowsHookEx() failed! (%#lX)\n",
                __FUNCTION__, __LINE__, (unsigned long) GetLastError());

        status = UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX;
    }


    // Unregister any hooks that may still be installed.
    unregister_running_hooks();

    // We must explicitly call the cleanup handler because Windows does not
    // provide a thread cleanup method like POSIX pthread_cleanup_push/pop.
    hook_stop_proc();

    return status;
}

UIOHOOK_API int hook_stop() {
    int status = UIOHOOK_FAILURE;

    // Try to exit the thread naturally.
    if (PostThreadMessage(hook_thread_id, WM_QUIT, (WPARAM) NULL, (LPARAM) NULL)) {
        status = UIOHOOK_SUCCESS;
    }

    logger(LOG_LEVEL_DEBUG, "%s [%u]: Status: %#X.\n",
            __FUNCTION__, __LINE__, status);

    return status;
}
