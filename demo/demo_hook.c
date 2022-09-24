/* libUIOHook: Cross-platform keyboard and mouse hooking from userland.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <uiohook.h>
#include <wchar.h>


#include <stdlib.h>
#include <combinationInput.h>


#define TRAY_WINAPI 1

#include <tray.h>
#include <main.h>


static void toggle_cb(struct tray_menu *item) {
    printf("toggle cb\n");
    item->checked = !item->checked;
    tray_update(&tray);
    if(item->checked){
        printf("running");
        hook_program_run();    
    }
    else{
        hook_program_stop();
    }
}

static void hello_cb(struct tray_menu *item) {
  (void)item;
  printf("hello cb\n");
  if (strcmp(tray.icon, TRAY_ICON1) == 0) {
    tray.icon = TRAY_ICON2;
  } else {
    tray.icon = TRAY_ICON1;
  }
  tray_update(&tray);
}

static void quit_cb(struct tray_menu *item) {
    (void)item;
    printf("quit cb\n");
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
    tray_exit();
}

static void submenu_cb(struct tray_menu *item) {
  (void)item;
  printf("submenu: clicked on %s\n", item->text);
  tray_update(&tray);
}

static struct tray_menu menu[] = {
            {.text = "running", .checked = 0, .cb = toggle_cb},
            {.text = "Disabled", .disabled = 1},
            {.text = "-"},
            {.text = "Quit", .cb = quit_cb},
            {.text = NULL}
            };




int main() {

    // Set the logger callback for library output.
    hook_set_logger_proc(&logger_proc);
    
    // Set the event callback for uiohook events.
    hook_set_dispatch_proc(&dispatch_proc);

    // Start the hook and block.
    // NOTE If EVENT_HOOK_ENABLED was delivered, the status will always succeed.
    if (tray_init(&tray) < 0) {
        printf("failed to create tray\n");
        return 1;
    }

    toggle_cb(&menu[0]);
    
    while (tray_loop(1) == 0);

    return 0;
}
