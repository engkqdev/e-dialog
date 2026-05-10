# E-Dialog

Simple, clean dialog manager untuk SA-MP (PAWN).

Versi: v0.3 (stable)

Fitur utama:
- Header templating: global dan per-dialog (gunakan `%s` sebagai placeholder untuk title)
- Alias `edialog_header` untuk format header
- Pendaftaran (register/unregister) dialog ID
- Wrapper `EDialog_Show` untuk menampilkan dialog dengan header otomatis
- Helper `EDialog_MakeListInfo` untuk membuat list-info (gunakan `\t` di string untuk kolom rapi)
- Per-player queue: `EDialog_QueueForPlayer`, `EDialog_ShowNextQueued`, `EDialog_ClearPlayerQueue`
- Auto-padding helpers: `EDialog_FormatColumns`, `EDialog_AutoPadColumns`
- Input sanitization: `EDialog_SanitizeInput`
- ID generator: `EDialog_GenerateId`
- Simple in-memory logger: `EDialog_LogEvent`

Instalasi:
1. Salin file `include/E-Dialog.inc` ke folder `include/` di project PAWN Anda.
2. Tambahkan `#include "E-Dialog.inc"` di gamemode Anda (atau gunakan relative include seperti `"../include/E-Dialog.inc"` untuk contoh).
3. Daftarkan dialog ID yang akan digunakan di `OnGameModeInit()`:
   `EDialog_Register(DLG_ID);`
4. (Opsional) set header global: `EDialog_SetGlobalHeader("=== Server ===\n  %s\n================\n");`
5. (Opsional) set header per-dialog: `EDialog_SetDialogHeader(DLG_ID, "-- My Dialog --\n  %s\n----------------\n");`
6. Panggil `EDialog_Show(playerid, dialogid, style, title, info, btn1, btn2)` saat diperlukan.
7. Implementasikan forward `EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[])` untuk memproses balikan.

Semua fungsi (cara pemasangan singkat):

- EDialog_Register(dialogid)
  - Panggil di `OnGameModeInit()` untuk mendaftarkan dialog id agar include meng-intercept respons.

- EDialog_Unregister(dialogid)
  - Panggil jika Anda ingin melepas registrasi dialog (opsional).

- EDialog_SetGlobalHeader(template[])
  - Template harus mengandung `%s` untuk title; contoh: `EDialog_SetGlobalHeader("=== Server ===\n  %s\n================\n");`

- EDialog_SetDialogHeader(dialogid, template[])
  - Panggil setelah `EDialog_Register(dialogid)`; template juga harus mengandung `%s`.

- EDialog_FormatHeader(out[], outlen, title[], dialogid)
  - Biasanya tidak diperlukan langsung; digunakan internal. Anda dapat memanggil `edialog_header` alias juga.

- EDialog_Show(playerid, dialogid, style, title[], info[], button1[], button2[])
  - Panggil di mana saja (command, event). style: 0=msgbox, 1=list, 2=input.

- EDialog_MakeListInfo(out[], outlen, columnHeader[], rows[])
  - Gunakan untuk membuat info pada list; disarankan gunakan `\t` dalam `columnHeader` dan `rows`.

- EDialog_GenerateId(prefix[])
  - Menghasilkan id numerik unik (incremental). Gunakan saat membuat dialog dinamis.

- EDialog_SanitizeInput(input[], out[], outlen)
  - Bersihkan input dari karakter kontrol; panggilan contoh ada di `example/example.pwn`.

- EDialog_FormatColumns(out[], outlen, row[], widths[], colcount)
  - Format kolom menggunakan lebar yang ditentukan. `row` berisi kolom yang dipisah `\t`.

- EDialog_AutoPadColumns(out[], outlen, row[], maxWidth)
  - Simple pad columns to fixed width; `row` uses `\t` separators.

- EDialog_QueueForPlayer(playerid, dialogid, style, title[], info[], button1[], button2[])
  - Menambahkan dialog ke antrian pemain. Berguna saat pemain sedang melihat dialog lain.

- EDialog_ShowNextQueued(playerid)
  - Menampilkan dialog berikutnya pada antrian pemain (otomatis dipanggil setelah OnResponse jika dialog terdaftar).

- EDialog_ClearPlayerQueue(playerid)
  - Menghapus semua entri antrian untuk pemain.

- EDialog_GetQueuedCount(playerid)
  - Mengembalikan jumlah dialog yang saat ini ada di antrian pemain.

- EDialog_ShowOrQueue(playerid, dialogid, style, title[], info[], button1[], button2[])
  - Shortcut: jika pemain tidak memiliki antrian, tampilkan langsung, jika ada maka antri.

- EDialog_LogEvent(message[])
  - Simpan pesan log (dalam-memory ring buffer). Berguna untuk debugging; tambahkan mekanisme dump jika perlu.

Contoh singkat (lihat `example/example.pwn`):
- Daftarkan dialog di `OnGameModeInit()`.
- Set header global / per-dialog jika perlu.
- Panggil `EDialog_Show(...)` atau `EDialog_ShowOrQueue(...)` untuk menampilkan dialog.
- Tangani `EDialog_OnResponse(...)` di gamemode untuk memproses aksi pemain.

Releases:
- v0.3 (stable): initial stable release with queueing, auto-padding helpers, sanitize, ID generator, and example.

Lisensi:
Tambahkan file LICENSE jika Anda ingin menyertakan lisensi (misal MIT). Saat ini repository tidak menyertakan file LICENSE.
