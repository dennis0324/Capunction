// #include <uiohook.h>
#include <stdio.h>
#include <string.h>
#include <tray.h>
#include <hook.h>
#include <capunction_tray.h>

#include <stdio.h>

int main() {
    // #ifdef 
    // Set the logger callback for library output.
    hook_set_logger_proc(&logger_proc);
    
    // Set the event callback for uiohook events.
    hook_set_dispatch_proc(&dispatch_proc);

    
    personalwhile();
    return 0;
}
