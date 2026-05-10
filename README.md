# E-Dialog v0.4 (stable)

E-Dialog v0.4 focuses on ease-of-use: default auto-registration, simple wrapper functions for common dialog types, and clear documentation so you can integrate dialogs with minimal code.

Author: engkq

What's new in v0.4 (summary)
- Default auto-register ON (EDialog_SetAutoRegister still available)
- Simple helpers: EDialog_ShowSimple, EDialog_ShowInput, EDialog_ShowList
- Backwards-compatible with v0.3 (all previous helpers remain)
- Queueing and logging improved limits and clearer examples

Installation (quick)
1. Copy `include/E-Dialog.inc` to your project's `include/` folder.
2. `#include "E-Dialog.inc"` in your gamemode.
3. (Optional) Register dialog IDs in `OnGameModeInit()` via `EDialog_Register(ID)` or rely on auto-register.
4. Use the simple helpers or the full API as needed.

Detailed usage (step-by-step examples)

1) Simple message box (one call)
- Use: EDialog_ShowSimple(playerid, dialogid, title, message, okButton, cancelButton)
- Example: EDialog_ShowSimple(playerid, 4000, "Info", "Welcome!", "OK", "Close");
- What to handle: implement `EDialog_OnResponse` and check `dialogid`.

2) Input prompt (one call)
- Use: EDialog_ShowInput(playerid, dialogid, title, prompt, defaultText, saveButton, cancelButton)
- Example: EDialog_ShowInput(playerid, 4001, "Nickname", "Enter new nick:", "", "Save", "Cancel");
- In EDialog_OnResponse: when response == 1, sanitize input using `EDialog_SanitizeInput(inputtext, out, outlen)`.

3) List display
- Use: EDialog_ShowList(playerid, dialogid, title, columnHeader, rows)
- columnHeader and each row should use `\t` between columns and `\n` between rows.
- Example:
  new rows[] = "1\tAlice\t2000\n2\tBob\t1800";
  EDialog_ShowList(pid, 4002, "Top", "#\tName\tScore", rows);
- Handle selection: in EDialog_OnResponse, when response == 0, `listitem` contains selected row index.

4) Queueing
- Show-or-queue: EDialog_ShowOrQueue(playerid, dialogid, style, title, info, btn1, btn2)
- Manually queue: EDialog_QueueForPlayer(...)
- Show next queued: EDialog_ShowNextQueued(playerid)

5) Advanced helpers
- EDialog_FormatColumns / EDialog_AutoPadColumns: create nicely padded column strings if you don't want to use `\t`.
- EDialog_GenerateId(prefix): create numeric id for dynamic dialogs.
- EDialog_LogEvent(msg): keep small in-memory logs for debugging.

Example flow (recommended):
1. Call EDialog_ShowInput(...) when you need a text input.
2. In EDialog_OnResponse: sanitize input with EDialog_SanitizeInput and store.
3. If you need multiple dialogs in sequence, use EDialog_ShowOrQueue so dialogs won't overlap.

Files included in this release
- include/E-Dialog.inc (v0.4)
- example/example.pwn (demo)
- README.md / RELEASES/v0.4.md
