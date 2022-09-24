#ifndef MAIN_H
#define MAIN_H

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

#if TRAY_APPINDICATOR
#define TRAY_ICON1 "indicator-messages"
#define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#define TRAY_ICON1 "icon.png"
#define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#define TRAY_ICON1 "icon.ico"
#define TRAY_ICON2 "icon.ico"
#endif

static struct tray tray;

enum Menu_Option{
    running_state = 0,
    disabled
};


static void toggle_cb(struct tray_menu *item);

static void hello_cb(struct tray_menu *item);

static void quit_cb(struct tray_menu *item);

static void submenu_cb(struct tray_menu *item);




static struct tray_menu menu[];

static struct tray tray = {
    .icon = TRAY_ICON1,
    .menu = menu
};


/**
 * @brief this is for caplock funciton key
 * 
 */
static short is_doubled = FUNCTION_IDLE;

/**
 * @brief 
 * 
 */
bool logger_proc(unsigned int level, const char *format, ...);

void dispatch_proc(uiohook_event * const event);

void hook_program_run();

void hook_program_stop();


#endif