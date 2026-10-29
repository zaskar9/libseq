/* ttsprobe: calls the exported functions of a 16-bit Windows DLL from a script and logs the results.
 *
 *   ttsprobe SCRIPT OUTPUT
 *
 * It observes a DLL through its interface only: it loads the DLL, looks functions up by their
 * exported name, calls them with the argument words the script gives, and writes what comes back.
 *
 * Script lines (one command each; '#' starts a comment):
 *   load NAME                 LoadLibrary(NAME); "load" again switches to another DLL
 *   buf N SIZE                allocate buffer N (0..15) of SIZE bytes, zeroed
 *   fill N OFFSET HEX...      write bytes into buffer N
 *   put N OFFSET "text"       write a zero-terminated string into buffer N
 *   call NAME ARG...          call NAME; the result (DX:AX) is printed and kept as $r
 *   call.w / call.d / call.p / call.v NAME ARG...
 *                             the same, printing the result as the function's type: a word (AX),
 *                             a dword (DX:AX), a far pointer (@N+OFF inside buffer N, null, or
 *                             "ptr"), or nothing (void); for comparable output
 *   dxs HEX                   AX and DX on entry to every call (default 0), to tell void, word and
 *                             dword results apart (a register that comes back unchanged was not set)
 *   cbptr K [L]               callbacks show words K,K+1 as a far pointer: the bytes it points to
 *                             (word L gives the count, else 16), so no address reaches the output
 *   let N                     keep the last result in variable N (0..9); argument %N pushes its low
 *                             word, %dN the whole dword (for handles the engine hands out)
 *   mode oracle|raw           oracle: leave out what differs between runs (handles, times,
 *                             repeat counts), so two runs give the same text; raw is the default
 *   dump N OFFSET LEN         hex dump of buffer N
 *   dumpr LEN / strr          hex dump / string at the far pointer $r (DX:AX) of the last call
 *   echo TEXT                 copy TEXT to the output
 *   sleep MS                  wait MS milliseconds, dispatching window messages meanwhile
 *   idle MS                   the same, calling SEQIDLE() in between (prints non-zero results)
 *   time                      print GetTickCount()
 *   cbret HEX                 what the callbacks cb0..cb8 return (default 1)
 *   cbyield 0|1               whether the callbacks yield (PeekMessage) before returning (default 0);
 *                             needed when the engine waits in a loop that calls the callback
 * Argument cbN (N = 0..8) is a far pointer to a callback taking N words (Pascal: it removes them);
 * ccN is the same with the C convention (the caller removes them). Each call is printed after the
 * command during which it happened ("callback cbN(words) at ms").
 * Arguments of call, in declaration order (as Wine's +snoop prints them):
 *   1a2b     one word (hex)
 *   d:HEX    a dword, pushed as high word then low word (like a far pointer or a LONG)
 *   @N       far pointer to buffer N (two words: segment, offset); @N+OFF adds an offset
 *   "text"   far pointer to a temporary copy of the string
 *   $r / $ax / $dx   the last result: dword, or one of its words
 *   null     a null far pointer (two zero words)
 * The caller's SP is restored after each call, so the callee's calling convention does not matter
 * for the stack; the argument count does matter for what the callee sees.
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXBUF 16
#define MAXARG 32

static BYTE far* g_buf[MAXBUF];
static unsigned g_len[MAXBUF];
static char g_str[8][256];
static HINSTANCE g_dll = 0;
static DWORD g_r = 0;
static FILE* g_out;
static int g_oracle = 0;
static DWORD g_var[10];
static WORD g_dxs = 0;

/* ---- callbacks: cbN takes N words (Pascal); each call is stored and printed by flushcb() ---- */
#define MAXCB 256
typedef struct { int id; int n; WORD w[8]; DWORD at; DWORD count; BYTE data[64]; int dlen; } CbRec;
static CbRec g_cb[MAXCB];
static int g_cbhead = 0, g_cbtail = 0, g_cblost = 0;
static DWORD g_cbret = 1;
static int g_cbyield = 0;
static int g_cbptr = -1, g_cblen = -1; /* "cbptr K L": words K,K+1 are a far pointer, word L its length */

static DWORD cbrecord(int id, int n, WORD w0, WORD w1, WORD w2, WORD w3, WORD w4, WORD w5, WORD w6, WORD w7) {
    int next = (g_cbhead + 1) % MAXCB;
    int last = (g_cbhead + MAXCB - 1) % MAXCB;
    if (g_cbyield) { MSG m; PeekMessage(&m, NULL, 0, 0, PM_NOREMOVE); } /* lets other tasks and timers run */
    /* repeated identical calls (a polling loop) are counted, not stored again */
    if (g_cbhead != g_cbtail && g_cb[last].id == id && g_cb[last].w[0] == w0 && g_cb[last].w[1] == w1 &&
        g_cb[last].w[2] == w2 && g_cb[last].w[3] == w3 && g_cb[last].w[4] == w4 && g_cb[last].w[5] == w5 &&
        g_cb[last].w[6] == w6 && g_cb[last].w[7] == w7) { ++g_cb[last].count; return g_cbret; }
    if (next == g_cbtail) { ++g_cblost; return g_cbret; }
    g_cb[g_cbhead].count = 1;
    g_cb[g_cbhead].dlen = -1;
    if (g_cbptr >= 0 && g_cbptr + 1 < n) {
        WORD wv[8];
        BYTE far* q;
        int k, len;
        wv[0] = w0; wv[1] = w1; wv[2] = w2; wv[3] = w3; wv[4] = w4; wv[5] = w5; wv[6] = w6; wv[7] = w7;
        q = (BYTE far*)MAKELONG(wv[g_cbptr + 1], wv[g_cbptr]);
        len = (g_cblen >= 0 && g_cblen < n) ? (int)wv[g_cblen] : 16;
        if (len > 64) len = 64;
        if (q == NULL) len = 0;
        for (k = 0; k < len; ++k) g_cb[g_cbhead].data[k] = q[k];
        g_cb[g_cbhead].dlen = q == NULL ? -2 : len;
    }
    g_cb[g_cbhead].id = id; g_cb[g_cbhead].n = n; g_cb[g_cbhead].at = GetCurrentTime();
    g_cb[g_cbhead].w[0] = w0; g_cb[g_cbhead].w[1] = w1; g_cb[g_cbhead].w[2] = w2; g_cb[g_cbhead].w[3] = w3;
    g_cb[g_cbhead].w[4] = w4; g_cb[g_cbhead].w[5] = w5; g_cb[g_cbhead].w[6] = w6; g_cb[g_cbhead].w[7] = w7;
    g_cbhead = next;
    return g_cbret;
}

#define CBATTR __far __pascal __export
DWORD CBATTR cb0(void) { return cbrecord(0, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
DWORD CBATTR cb1(WORD a) { return cbrecord(1, 1, a, 0, 0, 0, 0, 0, 0, 0); }
DWORD CBATTR cb2(WORD a, WORD b) { return cbrecord(2, 2, a, b, 0, 0, 0, 0, 0, 0); }
DWORD CBATTR cb3(WORD a, WORD b, WORD c) { return cbrecord(3, 3, a, b, c, 0, 0, 0, 0, 0); }
DWORD CBATTR cb4(WORD a, WORD b, WORD c, WORD d) { return cbrecord(4, 4, a, b, c, d, 0, 0, 0, 0); }
DWORD CBATTR cb5(WORD a, WORD b, WORD c, WORD d, WORD e) { return cbrecord(5, 5, a, b, c, d, e, 0, 0, 0); }
DWORD CBATTR cb6(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f) { return cbrecord(6, 6, a, b, c, d, e, f, 0, 0); }
DWORD CBATTR cb7(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f, WORD g) { return cbrecord(7, 7, a, b, c, d, e, f, g, 0); }
DWORD CBATTR cb8(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f, WORD g, WORD h) { return cbrecord(8, 8, a, b, c, d, e, f, g, h); }
/* the same with the C convention (the caller removes the arguments): ccN */
#define CCATTR __far __cdecl __export
DWORD CCATTR cc0(void) { return cbrecord(10, 0, 0, 0, 0, 0, 0, 0, 0, 0); }
DWORD CCATTR cc1(WORD a) { return cbrecord(11, 1, a, 0, 0, 0, 0, 0, 0, 0); }
DWORD CCATTR cc2(WORD a, WORD b) { return cbrecord(12, 2, a, b, 0, 0, 0, 0, 0, 0); }
DWORD CCATTR cc3(WORD a, WORD b, WORD c) { return cbrecord(13, 3, a, b, c, 0, 0, 0, 0, 0); }
DWORD CCATTR cc4(WORD a, WORD b, WORD c, WORD d) { return cbrecord(14, 4, a, b, c, d, 0, 0, 0, 0); }
DWORD CCATTR cc5(WORD a, WORD b, WORD c, WORD d, WORD e) { return cbrecord(15, 5, a, b, c, d, e, 0, 0, 0); }
DWORD CCATTR cc6(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f) { return cbrecord(16, 6, a, b, c, d, e, f, 0, 0); }
DWORD CCATTR cc7(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f, WORD g) { return cbrecord(17, 7, a, b, c, d, e, f, g, 0); }
DWORD CCATTR cc8(WORD a, WORD b, WORD c, WORD d, WORD e, WORD f, WORD g, WORD h) { return cbrecord(18, 8, a, b, c, d, e, f, g, h); }
static FARPROC g_cbfn[19];

/* Instance thunks, as Windows 3.1 programs make for callbacks: they load the program's DS. */
static void makecallbacks(HINSTANCE inst) {
    g_cbfn[0] = MakeProcInstance((FARPROC)cb0, inst); g_cbfn[1] = MakeProcInstance((FARPROC)cb1, inst);
    g_cbfn[2] = MakeProcInstance((FARPROC)cb2, inst); g_cbfn[3] = MakeProcInstance((FARPROC)cb3, inst);
    g_cbfn[4] = MakeProcInstance((FARPROC)cb4, inst); g_cbfn[5] = MakeProcInstance((FARPROC)cb5, inst);
    g_cbfn[6] = MakeProcInstance((FARPROC)cb6, inst); g_cbfn[7] = MakeProcInstance((FARPROC)cb7, inst);
    g_cbfn[8] = MakeProcInstance((FARPROC)cb8, inst);
    g_cbfn[10] = MakeProcInstance((FARPROC)cc0, inst); g_cbfn[11] = MakeProcInstance((FARPROC)cc1, inst);
    g_cbfn[12] = MakeProcInstance((FARPROC)cc2, inst); g_cbfn[13] = MakeProcInstance((FARPROC)cc3, inst);
    g_cbfn[14] = MakeProcInstance((FARPROC)cc4, inst); g_cbfn[15] = MakeProcInstance((FARPROC)cc5, inst);
    g_cbfn[16] = MakeProcInstance((FARPROC)cc6, inst); g_cbfn[17] = MakeProcInstance((FARPROC)cc7, inst);
    g_cbfn[18] = MakeProcInstance((FARPROC)cc8, inst);
}

static void flushcb(void) {
    while (g_cbtail != g_cbhead) {
        CbRec* r = &g_cb[g_cbtail];
        int i;
        if (r->id >= 10) fprintf(g_out, "callback cc%d(", r->id - 10); else fprintf(g_out, "callback cb%d(", r->id);
        for (i = 0; i < r->n; ++i) {
            if (r->dlen != -1 && i == g_cbptr) {
                int k;
                if (r->dlen == -2) fprintf(g_out, "%snull", i ? "," : "");
                else {
                    fprintf(g_out, "%s[", i ? "," : "");
                    for (k = 0; k < r->dlen; ++k) fprintf(g_out, "%s%02x", k ? " " : "", r->data[k]);
                    fprintf(g_out, "]");
                }
                ++i;
                continue;
            }
            fprintf(g_out, "%s%04x", i ? "," : "", r->w[i]);
        }
        if (g_oracle) fprintf(g_out, r->count > 1 ? "), repeated\n" : ")\n");
        else if (r->count > 1) fprintf(g_out, ") at %lu, %lu times\n", r->at, r->count);
        else fprintf(g_out, ") at %lu\n", r->at);
        g_cbtail = (g_cbtail + 1) % MAXCB;
    }
    if (g_cblost) { fprintf(g_out, "callback records lost: %d\n", g_cblost); g_cblost = 0; }
}

static DWORD callfar(FARPROC fn, int n, WORD far* w) {
    WORD rax = 0, rdx = 0, savesp = 0, dxs = g_dxs;
    _asm {
        push ds
        push si
        push di
        mov savesp, sp
        mov cx, n
        les bx, w
    next:
        jcxz done
        push word ptr es:[bx]
        add bx, 2
        dec cx
        jmp next
    done:
        mov dx, dxs
        mov ax, dxs
        call dword ptr fn
        mov sp, savesp
        pop di
        pop si
        pop ds
        mov rax, ax
        mov rdx, dx
    }
    return ((DWORD)rdx << 16) | rax;
}

static char* nexttok(char** p) {
    char* s = *p;
    char* t;
    while (*s == ' ' || *s == '\t') ++s;
    if (*s == 0 || *s == '#') { *p = s; return NULL; }
    if (*s == '"') {
        t = ++s;
        while (*s && *s != '"') ++s;
        if (*s) *s++ = 0;
        *p = s;
        return t - 1; /* keeps the opening quote as a marker */
    }
    t = s;
    while (*s && *s != ' ' && *s != '\t') ++s;
    if (*s) *s++ = 0;
    *p = s;
    return t;
}

static void hexdump(BYTE far* p, unsigned len) {
    unsigned i;
    for (i = 0; i < len; ++i) {
        fprintf(g_out, "%s%02x", (i % 16 == 0) ? (i ? "\n  " : "  ") : " ", p[i]);
    }
    fprintf(g_out, "\n");
}

static void run(char* line) {
    char* p = line;
    char* cmd = nexttok(&p);
    if (cmd == NULL) return;
    if (strcmp(cmd, "echo") == 0) {
        fprintf(g_out, "%s\n", p);
    } else if (strcmp(cmd, "sleep") == 0) {
        /* sleep MS: wait while dispatching messages (the engine's timer and window messages run) */
        DWORD until = GetTickCount() + strtoul(nexttok(&p), NULL, 0);
        MSG msg;
        while (GetTickCount() < until) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            flushcb();
            Yield();
        }
    } else if (strcmp(cmd, "idle") == 0) {
        /* idle MS: like sleep, but calls SEQIDLE() between messages, as Cakewalk's message loop
         * does; prints each non-zero result with the time since the command started */
        DWORD start = GetTickCount();
        DWORD until = start + strtoul(nexttok(&p), NULL, 0);
        FARPROC idle = g_dll ? GetProcAddress(g_dll, "SEQIDLE") : NULL;
        MSG msg;
        while (GetTickCount() < until) {
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            if (idle) {
                DWORD r = callfar(idle, 0, (WORD far*)&r);
                if (LOWORD(r) && g_oracle) fprintf(g_out, "SEQIDLE() -> %04x\n", LOWORD(r));
                else if (LOWORD(r)) fprintf(g_out, "SEQIDLE() -> %04x:%04x at +%lu ms\n", HIWORD(r), LOWORD(r), GetTickCount() - start);
            }
            flushcb();
            Yield();
        }
    } else if (strcmp(cmd, "dxs") == 0) {
        /* dxs HEX: the value DX holds when a function is called (default 0); a DX that comes back
         * unchanged suggests a function that returns only a word */
        g_dxs = (WORD)strtoul(nexttok(&p), NULL, 16);
    } else if (strcmp(cmd, "cbptr") == 0) {
        /* cbptr K [L]: callbacks print words K,K+1 (segment, offset) as the bytes they point to,
         * word L giving the length (16 without L); cbptr -1 turns it off */
        char* l;
        g_cbptr = atoi(nexttok(&p));
        l = nexttok(&p);
        g_cblen = l ? atoi(l) : -1;
    } else if (strcmp(cmd, "let") == 0) {
        g_var[atoi(nexttok(&p)) % 10] = g_r;
    } else if (strcmp(cmd, "mode") == 0) {
        g_oracle = strcmp(nexttok(&p), "oracle") == 0;
    } else if (strcmp(cmd, "cbyield") == 0) {
        /* cbyield 0|1: whether the callbacks call PeekMessage (yield) before returning */
        g_cbyield = atoi(nexttok(&p));
    } else if (strcmp(cmd, "cbret") == 0) {
        /* cbret HEX: what the callbacks return from now on (default 1) */
        g_cbret = strtoul(nexttok(&p), NULL, 16);
    } else if (strcmp(cmd, "time") == 0) {
        if (!g_oracle) fprintf(g_out, "time %lu\n", GetTickCount());
    } else if (strcmp(cmd, "load") == 0) {
        char* name = nexttok(&p);
        g_dll = LoadLibrary(name);
        if (g_oracle) fprintf(g_out, "load %s -> %s\n", name, (UINT)g_dll < 32 ? "failed" : "ok");
        else fprintf(g_out, "load %s -> %04x\n", name, (unsigned)g_dll);
        if ((UINT)g_dll < 32) g_dll = 0;
    } else if (strcmp(cmd, "buf") == 0) {
        int n = atoi(nexttok(&p));
        unsigned size = (unsigned)strtoul(nexttok(&p), NULL, 0);
        HGLOBAL h = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, size);
        g_buf[n] = (BYTE far*)GlobalLock(h);
        g_len[n] = size;
    } else if (strcmp(cmd, "fill") == 0) {
        int n = atoi(nexttok(&p));
        unsigned off = (unsigned)strtoul(nexttok(&p), NULL, 0);
        char* t;
        while ((t = nexttok(&p)) != NULL) g_buf[n][off++] = (BYTE)strtoul(t, NULL, 16);
    } else if (strcmp(cmd, "put") == 0) {
        int n = atoi(nexttok(&p));
        unsigned off = (unsigned)strtoul(nexttok(&p), NULL, 0);
        char* t = nexttok(&p);
        _fstrcpy((char far*)(g_buf[n] + off), t + 1);
    } else if (strcmp(cmd, "dump") == 0) {
        int n = atoi(nexttok(&p));
        unsigned off = (unsigned)strtoul(nexttok(&p), NULL, 0);
        unsigned len = (unsigned)strtoul(nexttok(&p), NULL, 0);
        fprintf(g_out, "dump %d+%u:\n", n, off);
        hexdump(g_buf[n] + off, len);
    } else if (strcmp(cmd, "dumpr") == 0) {
        unsigned len = (unsigned)strtoul(nexttok(&p), NULL, 0);
        BYTE far* q = (BYTE far*)g_r;
        if (g_oracle) fprintf(g_out, "dump $r:\n"); else fprintf(g_out, "dump %04x:%04x:\n", HIWORD(g_r), LOWORD(g_r));
        if (q) hexdump(q, len);
    } else if (strcmp(cmd, "strr") == 0) {
        char far* q = (char far*)g_r;
        if (g_oracle) fprintf(g_out, "str $r = \"%s\"\n", q ? q : (char far*)"");
        else fprintf(g_out, "str %04x:%04x = \"%s\"\n", HIWORD(g_r), LOWORD(g_r), q ? q : (char far*)"");
    } else if (strncmp(cmd, "call", 4) == 0 && (cmd[4] == 0 || cmd[4] == '.')) {
        WORD w[MAXARG];
        int n = 0;
        char type = cmd[4] ? cmd[5] : 'r';
        char* name = nexttok(&p);
        char* t;
        FARPROC fn = g_dll ? GetProcAddress(g_dll, name) : NULL;
        fprintf(g_out, "%s(", name);
        while ((t = nexttok(&p)) != NULL && n < MAXARG - 1) {
            fprintf(g_out, "%s%s%s", n ? "," : "", t, t[0] == '"' ? "\"" : "");
            if (t[0] == '"') {
                /* each string gets its own copy that stays alive: the engine may keep the pointer
                 * (SeqRegisterEventKind does) */
                char far* q = (char far*)GlobalLock(GlobalAlloc(GMEM_FIXED, strlen(t + 1) + 1));
                _fstrcpy(q, (char far*)(t + 1));
                w[n++] = HIWORD((DWORD)q); w[n++] = LOWORD((DWORD)q);
            } else if (t[0] == '@') {
                int b = atoi(t + 1);
                char* plus = strchr(t, '+');
                BYTE far* q = g_buf[b] + (plus ? (unsigned)strtoul(plus + 1, NULL, 0) : 0);
                w[n++] = HIWORD((DWORD)q); w[n++] = LOWORD((DWORD)q);
            } else if (strncmp(t, "d:", 2) == 0) {
                DWORD v = strtoul(t + 2, NULL, 16);
                w[n++] = HIWORD(v); w[n++] = LOWORD(v);
            } else if (t[0] == 'c' && (t[1] == 'b' || t[1] == 'c') && t[2] >= '0' && t[2] <= '8' && t[3] == 0) {
                DWORD q = (DWORD)g_cbfn[t[2] - '0' + (t[1] == 'c' ? 10 : 0)];
                w[n++] = HIWORD(q); w[n++] = LOWORD(q);
            } else if (t[0] == '%' && t[1] == 'd') {
                DWORD v = g_var[atoi(t + 2) % 10];
                w[n++] = HIWORD(v); w[n++] = LOWORD(v);
            } else if (t[0] == '%') {
                w[n++] = LOWORD(g_var[atoi(t + 1) % 10]);
            } else if (strcmp(t, "null") == 0) {
                w[n++] = 0; w[n++] = 0;
            } else if (strcmp(t, "$r") == 0) {
                w[n++] = HIWORD(g_r); w[n++] = LOWORD(g_r);
            } else if (strcmp(t, "$ax") == 0) {
                w[n++] = LOWORD(g_r);
            } else if (strcmp(t, "$dx") == 0) {
                w[n++] = HIWORD(g_r);
            } else {
                w[n++] = (WORD)strtoul(t, NULL, 16);
            }
        }
        if (fn == NULL) {
            fprintf(g_out, ") -> not found\n");
            return;
        }
        g_r = callfar(fn, n, (WORD far*)w);
        if (type == 'w') fprintf(g_out, ") -> %04x\n", LOWORD(g_r));
        else if (type == 'd') fprintf(g_out, ") -> %08lx\n", g_r);
        else if (type == 'v') fprintf(g_out, ")\n");
        else if (type == 'p') {
            int b, found = 0;
            if (g_r == 0) fprintf(g_out, ") -> null\n");
            else {
                for (b = 0; b < MAXBUF && !found; ++b)
                    if (g_buf[b] && HIWORD(g_r) == HIWORD((DWORD)g_buf[b]) && LOWORD(g_r) >= LOWORD((DWORD)g_buf[b]) &&
                        LOWORD(g_r) - LOWORD((DWORD)g_buf[b]) <= g_len[b]) {
                        fprintf(g_out, ") -> @%d+%u\n", b, LOWORD(g_r) - LOWORD((DWORD)g_buf[b]));
                        found = 1;
                    }
                if (!found) fprintf(g_out, ") -> ptr\n");
            }
        } else fprintf(g_out, ") -> %04x:%04x\n", HIWORD(g_r), LOWORD(g_r));
        flushcb();
    } else {
        fprintf(g_out, "? %s\n", cmd);
    }
    fflush(g_out);
}

int PASCAL WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
    char args[256], line[512];
    char *script, *output, *p;
    FILE* in;
    makecallbacks(inst);
    _fstrcpy(args, cmdline);
    p = args;
    script = nexttok(&p);
    output = nexttok(&p);
    if (script == NULL || output == NULL) return 1;
    in = fopen(script, "r");
    g_out = fopen(output, "w");
    if (in == NULL || g_out == NULL) return 2;
    while (fgets(line, sizeof line, in)) {
        line[strcspn(line, "\r\n")] = 0;
        run(line);
    }
    fclose(in);
    fclose(g_out);
    if (g_dll) FreeLibrary(g_dll);
    return 0;
}
