#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
int PASCAL WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {
    FILE* f = fopen("caps16.txt", "w");
    UINT n = midiOutGetNumDevs();
    int i;
    fprintf(f, "midiOutGetNumDevs = %u\n", n);
    for (i = -1; i < (int)n; ++i) {
        MIDIOUTCAPS c; UINT r = midiOutGetDevCaps((UINT)i, &c, sizeof c);
        fprintf(f, "dev %d: r=%u mid=%u pid=%u ver=%04x name='%s' tech=%u\n", i, r, c.wMid, c.wPid, c.vDriverVersion, (char far*)c.szPname, c.wTechnology);
    }
    fclose(f);
    return 0;
}
