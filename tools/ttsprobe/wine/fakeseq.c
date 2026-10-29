/* fakeseq: an LD_PRELOAD replacement for libasound's sequencer API, enough for Wine's winealsa
 * MIDI driver to see one output port, "Fake - MIDI Out", and one input port, "Fake - MIDI In".
 * Every event sent to the output goes to a log file with a timestamp. No kernel ALSA needed.
 * Build: gcc -m32 -shared -fPIC -O2 -pthread.
 * Log: $FAKESEQ_LOG (default /tmp/midilog.txt), one line per event:
 *   <ms since the sequencer was opened> <kind> <bytes...>
 * Input: $FAKESEQ_INPUT names a script of lines "<ms> <hex bytes...>" (a channel message or a
 * complete F0...F7 sysex; '#' starts a comment). When Wine connects to the input port, a thread
 * delivers each message <ms> after the connection and logs it as "in <bytes>" on the same clock.
 * With $FAKESEQ_INPUT_GO set to a file name, the times count from when that file appears instead
 * (touch it to start the script; logged as "go").
 */
#define _GNU_SOURCE
#include <errno.h>
#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* --- ALSA sequencer types, as libasound lays them out on i386 --- */
typedef struct { unsigned char client, port; } snd_seq_addr_t;
typedef struct { unsigned int tick; } snd_seq_tick_time_t;
typedef struct { unsigned int tv_sec, tv_nsec; } snd_seq_real_time_t;
typedef union { snd_seq_tick_time_t tick; snd_seq_real_time_t time; } snd_seq_timestamp_t;
typedef struct { unsigned char channel, note, velocity, off_velocity; unsigned int duration; } snd_seq_ev_note_t;
typedef struct { unsigned char channel, unused[3]; unsigned int param; signed int value; } snd_seq_ev_ctrl_t;
typedef struct { unsigned int len; void* ptr; } snd_seq_ev_ext_t;
typedef struct {
    unsigned char type, flags, tag, queue;
    snd_seq_timestamp_t time;
    snd_seq_addr_t source, dest;
    union {
        snd_seq_ev_note_t note;
        snd_seq_ev_ctrl_t control;
        unsigned char raw8[12];
        snd_seq_ev_ext_t ext;
    } data;
} snd_seq_event_t;

enum {
    SND_SEQ_EVENT_NOTEON = 6, SND_SEQ_EVENT_NOTEOFF = 7, SND_SEQ_EVENT_KEYPRESS = 8,
    SND_SEQ_EVENT_CONTROLLER = 10, SND_SEQ_EVENT_PGMCHANGE = 11, SND_SEQ_EVENT_CHANPRESS = 12,
    SND_SEQ_EVENT_PITCHBEND = 13, SND_SEQ_EVENT_CONTROL14 = 14, SND_SEQ_EVENT_NONREGPARAM = 15,
    SND_SEQ_EVENT_REGPARAM = 16, SND_SEQ_EVENT_SYSEX = 130
};
#define SND_SEQ_PORT_CAP_READ (1 << 0)
#define SND_SEQ_PORT_CAP_WRITE (1 << 1)
#define SND_SEQ_PORT_CAP_SUBS_READ (1 << 5)
#define SND_SEQ_PORT_CAP_SUBS_WRITE (1 << 6)
#define SND_SEQ_PORT_TYPE_MIDI_GENERIC (1 << 1)
#define SND_SEQ_PORT_TYPE_HARDWARE (1 << 16)

typedef struct { int fd[2]; int next_port; } fake_seq_t;
typedef struct { int client; int type; char name[64]; } fake_client_info_t;
typedef struct { int client; int port; char name[64]; unsigned int capability; unsigned int type; } fake_port_info_t;
typedef struct { unsigned char buf[8]; int len; int expected; } fake_midi_event_t;

#define FAKE_CLIENT 20
#define FAKE_PORT 0
#define FAKE_IN_PORT 1

static FILE* g_log;
static struct timespec g_t0;
static int g_have_t0;

static void logline(const char* kind, const unsigned char* bytes, const unsigned int n) {
    struct timespec now;
    if (!g_log) {
        const char* path = getenv("FAKESEQ_LOG");
        g_log = fopen(path ? path : "/tmp/midilog.txt", "a");
        if (!g_log) return;
    }
    clock_gettime(CLOCK_MONOTONIC, &now);
    if (!g_have_t0) { g_t0 = now; g_have_t0 = 1; }
    const double ms = (double)(now.tv_sec - g_t0.tv_sec) * 1000.0 + (double)(now.tv_nsec - g_t0.tv_nsec) / 1e6;
    fprintf(g_log, "%12.3f %s", ms, kind);
    for (unsigned int i = 0; i < n; ++i) fprintf(g_log, " %02x", bytes[i]);
    fputc('\n', g_log);
    fflush(g_log);
}

int snd_seq_open(fake_seq_t** handle, const char* name, int streams, int mode) {
    (void)name; (void)streams; (void)mode;
    fake_seq_t* s = calloc(1, sizeof *s);
    if (!s || pipe(s->fd) != 0) { free(s); return -1; }
    s->next_port = 0;
    *handle = s;
    logline("open", NULL, 0);
    return 0;
}
int snd_seq_close(fake_seq_t* s) { if (s) { close(s->fd[0]); close(s->fd[1]); free(s); } logline("close", NULL, 0); return 0; }
int snd_seq_client_id(fake_seq_t* s) { (void)s; return 128; }
int snd_seq_set_client_name(fake_seq_t* s, const char* name) { (void)s; (void)name; return 0; }
int snd_seq_create_simple_port(fake_seq_t* s, const char* name, unsigned int caps, unsigned int type) {
    (void)name; (void)caps; (void)type; return s->next_port++;
}
int snd_seq_delete_simple_port(fake_seq_t* s, int port) { (void)s; (void)port; return 0; }
int snd_seq_connect_to(fake_seq_t* s, int my_port, int dest_client, int dest_port) {
    (void)s; (void)my_port; (void)dest_client; (void)dest_port; logline("connect", NULL, 0); return 0;
}
static void start_input(fake_seq_t* s);
int snd_seq_connect_from(fake_seq_t* s, int my_port, int src_client, int src_port) {
    (void)my_port;
    logline("connect-in", NULL, 0);
    if (src_client == FAKE_CLIENT && src_port == FAKE_IN_PORT) start_input(s);
    return 0;
}
int snd_seq_disconnect_from(fake_seq_t* s, int my_port, int src_client, int src_port) {
    (void)s; (void)my_port; (void)src_client; (void)src_port; return 0;
}
int snd_seq_disconnect_to(fake_seq_t* s, int my_port, int dest_client, int dest_port) {
    (void)s; (void)my_port; (void)dest_client; (void)dest_port; return 0;
}

/* enumeration: one client with one output port */
size_t snd_seq_client_info_sizeof(void) { return sizeof(fake_client_info_t); }
size_t snd_seq_port_info_sizeof(void) { return sizeof(fake_port_info_t); }
void snd_seq_client_info_set_client(fake_client_info_t* c, int client) { c->client = client; }
int snd_seq_client_info_get_client(const fake_client_info_t* c) { return c->client; }
int snd_seq_client_info_get_type(const fake_client_info_t* c) { (void)c; return 2; /* SND_SEQ_KERNEL_CLIENT */ }
const char* snd_seq_client_info_get_name(fake_client_info_t* c) { return c->name; }
int snd_seq_query_next_client(fake_seq_t* s, fake_client_info_t* c) {
    (void)s;
    if (c->client < FAKE_CLIENT) { c->client = FAKE_CLIENT; strcpy(c->name, "Fake"); return 0; }
    return -1;
}
void snd_seq_port_info_set_client(fake_port_info_t* p, int client) { p->client = client; }
void snd_seq_port_info_set_port(fake_port_info_t* p, int port) { p->port = port; }
int snd_seq_port_info_get_port(const fake_port_info_t* p) { return p->port; }
const char* snd_seq_port_info_get_name(const fake_port_info_t* p) { return p->name; }
unsigned int snd_seq_port_info_get_capability(const fake_port_info_t* p) { return p->capability; }
unsigned int snd_seq_port_info_get_type(const fake_port_info_t* p) { return p->type; }
const snd_seq_addr_t* snd_seq_port_info_get_addr(const fake_port_info_t* p) {
    static snd_seq_addr_t a; a.client = (unsigned char)p->client; a.port = (unsigned char)p->port; return &a;
}
int snd_seq_query_next_port(fake_seq_t* s, fake_port_info_t* p) {
    (void)s;
    if (p->client != FAKE_CLIENT) return -1;
    if (p->port < FAKE_PORT) {
        p->port = FAKE_PORT;
        strcpy(p->name, "MIDI Out");
        p->capability = SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE;
        p->type = SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_HARDWARE;
        return 0;
    }
    if (p->port < FAKE_IN_PORT) {
        p->port = FAKE_IN_PORT;
        strcpy(p->name, "MIDI In");
        p->capability = SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ;
        p->type = SND_SEQ_PORT_TYPE_MIDI_GENERIC | SND_SEQ_PORT_TYPE_HARDWARE;
        return 0;
    }
    return -1;
}

/* output */
int snd_seq_event_output_direct(fake_seq_t* s, snd_seq_event_t* ev) {
    (void)s;
    unsigned char b[3];
    switch (ev->type) {
    case SND_SEQ_EVENT_NOTEON: b[0] = (unsigned char)(0x90 | (ev->data.note.channel & 15)); b[1] = ev->data.note.note; b[2] = ev->data.note.velocity; logline("short", b, 3); break;
    case SND_SEQ_EVENT_NOTEOFF: b[0] = (unsigned char)(0x80 | (ev->data.note.channel & 15)); b[1] = ev->data.note.note; b[2] = ev->data.note.velocity; logline("short", b, 3); break;
    case SND_SEQ_EVENT_KEYPRESS: b[0] = (unsigned char)(0xa0 | (ev->data.note.channel & 15)); b[1] = ev->data.note.note; b[2] = ev->data.note.velocity; logline("short", b, 3); break;
    case SND_SEQ_EVENT_CONTROLLER: b[0] = (unsigned char)(0xb0 | (ev->data.control.channel & 15)); b[1] = (unsigned char)ev->data.control.param; b[2] = (unsigned char)ev->data.control.value; logline("short", b, 3); break;
    case SND_SEQ_EVENT_PGMCHANGE: b[0] = (unsigned char)(0xc0 | (ev->data.control.channel & 15)); b[1] = (unsigned char)ev->data.control.value; logline("short", b, 2); break;
    case SND_SEQ_EVENT_CHANPRESS: b[0] = (unsigned char)(0xd0 | (ev->data.control.channel & 15)); b[1] = (unsigned char)ev->data.control.value; logline("short", b, 2); break;
    case SND_SEQ_EVENT_PITCHBEND: {
        const int v = ev->data.control.value + 8192;
        b[0] = (unsigned char)(0xe0 | (ev->data.control.channel & 15)); b[1] = (unsigned char)(v & 0x7f); b[2] = (unsigned char)((v >> 7) & 0x7f); logline("short", b, 3); break;
    }
    case SND_SEQ_EVENT_SYSEX: logline("sysex", ev->data.ext.ptr, ev->data.ext.len); break;
    case 90: logline("short", ev->data.raw8, 1); break; /* a one-byte system message, see encode_byte */
    default: { unsigned char t = ev->type; logline("event", &t, 1); break; }
    }
    return 0;
}
int snd_seq_event_output(fake_seq_t* s, snd_seq_event_t* ev) { return snd_seq_event_output_direct(s, ev); }
int snd_seq_drain_output(fake_seq_t* s) { (void)s; return 0; }
int snd_seq_free_event(snd_seq_event_t* ev) { (void)ev; return 0; }

/* input: a queue filled by a thread from $FAKESEQ_INPUT; one byte in the pipe per queued event */
#define INQ 4096
static snd_seq_event_t g_inq[INQ];
static unsigned char g_insysex[INQ][256];
static unsigned int g_inhead, g_intail;
static pthread_mutex_t g_inlock = PTHREAD_MUTEX_INITIALIZER;
static int g_instarted;

static int ev_from_bytes(snd_seq_event_t* ev, unsigned char* sx, const unsigned char* b, int n) {
    memset(ev, 0, sizeof *ev);
    ev->source.client = FAKE_CLIENT; ev->source.port = FAKE_IN_PORT;
    if (n < 1) return 0;
    const unsigned char st = b[0] & 0xf0, ch = b[0] & 0x0f;
    switch (st) {
    case 0x80: ev->type = SND_SEQ_EVENT_NOTEOFF; ev->data.note.channel = ch; ev->data.note.note = b[1]; ev->data.note.velocity = b[2]; return 1;
    case 0x90: ev->type = SND_SEQ_EVENT_NOTEON; ev->data.note.channel = ch; ev->data.note.note = b[1]; ev->data.note.velocity = b[2]; return 1;
    case 0xa0: ev->type = SND_SEQ_EVENT_KEYPRESS; ev->data.note.channel = ch; ev->data.note.note = b[1]; ev->data.note.velocity = b[2]; return 1;
    case 0xb0: ev->type = SND_SEQ_EVENT_CONTROLLER; ev->data.control.channel = ch; ev->data.control.param = b[1]; ev->data.control.value = b[2]; return 1;
    case 0xc0: ev->type = SND_SEQ_EVENT_PGMCHANGE; ev->data.control.channel = ch; ev->data.control.value = b[1]; return 1;
    case 0xd0: ev->type = SND_SEQ_EVENT_CHANPRESS; ev->data.control.channel = ch; ev->data.control.value = b[1]; return 1;
    case 0xe0: ev->type = SND_SEQ_EVENT_PITCHBEND; ev->data.control.channel = ch; ev->data.control.value = (b[1] | (b[2] << 7)) - 8192; return 1;
    }
    if (b[0] == 0xf0 && n <= 256) { ev->type = SND_SEQ_EVENT_SYSEX; memcpy(sx, b, (size_t)n); ev->data.ext.len = (unsigned int)n; ev->data.ext.ptr = sx; return 1; }
    return 0;
}

static void* input_thread(void* arg) {
    fake_seq_t* s = arg;
    const char* path = getenv("FAKESEQ_INPUT");
    FILE* f = path ? fopen(path, "r") : NULL;
    if (!f) return NULL;
    const char* go = getenv("FAKESEQ_INPUT_GO");
    if (go) {
        const struct timespec tick = { 0, 2000000 };
        while (access(go, F_OK) != 0) nanosleep(&tick, NULL);
        logline("go", NULL, 0);
    }
    struct timespec base; clock_gettime(CLOCK_MONOTONIC, &base);
    char line[1024];
    while (fgets(line, sizeof line, f)) {
        char* hash = strchr(line, '#'); if (hash) *hash = 0;
        char* p = line; char* end;
        const double ms = strtod(p, &end);
        if (end == p) continue;
        p = end;
        unsigned char b[256]; int n = 0;
        for (;;) { const long v = strtol(p, &end, 16); if (end == p || n == 256) break; b[n++] = (unsigned char)v; p = end; }
        if (n == 0) continue;
        struct timespec due = base;
        due.tv_sec += (time_t)(ms / 1000.0);
        due.tv_nsec += (long)((ms - 1000.0 * (double)(long)(ms / 1000.0)) * 1e6);
        if (due.tv_nsec >= 1000000000L) { due.tv_sec++; due.tv_nsec -= 1000000000L; }
        while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &due, NULL) == EINTR) {}
        pthread_mutex_lock(&g_inlock);
        const unsigned int slot = g_intail % INQ;
        const int ok = (g_intail - g_inhead) < INQ && ev_from_bytes(&g_inq[slot], g_insysex[slot], b, n);
        if (ok) g_intail++;
        pthread_mutex_unlock(&g_inlock);
        if (ok) { logline("in", b, (unsigned int)n); const char c = 1; if (write(s->fd[1], &c, 1) != 1) break; }
    }
    fclose(f);
    return NULL;
}

static void start_input(fake_seq_t* s) {
    if (g_instarted || !getenv("FAKESEQ_INPUT")) return;
    g_instarted = 1;
    pthread_t t;
    if (pthread_create(&t, NULL, input_thread, s) == 0) pthread_detach(t);
}

int snd_seq_poll_descriptors_count(fake_seq_t* s, short events) { (void)s; (void)events; return 1; }
int snd_seq_poll_descriptors(fake_seq_t* s, struct pollfd* pfds, unsigned int space, short events) {
    if (space < 1) return 0;
    pfds[0].fd = s->fd[0]; pfds[0].events = POLLIN; pfds[0].revents = 0; (void)events; return 1;
}
int snd_seq_event_input_pending(fake_seq_t* s, int fetch) {
    (void)s; (void)fetch;
    pthread_mutex_lock(&g_inlock); const int n = (int)(g_intail - g_inhead); pthread_mutex_unlock(&g_inlock);
    return n;
}
static snd_seq_event_t g_inev;
int snd_seq_event_input(fake_seq_t* s, snd_seq_event_t** ev) {
    pthread_mutex_lock(&g_inlock);
    if (g_intail == g_inhead) { pthread_mutex_unlock(&g_inlock); *ev = NULL; return -EAGAIN; }
    g_inev = g_inq[g_inhead % INQ]; g_inhead++;
    pthread_mutex_unlock(&g_inlock);
    char c; if (read(s->fd[0], &c, 1) != 1) { /* the byte for this event; ignore errors */ }
    *ev = &g_inev;
    return 1;
}

/* raw byte to event encoder (used by Wine for input; kept minimal) */
int snd_midi_event_new(size_t bufsize, fake_midi_event_t** out) { (void)bufsize; *out = calloc(1, sizeof **out); return *out ? 0 : -1; }
void snd_midi_event_free(fake_midi_event_t* e) { free(e); }
void snd_midi_event_init(fake_midi_event_t* e) { memset(e, 0, sizeof *e); }
long snd_midi_event_encode_byte(fake_midi_event_t* e, int c, snd_seq_event_t* ev) {
    /* Wine's output path uses this only for one-byte system messages (tune request, clock, start,
     * continue, stop, active sensing): keep the byte, logged as a short message. */
    (void)e;
    ev->type = 90; /* SND_SEQ_EVENT_USR0 */
    ev->data.raw8[0] = (unsigned char)c;
    return 1;
}
