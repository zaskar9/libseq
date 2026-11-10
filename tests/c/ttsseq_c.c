/* A C program on libseq's C interface (include/libseq/ttsseq.h): it compiles as C, links, and
 * makes a few calls whose results the oracle suites establish for TTSSEQ.DLL 1.01. */
#include "libseq/ttsseq.h"

#include <stdio.h>
#include <string.h>

static int sent;
static unsigned char last[3];

static int ports(void* context) {
    (void)context;
    return 1;
}
static const char* name(void* context, int port) {
    (void)context;
    (void)port;
    return "Test Out";
}
static void send(void* context, int port, const uint8_t* message, size_t length) {
    (void)context;
    (void)port;
    ++sent;
    if (length <= sizeof last) {
        memcpy(last, message, length);
    }
}
static uint32_t milliseconds(void* context) {
    (void)context;
    return 0;
}
static int failures;
static void check(int ok, const char* what) {
    if (!ok) {
        printf("failed: %s\n", what);
        ++failures;
    }
}

int main(void) {
    LIBSEQPLATFORM platform;
    SEQEVENT note;
    HSEQ seq;
    HSTREAM stream;
    char text[64];
    memset(&platform, 0, sizeof platform);
    platform.outputPorts = ports;
    platform.outputName = name;
    platform.send = send;
    platform.milliseconds = milliseconds;
    LibSeqSetPlatform(&platform);

    check(SeqGetVersion() == 0x0101, "SeqGetVersion");
    check(SeqOpen(0, 0) == 1, "SeqOpen");
    check(sent == 16, "Local Control Off on 16 channels");
    seq = SeqCreateSeq(0, 0);
    check(seq == 1, "SeqCreateSeq");
    check(SeqGetPPQ(seq) == 120, "SeqGetPPQ");
    stream = SeqCreateStream(seq, 0, 0);
    check(stream == 0x0100, "SeqCreateStream");

    memset(&note, 0, sizeof note);
    note.time[0] = 240; /* tick 240: measure 1, beat 3 in 4/4 at 120 ppq */
    note.status = 0x90;
    note.data1 = 60;
    note.data2 = 100;
    note.duration = 120;
    check(SeqInsEvt(stream, &note) == 1, "SeqInsEvt");
    check(SeqStreamLen(stream) == 1, "SeqStreamLen");
    memset(&note, 0, sizeof note);
    check(SeqSeek(stream, 0, 0) == 0 && SeqGetEvt(stream, &note) == 1, "SeqGetEvt");
    check(note.time[0] == 240 && note.data1 == 60 && note.duration == 120, "the event comes back");

    check(strcmp(SeqTick2MBTStr(seq, 240, text, 0), "1:3:000") == 0, "SeqTick2MBTStr");
    check(SeqMBT2Tick(seq, 2, 1, 0) == 480, "SeqMBT2Tick");
    check(strcmp(SeqTempoRatio2Str(0x50, text), "1.25") == 0, "SeqTempoRatio2Str");
    check(SeqGetPortName(0, text) == 1 && strcmp(text, "1: Test Out") == 0, "SeqGetPortName");

    sent = 0;
    check(SeqTxMidiMsg(0, 0, 0x90, 60, 100) == 1, "SeqTxMidiMsg");
    check(sent == 1 && last[0] == 0x90 && last[1] == 60 && last[2] == 100, "the note on goes out");
    check(SeqTxMidiMsg(0, 0, 0xe0, 0, 0x40) == 0, "no pitch wheel");

    check(SeqClose() == 1, "SeqClose");
    LibSeqUnload();
    LibSeqSetPlatform(NULL);
    if (failures == 0) {
        printf("ok\n");
    }
    return failures == 0 ? 0 : 1;
}
