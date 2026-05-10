// example/example.pwn
// Contoh penggunaan E-Dialog v0.4 (stabil)
// Penulis: engkq

#include <a_samp>
#include "../include/E-Dialog.inc"

forward EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[]);

#define DLG_INFO 4000
#define DLG_INPUT 4001
#define DLG_LIST 4002

public OnGameModeInit()
{
    // Auto-register default ON; pendaftaran manual tetap diperbolehkan
    EDialog_Register(DLG_INFO);
    EDialog_Register(DLG_INPUT);
    EDialog_Register(DLG_LIST);

    EDialog_SetGlobalHeader("=== Server Saya ===\n  %s\n================\n");

    return 1;
}

stock ShowInfo(playerid)
{
    EDialog_ShowSimple(playerid, DLG_INFO, "Info", "Selamat datang di server!", "OK", "Tutup");
    return 1;
}

stock PromptNickname(playerid)
{
    EDialog_ShowInput(playerid, DLG_INPUT, "Ganti Nick", "Masukkan nickname baru:", "", "Simpan", "Batal");
    return 1;
}

stock ShowTopPlayers(playerid)
{
    new rows[512];
    format(rows, sizeof rows,
        "1\tAlpha\t2000\n"
        "2\tBeta\t1800\n"
        "3\tGamma\t1500"
    );
    EDialog_ShowList(playerid, DLG_LIST, "Top Players", "#\tNama\tSkor", rows);
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
            format(buf, sizeof buf, "Nickname diperbarui: %s", clean);
            SendClientMessage(playerid, 0x00FF00FF, buf);
        } else {
            SendClientMessage(playerid, 0xFF0000FF, "Batal.");
        }
    }
    else if (dialogid == DLG_LIST) {
        if (response == 0) {
            new buf[64];
            format(buf, sizeof buf, "Anda memilih baris %d", listitem);
            SendClientMessage(playerid, 0xFFFFFF00, buf);
        }
    }

    new logmsg[128];
    format(logmsg, sizeof logmsg, "P%d D%d R%d", playerid, dialogid, response);
    EDialog_LogEvent(logmsg);

    return 1;
}
