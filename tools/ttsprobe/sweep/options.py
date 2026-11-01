#!/usr/bin/env python3
"""options.py VER TEMPLATE [KEY=VALUE[,KEY=VALUE...]]... : what one TTSSEQ.INI [Options] setting changes.

Runs TEMPLATE (render.py syntax, through try.sh) on one version, first with the usual TTSSEQ.INI,
then once per argument with those keys added to [Options], and prints what differs from the first
run: the probe's output, the MIDI the fake port got (messages in order, without times) and, with
--relay, the multimedia calls the engine made (Wine's relay trace: timers, MIDI buffers, memory).
The usual TTSSEQ.INI is put back afterwards. Options:
  --input FILE   scripted MIDI input for the fake port
  --irq          interrupt-time callbacks (WINE_WIN16_IRQ_CALLBACKS=serial)
  --relay        compare the engine's MMSYSTEM calls too (slow: Wine's relay trace)
  --times        print the MIDI with times (ms after the first message) instead of a diff
  --outputs2     two output devices: port 1 the fake's second port, port 2 its first (the MIDI
                 lines then name the fake port, @2 or @0)
Results are kept in $TTSPROBE_WORK/options/.
Example: options.py 210 t.tpl SendLocalOff=0 SendLocalOn=1
"""
import difflib, os, re, subprocess, sys

here = os.path.dirname(os.path.abspath(__file__))
work = os.environ.get('TTSPROBE_WORK', os.path.expanduser('~/ttsprobe-work'))
prefix = os.environ.get('WINEPREFIX', os.path.join(work, 'prefix'))
args = sys.argv[1:]
flags = {a for a in args if a in ('--irq', '--relay', '--times', '--outputs2')}
inp = None
if '--input' in args:
    i = args.index('--input'); inp = os.path.abspath(args[i + 1]); del args[i:i + 2]
args = [a for a in args if a not in flags]
ver, tpl, variants = args[0], os.path.abspath(args[1]), args[2:]
ini_path = os.path.join(prefix, 'drive_c', 'PROBE', 'V' + ver, 'TTSSEQ.INI')
out_dir = os.path.join(work, 'options'); os.makedirs(out_dir, exist_ok=True)
USUAL = '[Options]\r\nSyncOutputPort=1\r\n[MIDI Output Devices]\r\n0=Fake - MIDI Out\r\n[MIDI Input Devices]\r\n0=Fake - MIDI In\r\n'
MM = re.compile(r'Call (MMSYSTEM\.\d+: (?:TIME\w+|MIDI\w+)|KERNEL\.\d+: GLOBAL(?:ALLOC|REALLOC))\(([^)]*)\)')


USUAL2 = USUAL.replace('0=Fake - MIDI Out\r\n', '0=Fake - MIDI Out 2\r\n1=Fake - MIDI Out\r\n')


def run(tag, extra):
    keys = {k for k, _ in extra}  # a key given here replaces the usual line (the first one counts)
    sync = '' if 'SyncOutputPort' in keys else 'SyncOutputPort=1\r\n'
    ini = (USUAL2 if '--outputs2' in flags else USUAL).replace('SyncOutputPort=1\r\n', sync + ''.join(f'{k}={v}\r\n' for k, v in extra))
    with open(ini_path, 'w', newline='') as f:
        f.write(ini)
    env = dict(os.environ, TTSPROBE_WORK=work, FAKELOG=os.path.join(out_dir, tag + '.fake'))
    if inp: env['FAKESEQ_INPUT'] = inp
    if '--outputs2' in flags: env['FAKESEQ_OUTPUTS'] = '2'
    if '--irq' in flags: env['WINE_WIN16_IRQ_CALLBACKS'] = 'serial'
    env['TRACE'] = '+relay' if '--relay' in flags else '-all'
    open(env['FAKELOG'], 'w').close()
    out = subprocess.run([os.path.join(here, '..', 'oracles', 'try.sh'), ver, '40'], stdin=open(tpl),
                         capture_output=True, text=True, env=env).stdout
    midi, t0 = [], None
    for line in open(env['FAKELOG']):
        p = line.split()
        if len(p) > 2 and p[1] in ('short', 'sysex', 'long') or (len(p) > 2 and p[1].startswith('short@')):
            t = float(p[0]); t0 = t if t0 is None else t0
            midi.append((t - t0, ' '.join(p[1:])))
    mm, ticks = [], 0
    if '--relay' in flags:
        for line in open(os.path.join(work, 'try.log'), errors='replace'):
            m = MM.search(line)
            if m and 'TIMEGETTIME' not in m.group(1):  # called by every timer callback: counted below
                mm.append(m.group(1).split(': ')[1] + '(' + re.sub(r'\b[0-9a-f]{4}:[0-9a-f]{4}\b', 'ptr', m.group(2)) + ')')
            elif m: ticks += 1
        mm.append(f'(timeGetTime calls: about {round(ticks, -1)})')
    return out.splitlines(), midi, mm


def show(title, a, b):
    d = list(difflib.unified_diff(a, b, 'usual', title, n=1, lineterm=''))
    print('\n'.join(d[2:]) if d else '  (no difference)')


try:
    base = run('usual', [])
    for v in variants:
        extra = [kv.split('=', 1) for kv in v.split(',')]
        res = run(re.sub(r'[^A-Za-z0-9=,_-]', '_', v), extra)
        print(f'=== {ver} {v}')
        print('-- probe'); show(v, base[0], res[0])
        if '--times' in flags:
            print('-- MIDI'); print('\n'.join(f'  {t:9.1f} {m}' for t, m in res[1]))
        else:
            print('-- MIDI'); show(v, [m for _, m in base[1]], [m for _, m in res[1]])
        if '--relay' in flags:
            print('-- calls'); show(v, base[2], res[2])
finally:
    with open(ini_path, 'w', newline='') as f:
        f.write(USUAL)
