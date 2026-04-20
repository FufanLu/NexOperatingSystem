/* ============================================================
 * NexOS Kernel
 * Main kernel entry point. Initializes all subsystems
 * and launches the interactive shell.
 * ============================================================ */

#include "../include/screen.h"
#include "../include/filesystem.h"
#include "../include/shell.h"

void kernel_main(void) {
    /* Clear screen and show boot messages */
    clear_screen();

    print_color("[BOOT] ", CYAN_ON_BLACK);
    print("NexOS kernel loaded successfully.\n");

    /* Initialize the filesystem */
    print_color("[INIT] ", CYAN_ON_BLACK);
    print("Initializing filesystem...\n");
    fs_init();
    print_color("[INIT] ", CYAN_ON_BLACK);
    print("Filesystem ready.\n");

    /* Initialize and run the shell */
    print_color("[INIT] ", CYAN_ON_BLACK);
    print("Starting shell...\n");

    shell_run();
}
