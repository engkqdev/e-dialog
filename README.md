# E-Dialog v0.4 (stabil)

E-Dialog v0.4 memfokuskan pada kemudahan penggunaan: auto-register default ON, fungsi pembungkus untuk tipe dialog umum, dan dokumentasi ringkas agar integrasi lebih cepat.

Author : engkq

Fitur utama
- Helper sederhana: EDialog_ShowSimple, EDialog_ShowInput, EDialog_ShowList
- Auto-register default = ON
- Antrian per-pemain: EDialog_ShowOrQueue, EDialog_QueueForPlayer
- Helper format kolom: EDialog_FormatColumns, EDialog_AutoPadColumns
- Sanitasi input: EDialog_SanitizeInput
- Penghasil ID dinamis: EDialog_GenerateId
- Logger sederhana: EDialog_LogEvent

Cara instal singkat
1. Salin file `include/E-Dialog.inc` ke folder `include/` proyek PAWN Anda.
2. Tambahkan `#include "E-Dialog.inc"` di gamemode.
3. (Opsional) Daftarkan dialog di `OnGameModeInit()` atau pakai auto-register.
4. Gunakan helper seperti `EDialog_ShowSimple` atau API lengkap `EDialog_Show`.

Contoh singkat
- Message: `EDialog_ShowSimple(pid, 4000, "Info", "Welcome!", "OK", "Close")`
- Input: `EDialog_ShowInput(pid, 4001, "Nick", "Enter new nick:", "", "Save", "Cancel")` (sanitasi di callback)
- List: `EDialog_ShowList(pid, 4002, "Top", "#\tNama\tSkor", rows)`

Lihat `example/example.pwn` untuk contoh implementasi.
