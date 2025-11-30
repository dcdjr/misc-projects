# Explanation

## STEP 1: Windows generates events

Windows creates an event every time something happens:

- Keyboard press → keyboard event
- Mouse click → mouse event
- Window moved → window event

By default, these events are delivered only to the active window.

---

## STEP 2: A hook lets you intercept events before they reach the window

A hook is a function that Windows calls whenever a specific type of event occurs.

For keyboard events, any program that installs a keyboard hook gets called before the keystroke reaches the target program.

---

## STEP 3: SetWindowsHookEx installs a hook

SetWindowsHookEx(idHook, lpfn, hMod, dwThreadId)

**Parameters:**

- **idHook**  
  Which event to intercept.  
  `WH_KEYBOARD_LL` = low-level keyboard hook that captures keystrokes from all applications.

- **lpfn**  
  Pointer to the hook function.  
  Windows will call this function whenever a keyboard event occurs.

- **hMod**  
  Module (DLL/exe) containing the callback.  
  We pass `NULL` because the callback is inside our own executable.  
  A callback = a function you write but Windows calls.

- **dwThreadId**  
  `0` = hook all threads (global), or a specific thread ID to hook only that thread.

**Return value:**  
Returns an `HHOOK` handle on success, `NULL` on failure.

---

## STEP 4: What the hook function looks like

Windows requires a strict signature:

LRESULT CALLBACK HookName(int nCode, WPARAM wParam, LPARAM lParam)

**Parameters:**

- **nCode**  
  Indicates whether the event contains valid information.  
  For low-level keyboard hooks, `HC_ACTION` means the event is valid.

- **wParam**  
  Identifies the type of keyboard event:  
  `WM_KEYDOWN`, `WM_KEYUP`, `WM_SYSKEYDOWN`, `WM_SYSKEYUP`.  
  Key presses arrive as `WM_KEYDOWN`.

- **lParam**  
  Pointer to a `KBDLLHOOKSTRUCT` containing key event data:
  - `vkCode` → virtual key code  
  - `scanCode` → hardware scan code  
  - `flags` → extended/injected flags  
  - `time` → timestamp  

This structure tells you exactly which key was pressed.

---

## STEP 5: Why the callback ends with CallNextHookEx

Multiple hooks may be installed at once (Windows, antivirus, accessibility tools, your program, etc.).

Hooks form a chain.

CallNextHookEx(...)

tells Windows to pass the event to the next hook in the chain.

If you do NOT call this:

- Normal keyboard behavior breaks
- Other programs malfunction
- Windows may remove your hook

---

## STEP 6: Why GetMessage() is required

Low-level hooks run in the context of your thread.  
If your thread exits, Windows automatically removes your hook.

GetMessage(&msg, NULL, 0, 0)

This:

- Blocks the thread
- Keeps it alive
- Allows hook callbacks to continue being delivered

Without the loop:

- The program reaches the end of `main()`
- The process exits instantly
- The hook is removed

**What GetMessage actually does:**

- Waits for a Windows event
- Copies the event into `msg`
- Returns `true` if the program should keep running