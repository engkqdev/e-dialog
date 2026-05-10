// example/example.pwn
// Demonstration for E-Dialog.inc v0.4 (stable)
// Author: engkq

#include <a_samp>
#include "../include/E-Dialog.inc"

forward EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[]);

#define DLG_INFO 4000
#define DLG_INPUT 4001
#define DLG_LIST 4002

public OnGameModeInit()
{
    // With v0.4, auto-register is ON by default for convenience.
    // You can still register manually if you prefer.
    EDialog_Register(DLG_INFO);
    EDialog_Register(DLG_INPUT);
    EDialog_Register(DLG_LIST);

    EDialog_SetGlobalHeader("=== My Server ===\n  %s\n================\n");

    return 1;
}

// Quick message
stock ShowInfo(playerid)
{
    EDialog_ShowSimple(playerid, DLG_INFO, "Info", "Selamat datang di server kami!", "OK", "Close");
    return 1;
}

// Quick input prompt
stock PromptNickname(playerid)
{
    EDialog_ShowInput(playerid, DLG_INPUT, "Ganti Nick", "Masukkan nickname baru:", "", "Simpan", "Batal");
    return 1;
}

// Quick list
stock ShowTopPlayers(playerid)
{
    new rows[512];
    format(rows, sizeof rows,
        "1\tAlpha\t2000\n"
        "2\tBeta\t1800\n"
        "3\tGamma\t1500"
    );
    EDialog_ShowList(playerid, DLG_LIST, "Top Players", "#\tName\tScore", rows);
    return 1;
}

public EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == DLG_INFO) {
        SendClientMessage(playerid, 0xFFFFFFAA, "Anda menutup dialog info.");
    }
    else if (dialogid == DLG_INPUT) {
        if (response == 1) {
            new clean[64];
            EDialog_SanitizeInput(inputtext, clean, sizeof clean);
            new buf[128];
            format(buf, sizeof buf, "Nickname updated: %s", clean);
            SendClientMessage(playerid, 0x00FF00FF, buf);
        } else {
            SendClientMessage(playerid, 0xFF0000FF, "Cancel.");
        }
    }
    else if (dialogid == DLG_LIST) {
        if (response == 0) {
            new buf[64];
            format(buf, sizeof buf, "You picked row %d", listitem);
            SendClientMessage(playerid, 0xFFFFFF00, buf);
        }
    }

    // Log event for debugging
    new logmsg[128];
    format(logmsg, sizeof logmsg, "P%d D%d R%d", playerid, dialogid, response);
    EDialog_LogEvent(logmsg);

    return 1;
}
