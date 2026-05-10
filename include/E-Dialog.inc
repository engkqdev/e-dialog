/*
 * E-Dialog.inc v0.4 (stabil)
 * Path: include/E-Dialog.inc
 *
 * Tujuan v0.4:
 * - Mempermudah operasi umum dengan fungsi pembungkus sederhana
 * - Tetap kompatibel dengan v0.3
 * - Menyediakan helper yang jelas untuk dialog message/input/list
 * - Source mudah dibaca (spasi, tanpa tab)
 *
 * Versi: 0.4
 * Tanggal: 2026-05-10
 * Penulis: engkq
 */

#define EDLG_MAX_REGS       256
#define EDLG_MAX_BUFFER     2048
#define EDLG_HDR_MAXLEN     256
#define EDLG_TITLE_LINE     "========================"
#define EDLG_VERSION        "0.4"

#define edialog_header EDialog_FormatHeader

new g_EDialog_Regs[EDLG_MAX_REGS];
new g_EDialog_RegCount = 0;
new g_EDialog_HeaderTemplate[EDLG_MAX_REGS][EDLG_HDR_MAXLEN];
new g_EDialog_GlobalHeader[EDLG_HDR_MAXLEN];
new g_EDialog_AutoRegister = 1; // default: on (lebih mudah digunakan)

// Antrian per-pemain
#define EDLG_MAX_PLAYERS    5000
#define EDLG_MAX_QUEUE      12
#define EDLG_MAX_TITLE      128
#define EDLG_MAX_INFO       1024
#define EDLG_MAX_BTN        32

new g_EDialog_QueueCount[EDLG_MAX_PLAYERS];
new g_EDialog_Queue_DialogId[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE];
new g_EDialog_Queue_Style[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE];
new g_EDialog_Queue_Title[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE][EDLG_MAX_TITLE];
new g_EDialog_Queue_Info[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE][EDLG_MAX_INFO];
new g_EDialog_Queue_Btn1[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE][EDLG_MAX_BTN];
new g_EDialog_Queue_Btn2[EDLG_MAX_PLAYERS][EDLG_MAX_QUEUE][EDLG_MAX_BTN];

// Penghasil ID unik
new g_EDialog_IdCounter = 1000;

// Logger sederhana (in-memory ring buffer)
#define EDLG_LOG_SIZE 128
new g_EDialog_LogIndex = 0;
new g_EDialog_LogMessages[EDLG_LOG_SIZE][128];

// forward yang harus diimplementasikan gamemode untuk menerima event dialog
forward EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext[]);

// ---------------- Utilitas ----------------
stock _EDialog_FindIndexById(dialogid)
{
    new i;
    for (i = 0; i < g_EDialog_RegCount; i++) {
        if (g_EDialog_Regs[i] == dialogid) return i;
    }
    return -1;
}

// ---------------- Registrasi ----------------
stock EDialog_Register(dialogid)
{
    if (_EDialog_FindIndexById(dialogid) != -1) return 0;
    if (g_EDialog_RegCount >= EDLG_MAX_REGS) return 0;
    g_EDialog_Regs[g_EDialog_RegCount] = dialogid;
    g_EDialog_HeaderTemplate[g_EDialog_RegCount][0] = '\0';
    g_EDialog_RegCount++;
    return 1;
}

stock EDialog_Unregister(dialogid)
{
    new idx = _EDialog_FindIndexById(dialogid);
    if (idx == -1) return 0;
    g_EDialog_RegCount--;
    g_EDialog_Regs[idx] = g_EDialog_Regs[g_EDialog_RegCount];
    strcpy(g_EDialog_HeaderTemplate[idx], g_EDialog_HeaderTemplate[g_EDialog_RegCount], sizeof g_EDialog_HeaderTemplate[idx]);
    return 1;
}

stock EDialog_IsRegistered(dialogid)
{
    return (_EDialog_FindIndexById(dialogid) != -1);
}

stock EDialog_SetAutoRegister(state)
{
    g_EDialog_AutoRegister = state ? 1 : 0;
    return 1;
}

// ---------------- Manajemen header ----------------
stock EDialog_SetGlobalHeader(template[])
{
    strmid(g_EDialog_GlobalHeader, template, 0, sizeof g_EDialog_GlobalHeader);
    return 1;
}

stock EDialog_SetDialogHeader(dialogid, template[])
{
    new idx = _EDialog_FindIndexById(dialogid);
    if (idx == -1) return 0;
    strmid(g_EDialog_HeaderTemplate[idx], template, 0, sizeof g_EDialog_HeaderTemplate[idx]);
    return 1;
}

stock EDialog_ClearDialogHeader(dialogid)
{
    new idx = _EDialog_FindIndexById(dialogid);
    if (idx == -1) return 0;
    g_EDialog_HeaderTemplate[idx][0] = '\0';
    return 1;
}

stock EDialog_FormatHeader(out[], outlen, title[], dialogid)
{
    new tmp[EDLG_MAX_BUFFER];
    new hdr[EDLG_HDR_MAXLEN];

    if (dialogid != -1) {
        new idx = _EDialog_FindIndexById(dialogid);
        if (idx != -1 && g_EDialog_HeaderTemplate[idx][0] != '\0') {
            strmid(hdr, g_EDialog_HeaderTemplate[idx], 0, sizeof hdr);
            format(tmp, sizeof tmp, hdr, title);
            strmid(out, tmp, 0, outlen);
            return 1;
        }
    }

    if (g_EDialog_GlobalHeader[0] != '\0') {
        strmid(hdr, g_EDialog_GlobalHeader, 0, sizeof hdr);
        format(tmp, sizeof tmp, hdr, title);
        strmid(out, tmp, 0, outlen);
        return 1;
    }

    format(tmp, sizeof tmp, "%s\n  %s\n%s", EDLG_TITLE_LINE, title, EDLG_TITLE_LINE);
    strmid(out, tmp, 0, outlen);
    return 1;
}

// ---------------- Fungsi utama tampilan ----------------
public EDialog_Show(playerid, dialogid, style, title[], info[], button1[], button2[])
{
    if (!EDialog_IsRegistered(dialogid) && g_EDialog_AutoRegister) {
        EDialog_Register(dialogid);
    }

    new caption[EDLG_HDR_MAXLEN];
    EDialog_FormatHeader(caption, sizeof caption, title, dialogid);
    ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
    return 1;
}

// ---------------- Helper sederhana (v0.4) ----------------
// Kotak pesan cepat (OK / Cancel)
stock EDialog_ShowSimple(playerid, dialogid, title[], message[], okButton[] = "OK", cancelButton[] = "Cancel")
{
    return EDialog_Show(playerid, dialogid, 0, title, message, okButton, cancelButton);
}

// Prompt input cepat: defaultText opsional
stock EDialog_ShowInput(playerid, dialogid, title[], prompt[], defaultText[], saveButton[] = "Save", cancelButton[] = "Cancel")
{
    new info[EDLG_MAX_INFO];
    if (defaultText[0] != '\0') format(info, sizeof info, "%s\n\n%s", prompt, defaultText);
    else strmid(info, prompt, 0, sizeof info);
    return EDialog_Show(playerid, dialogid, 2, title, info, saveButton, cancelButton);
}

// Tampilkan list cepat (columnHeader & rows menggunakan '\t' dan '\n')
stock EDialog_ShowList(playerid, dialogid, title[], columnHeader[], rows[])
{
    new info[EDLG_MAX_INFO];
    EDialog_MakeListInfo(info, sizeof info, columnHeader, rows);
    return EDialog_Show(playerid, dialogid, 1, title, info, "Close", "More");
}

// Tampilkan atau antri jika pemain sibuk
stock EDialog_ShowOrQueue(playerid, dialogid, style, title[], info[], button1[], button2[])
{
    if (EDialog_GetQueuedCount(playerid) == 0) {
        return EDialog_Show(playerid, dialogid, style, title, info, button1, button2);
    }
    return EDialog_QueueForPlayer(playerid, dialogid, style, title, info, button1, button2);
}

// ---------------- Helper lain ----------------
stock EDialog_MakeListInfo(out[], outlen, columnHeader[], rows[])
{
    new tmp[EDLG_MAX_BUFFER];
    format(tmp, sizeof tmp, "%s\n\n%s", columnHeader, rows);
    strmid(out, tmp, 0, outlen);
    return 1;
}

stock EDialog_GenerateId(prefix[])
{
    return g_EDialog_IdCounter++;
}

stock EDialog_SanitizeInput(input[], out[], outlen)
{
    new i, j = 0, c;
    for (i = 0; input[i] != '\0' && j < outlen - 1; i++) {
        c = input[i];
        if (c >= 32 && c != 127) {
            out[j++] = c;
        }
    }
    out[j] = '\0';
    return j;
}

stock EDialog_FormatColumns(out[], outlen, row[], widths[], colcount)
{
    new buf[EDLG_MAX_BUFFER];
    buf[0] = '\0';
    new col[256];
    new i = 0, w, k, len, rpos = 0;

    while (i < colcount) {
        k = 0;
        while (row[rpos] != '\0' && row[rpos] != '\t' && k < sizeof col - 1) {
            col[k++] = row[rpos++];
        }
        col[k] = '\0';
        if (row[rpos] == '\t') rpos++;
        w = widths[i];
        len = strlen(col);
        if (len >= w) {
            col[w] = '\0';
        } else {
            while (len < w && len < sizeof col - 1) col[len++] = ' ';
            col[len] = '\0';
        }
        strcat(buf, col);
        i++;
    }
    strmid(out, buf, 0, outlen);
    return 1;
}

stock EDialog_AutoPadColumns(out[], outlen, row[], maxWidth)
{
    new buf[EDLG_MAX_BUFFER];
    buf[0] = '\0';
    new tmp[256];
    new rpos = 0, k, len;

    while (row[rpos] != '\0') {
        k = 0;
        while (row[rpos] != '\0' && row[rpos] != '\t' && k < sizeof tmp - 1) tmp[k++] = row[rpos++];
        tmp[k] = '\0';
        if (row[rpos] == '\t') rpos++;
        len = strlen(tmp);
        strcat(buf, tmp);
        while (len < maxWidth) { strcat(buf, " "); len++; }
    }
    strmid(out, buf, 0, outlen);
    return 1;
}

stock EDialog_LogEvent(message[])
{
    strmid(g_EDialog_LogMessages[g_EDialog_LogIndex], message, 0, sizeof g_EDialog_LogMessages[0]);
    g_EDialog_LogIndex = (g_EDialog_LogIndex + 1) % EDLG_LOG_SIZE;
    return 1;
}

// ---------------- Queueing ----------------
stock EDialog_QueueForPlayer(playerid, dialogid, style, title[], info[], button1[], button2[])
{
    if (playerid < 0 || playerid >= EDLG_MAX_PLAYERS) return 0;
    new count = g_EDialog_QueueCount[playerid];
    if (count >= EDLG_MAX_QUEUE) return 0;

    g_EDialog_Queue_DialogId[playerid][count] = dialogid;
    g_EDialog_Queue_Style[playerid][count] = style;
    strmid(g_EDialog_Queue_Title[playerid][count], title, 0, EDLG_MAX_TITLE);
    strmid(g_EDialog_Queue_Info[playerid][count], info, 0, EDLG_MAX_INFO);
    strmid(g_EDialog_Queue_Btn1[playerid][count], button1, 0, EDLG_MAX_BTN);
    strmid(g_EDialog_Queue_Btn2[playerid][count], button2, 0, EDLG_MAX_BTN);
    g_EDialog_QueueCount[playerid] = count + 1;
    return 1;
}

stock EDialog_ShowNextQueued(playerid)
{
    if (playerid < 0 || playerid >= EDLG_MAX_PLAYERS) return 0;
    new count = g_EDialog_QueueCount[playerid];
    if (count <= 0) return 0;

    new dialogid = g_EDialog_Queue_DialogId[playerid][0];
    new style = g_EDialog_Queue_Style[playerid][0];
    new title[EDLG_MAX_TITLE];
    new info[EDLG_MAX_INFO];
    new btn1[EDLG_MAX_BTN];
    new btn2[EDLG_MAX_BTN];

    strmid(title, g_EDialog_Queue_Title[playerid][0], 0, sizeof title);
    strmid(info, g_EDialog_Queue_Info[playerid][0], 0, sizeof info);
    strmid(btn1, g_EDialog_Queue_Btn1[playerid][0], 0, sizeof btn1);
    strmid(btn2, g_EDialog_Queue_Btn2[playerid][0], 0, sizeof btn2);

    EDialog_Show(playerid, dialogid, style, title, info, btn1, btn2);

    new i;
    for (i = 1; i < count; i++) {
        g_EDialog_Queue_DialogId[playerid][i-1] = g_EDialog_Queue_DialogId[playerid][i];
        g_EDialog_Queue_Style[playerid][i-1] = g_EDialog_Queue_Style[playerid][i];
        strcpy(g_EDialog_Queue_Title[playerid][i-1], g_EDialog_Queue_Title[playerid][i], sizeof g_EDialog_Queue_Title[playerid][i-1]);
        strcpy(g_EDialog_Queue_Info[playerid][i-1], g_EDialog_Queue_Info[playerid][i], sizeof g_EDialog_Queue_Info[playerid][i-1]);
        strcpy(g_EDialog_Queue_Btn1[playerid][i-1], g_EDialog_Queue_Btn1[playerid][i], sizeof g_EDialog_Queue_Btn1[playerid][i-1]);
        strcpy(g_EDialog_Queue_Btn2[playerid][i-1], g_EDialog_Queue_Btn2[playerid][i], sizeof g_EDialog_Queue_Btn2[playerid][i-1]);
    }

    g_EDialog_Queue_Title[playerid][count-1][0] = '\0';
    g_EDialog_Queue_Info[playerid][count-1][0] = '\0';
    g_EDialog_Queue_Btn1[playerid][count-1][0] = '\0';
    g_EDialog_Queue_Btn2[playerid][count-1][0] = '\0';
    g_EDialog_QueueCount[playerid] = count - 1;
    return 1;
}

stock EDialog_ClearPlayerQueue(playerid)
{
    if (playerid < 0 || playerid >= EDLG_MAX_PLAYERS) return 0;
    g_EDialog_QueueCount[playerid] = 0;
    return 1;
}

stock EDialog_GetQueuedCount(playerid)
{
    if (playerid < 0 || playerid >= EDLG_MAX_PLAYERS) return 0;
    return g_EDialog_QueueCount[playerid];
}

// ---------------- Intercept dan forward response ----------------
public OnPlayerDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (EDialog_IsRegistered(dialogid)) {
        EDialog_OnResponse(playerid, dialogid, response, listitem, inputtext);
        if (EDialog_GetQueuedCount(playerid) > 0) EDialog_ShowNextQueued(playerid);
        return 1;
    }
    return 0;
}
