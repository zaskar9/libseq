#!/usr/bin/env python3
"""wrk.py FILE...: a chunk-by-chunk dump of Cakewalk .WRK files, as written on the first day
(step 1). Some readings were corrected later by observation (TIMEFMT, THRU, METRONOME): the
corrected layouts are in docs/reverse-engineering/installation-media.md."""
import struct,sys
NAMES={1:'TRACK',2:'STREAM',3:'VARS',4:'TEMPO',5:'METER',6:'SYSEX',7:'MEMRGN',8:'COMMENTS',9:'TRKOFFS',10:'TIMEBASE',11:'TIMEFMT',12:'TRKREPS',14:'TRKPATCH',15:'NTEMPO',16:'THRU',18:'LYRICS',19:'TRKVOL',20:'SYSEX2',21:'MARKERS',22:'STRTAB',23:'METERKEY',24:'TRKNAME',26:'VARIABLE',27:'NTRKOFS',30:'TRKBANK',36:'NTRACK',44:'NSYSEX',45:'NSTREAM',49:'SGMNT',74:'SOFTVER',255:'END'}
def u8(b,o): return b[o]
def u16(b,o): return struct.unpack_from('<H',b,o)[0]
def s16(b,o): return struct.unpack_from('<h',b,o)[0]
def u24(b,o): return b[o]|b[o+1]<<8|b[o+2]<<16
def u32(b,o): return struct.unpack_from('<I',b,o)[0]
def mbt(t,ppq,meters):
    # meters: list of (measure(0-based), num, den)
    m=0; beat_len=ppq*4//meters[0][2]; mlen=meters[0][1]*beat_len
    for (ms,num,den) in meters: pass
    meas=t//mlen; rem=t%mlen; return f"{meas+1}:{rem//beat_len+1}:{rem%beat_len:03d}"
def parse(path):
    b=open(path,'rb').read(); assert b[:8]==b'CAKEWALK'
    print(f"==== {path}  WRK version {b[10]}.{b[9]}  size={len(b)}")
    o=11; ppq=120; meters=[(0,4,4)]; stats={}
    while o<len(b):
        ck=b[o]; o+=1
        if ck==255: print(" [END]"); break
        ln=u32(b,o); o+=4; d=b[o:o+ln]; o+=ln
        name=NAMES.get(ck,f'#{ck}'); stats[name]=stats.get(name,0)+1
        if ck==10: ppq=u16(d,0); print(f" TIMEBASE ppq={ppq}")
        elif ck==3:
            now,frm,thru=u32(d,0),u32(d,4),u32(d,8); keysig,clock,autosave,playdelay=d[12],d[13],d[14],d[15]
            zc,spp,cont,ps,astop=d[17:22]; stop=u32(d,22); arew=d[26]; rewt=u32(d,27); mp,mr,ma,ci=d[31:35]; thruon=d[37]
            print(f" VARS now={now} from={frm} thru={thru} keysig={keysig} clock={clock}(0=int,1=midi,2=smpte) autosave={autosave}min metro play/rec/accent={mp}/{mr}/{ma} countin={ci} thru={thruon} zeroctl={zc} spp={spp} patchsearch={ps} len={ln}")
        elif ck==1:
            tn=u16(d,0); p=2; nm=[]
            for i in range(2): l=d[p]; nm.append(d[p+1:p+1+l].decode('latin1')); p+=1+l
            ch,key,vel,port,fl=d[p],struct.unpack('b',d[p+1:p+2])[0],struct.unpack('b',d[p+2:p+3])[0],d[p+3],d[p+4]
            print(f" TRACK #{tn+1:2d} name='{nm[0]}' name2='{nm[1]}' chan={ch&15 if ch<16 else ch} key+={key} vel+={vel} port={port} flags=0x{fl:02x}({'sel ' if fl&1 else ''}{'mute ' if fl&2 else ''}{'loop' if fl&4 else ''}) rawlen={ln}")
        elif ck==2:
            tn=u16(d,0); n=u16(d,2); kinds={}
            for i in range(n):
                p=4+8*i; st=d[p+3]&0xf0; kinds[st]=kinds.get(st,0)+1
            first=[]; 
            for i in range(min(n,4)):
                p=4+8*i; first.append(f"t={u24(d,p)} {d[p+3]:02x} {d[p+4]} {d[p+5]} dur={u16(d,p+6)}")
            print(f" STREAM track#{tn+1} events={n} kinds={{{', '.join(f'{k:02x}:{v}' for k,v in sorted(kinds.items()))}}}  first: "+" | ".join(first))
        elif ck==4 or ck==15:
            n=u16(d,0); f=1 if ck==4 else 100
            ts=[(u32(d,2+18*i), u16(d,2+18*i+8)*f/100) for i in range(n)]
            print(f" {name} count={n}: "+", ".join(f"t={t}:{bpm:g}bpm" for t,bpm in ts[:12])+(" ..." if n>12 else ""))
        elif ck==5:
            n=u16(d,0); ms=[(u16(d,2+12*i+4),d[2+12*i+6],2**d[2+12*i+7]) for i in range(n)]; meters=ms
            print(f" METER count={n}: "+", ".join(f"meas{m+1}:{a}/{bb}" for m,a,bb in ms))
        elif ck==8: print(f" COMMENTS: {d[2:2+u16(d,0)].decode('latin1')!r}")
        elif ck==9: print(f" TRKOFFS track#{u16(d,0)+1} offset={s16(d,2)} ticks")
        elif ck==12: print(f" TRKREPS track#{u16(d,0)+1} reps={u16(d,2)}")
        elif ck==14: print(f" TRKPATCH track#{u16(d,0)+1} patch={struct.unpack('b',d[2:3])[0]}")
        elif ck==19: print(f" TRKVOL track#{u16(d,0)+1} vol={struct.unpack('<h',d[2:4])[0]}")
        elif ck==11: print(f" TIMEFMT fmt={u16(d,0)} ofs={u16(d,2)}")
        elif ck==16: print(f" THRU port={d[2]} chan={struct.unpack('b',d[3:4])[0]} key+={d[4]} vel+={d[5]} localport={d[6]} mode={d[7]}")
        elif ck==6: l=u16(d,1); nl=d[4]; print(f" SYSEX bank={d[0]} len={l} autosend={d[3]} name='{d[5:5+nl].decode('latin1')}'")
        elif ck==21:
            n=u32(d,0); p=4; ms=[]
            for i in range(n):
                smpte=d[p]; t=u24(d,p+2); l=d[p+10]; ms.append(f"t={t}{' (smpte)' if smpte else ''}:'{d[p+11:p+11+l].decode('latin1')}'"); p+=11+l
            print(f" MARKERS n={n}: "+", ".join(ms))
        elif ck==7: print(f" MEMRGN len={ln} {d[:16].hex()}")
        else: print(f" {name} len={ln} data={d[:24].hex()}{'...' if ln>24 else ''}")
    print(" chunk summary:",stats)
for p in sys.argv[1:]: parse(p); print()
