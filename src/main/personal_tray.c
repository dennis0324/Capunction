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

#include <stdio.h>

#include <stdlib.h>
#include <combinationInput.h>

#define TRAY_WINAPI 1

#include <tray.h>
#include <hook.h>
#include <personal_tray.h>



#if TRAY_APPINDICATOR
#define TRAY_ICON1 "indicator-messages"
#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#define TRAY_ICON1 "icon.png"
#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#define TRAY_ICON1 "Capunction.exe"
#endif

static struct tray tray;


static void about(struct tray_menu *item){
    (void)item;
    printf("about\n");  
    ShellExecute(NULL, "open", "https://github.com/dennis0324/Capunction", NULL, NULL, SW_SHOWNORMAL);
    tray_update(&tray);
}

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

static struct tray_menu menuSelect[] = {
    {.text = "About", .checked = 0, .cb = about},
    {.text = "-"},
    {.text = "Running", .checked = 0, .cb = toggle_cb},
    {.text = "-"},
    {.text = "Quit", .cb = quit_cb},
    {.text = NULL}
};

static struct tray tray = {
    .icon = TRAY_ICON1,
    .menu = menuSelect
};



int create_tray(){
    if (tray_init(&tray) < 0) {
        printf("failed to create tray\n");
        return 1; 
    }

    return 0;
}

void set_toggle(){
    enum Menu_Option menu_option;
    menu_option = running_state;
    toggle_cb(&menuSelect[menu_option]);
}

struct tray get_tray(){
    return tray;
}

int personalwhile(){
    int status = create_tray();
    set_toggle();
    while (tray_loop(1) == 0);
    return status;
}


