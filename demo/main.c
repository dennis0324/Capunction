// #include <main.h>

// int main() {

//     // Set the logger callback for library output.
//     hook_set_logger_proc(&logger_proc);
    
//     // Set the event callback for uiohook events.
//     hook_set_dispatch_proc(&dispatch_proc);

//     // Start the hook and block.
//     // NOTE If EVENT_HOOK_ENABLED was delivered, the status will always succeed.
//     if (tray_init(&tray) < 0) {
//         printf("failed to create tray\n");
//         return 1;
//     }
    
//     toggle_cb(&menu[0]);
    
//     while (tray_loop(1) == 0);

//     return 0;
// }
