// example/example.pwn
// Demonstrasi E-Dialog.inc v0.3 (stable)

#include <a_samp>
#include "../include/E-Dialog.inc"

forward EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[]);

#define DLG_LEADERBOARD 3000
#define DLG_PROMPT      3001

public OnGameModeInit()
{
    EDialog_Register(DLG_LEADERBOARD);
    EDialog_Register(DLG_PROMPT);

    // Set global header template (gunakan %s untuk judul)
    EDialog_SetGlobalHeader("=== E-Dialog v" EDLG_VERSION " ===\n  %s\n========================\n");

    // Set per-dialog header khusus
    EDialog_SetDialogHeader(DLG_PROMPT, "-- Prompt Dialog --\n  %s\n--------------------\n");

    // Auto-register off (opsional)
    EDialog_SetAutoRegister(0);

    return 1;
}

stock ShowLeaderboard(playerid)
{
    new rows[512];

    // Example: build rows without relying on tabs; use EDialog_AutoPadColumns or EDialog_FormatColumns if desired
    format(rows, sizeof rows,
        "1\tBudi\t1500\n"
        "2\tSiti\t1200\n"
        "3\tAndi\t1100"
    );

    new info[1024];
    EDialog_MakeListInfo(info, sizeof info, "Rank\tName\tScore", rows);

    EDialog_Show(playerid, DLG_LEADERBOARD, 1, "Leaderboard", info, "Tutup", "Refresh");
    return 1;
}

stock AskNickname(playerid)
{
    EDialog_Show(playerid, DLG_PROMPT, 2, "Ganti Nick", "Masukkan nickname baru:", "Simpan", "Batal");
    return 1;
}

// Example usage of some new helpers
stock ExampleGenerateIdAndQueue(playerid)
{
    new dynId = EDialog_GenerateId("dyn");
    // register dynamic id and set header
    EDialog_Register(dynId);
    EDialog_SetDialogHeader(dynId, "-- Dynamic --\n  %s\n--------------\n");

    new title[64];
    format(title, sizeof title, "Dialog %d", dynId);
    new info[256];
    format(info, sizeof info, "Ini dialog dinamis dengan id %d", dynId);

    // Queue it for the player
    EDialog_QueueForPlayer(playerid, dynId, 0, title, info, "OK", "Batal");
    return 1;
}

public EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DLG_LEADERBOARD) {
        if (response == 1) {
            SendClientMessage(playerid, 0xFFFFFFFF, "Anda menutup leaderboard.");
        } else if (response == 2) {
            SendClientMessage(playerid, 0x00FF00FF, "Memuat ulang leaderboard...");
            ShowLeaderboard(playerid);
        } else if (response == 0) {
            new buf[128];
            format(buf, sizeof buf, "Anda memilih baris: %d", listitem);
            SendClientMessage(playerid, 0xFFFFFF00, buf);
        }
    } else if (dialogid == DLG_PROMPT) {
        if (response == 1) {
            new clean[96];
            EDialog_SanitizeInput(inputtext, clean, sizeof clean);
            new buf[96];
            format(buf, sizeof buf, "Nickname baru: %s", clean);
            SendClientMessage(playerid, 0x00FF00FF, buf);
        } else {
            SendClientMessage(playerid, 0xFF0000FF, "Ganti nickname dibatalkan.");
        }
    } else {
        // dynamic or queued dialog handling
        new msg[128];
        format(msg, sizeof msg, "Dialog %d diklik, response=%d", dialogid, response);
        SendClientMessage(playerid, 0x00FFFF00, msg);
    }

    // Log event
    new logmsg[128];
    format(logmsg, sizeof logmsg, "P%d: D%d R%d", playerid, dialogid, response);
    EDialog_LogEvent(logmsg);

    return 1;
}
