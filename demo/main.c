#include <uiohook.h>
#include <tray.h>
#include <hook.h>
#include <personal_tray.h>
#include <stdio.h>


int main() {

    // Set the logger callback for library output.
    hook_set_logger_proc(&logger_proc);
    
    // Set the event callback for uiohook events.
    hook_set_dispatch_proc(&dispatch_proc);

    // Start the hook and block.
    // NOTE If EVENT_HOOK_ENABLED was delivered, the status will always succeed.
    // int status = create_tray();
    
    personalwhile();

    return 0;
}
