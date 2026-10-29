#!/usr/bin/env python3
"""resolve.py FILE...: the functions an NE file imports by ordinal from the Windows 3.1 system
modules, resolved to names through Wine's 16-bit .spec files (output for the Cakewalk Apprentice
1.04 disk in docs/reverse-engineering/static/imports.txt).

The .spec files come from a Wine source tree (LGPL, not copied here): set WINE_SPECS to a folder
holding krnl386.exe16.spec, user.exe16.spec, gdi.exe16.spec, mmsystem.dll16.spec,
commdlg.dll16.spec, shell.dll16.spec, keyboard.drv16.spec and win87em.dll16.spec, taken from the
dlls/<module>/ folders of Wine's sources (default: a folder "specs" next to this script)."""
import os,re,sys,struct,collections
SPECS=os.environ.get('WINE_SPECS',os.path.join(os.path.dirname(os.path.abspath(__file__)),'specs'))
specs={'KERNEL':'krnl386.exe16.spec','USER':'user.exe16.spec','GDI':'gdi.exe16.spec','MMSYSTEM':'mmsystem.dll16.spec','COMMDLG':'commdlg.dll16.spec','SHELL':'shell.dll16.spec','KEYBOARD':'keyboard.drv16.spec','WIN87EM':'win87em.dll16.spec'}
tab={}
for m,f in specs.items():
    d={}
    for line in open(os.path.join(SPECS,f),encoding='latin1'):
        mm=re.match(r'\s*(\d+)\s+(\w+)\s+(?:-\w+\s+)*(\w+)',line)
        if mm: d[int(mm.group(1))]=mm.group(3)
    tab[m]=d
def rd(b,o,fmt): return struct.unpack_from('<'+fmt,b,o)
def imports(path):
    b=open(path,'rb').read(); ne=rd(b,0x3c,'I')[0]
    nseg,nmod=rd(b,ne+0x1c,'HH'); seg_off,res_off,resname_off,modref_off,impname_off=rd(b,ne+0x22,'HHHHH'); align=rd(b,ne+0x32,'H')[0]
    mods=[]
    for i in range(nmod):
        off=rd(b,ne+modref_off+2*i,'H')[0]; o=ne+impname_off+off; mods.append(b[o+1:o+1+b[o]].decode('latin1'))
    byord=collections.defaultdict(set)
    for i in range(nseg):
        so,sl,sf,smin=rd(b,ne+seg_off+8*i,'HHHH')
        if not (sf&0x100): continue
        o=(so<<align)+(sl or 65536); n=rd(b,o,'H')[0]; o+=2
        for k in range(n):
            atype,rtype,off,a,c=rd(b,o,'BBHHH'); o+=8
            if rtype&3==1: byord[mods[a-1]].add(c)
    print("==",path)
    for m in mods:
        d=tab.get(m.upper(),{})
        if not d: continue
        names=[d.get(o,f'#{o}') for o in sorted(byord[m])]
        print(f" {m} ({len(names)}): "+", ".join(names))
for p in sys.argv[1:]: imports(p)
