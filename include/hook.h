
#ifndef HOOK_H
#define HOOK_H

bool logger_proc(unsigned int level, const char *format, ...);

void dispatch_proc(uiohook_event * const event);

void hook_program_run();

void hook_program_stop();

#endif