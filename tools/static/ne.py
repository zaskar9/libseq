#!/usr/bin/env python3
"""ne.py FILE...: the NE header of 16-bit Windows executables and DLLs: linker version, flags,
expected Windows version, module name and description, imported modules, exported names with
ordinals, and the segment table. (Step 1; output for the Cakewalk Apprentice 1.04 disk in
docs/reverse-engineering/static/ne-headers.txt.)"""
import struct, sys
def rd(b,o,fmt): return struct.unpack_from('<'+fmt,b,o)
def parse(path):
    b=open(path,'rb').read()
    ne=rd(b,0x3c,'I')[0]
    assert b[ne:ne+2]==b'NE', 'not NE'
    (linker_ver,linker_rev,entry_off,entry_len,crc,flags,autodata,heap,stack,
     csip,sssp,nseg,nmod,nonres_len,seg_off,res_off,resname_off,modref_off,impname_off,
     nonres_off,nmovent,align,nres,os,oflags,retthunk,segrefthunk,swap,winver)=rd(b,ne+2,'BBHHIHHHHIIHHHHHHHHIHHHBBHHHH')
    print(f"== {path}")
    print(f" linker ver {linker_ver}.{linker_rev}  flags=0x{flags:04x} ({'DLL' if flags&0x8000 else 'EXE'}) target OS={os} expected win ver={winver>>8}.{winver&0xff}")
    print(f" segments={nseg} modrefs={nmod} autodata seg={autodata} heap={heap} stack={stack} align=1<<{align} nres={nres}")
    # resident names
    def names(off,end=None):
        out=[];o=off
        while True:
            n=b[o]
            if n==0: break
            s=b[o+1:o+1+n].decode('latin1'); ordn=rd(b,o+1+n,'H')[0]; out.append((ordn,s)); o+=3+n
        return out
    res=names(ne+resname_off)
    print(" module name:",res[0][1] if res else None)
    nonres=names(nonres_off)
    print(" description:",nonres[0][1] if nonres else None)
    # imported module names
    mods=[]
    for i in range(nmod):
        off=rd(b,ne+modref_off+2*i,'H')[0]
        o=ne+impname_off+off; n=b[o]; mods.append(b[o+1:o+1+n].decode('latin1'))
    print(" imported modules:",", ".join(mods))
    exp=[(o,s) for o,s in res[1:]+nonres[1:]]
    exp.sort()
    print(f" exported names ({len(exp)}):")
    for o,s in exp: print(f"   {o:4d} {s}")
    # segments
    print(" segments:")
    for i in range(nseg):
        so,sl,sf,smin=rd(b,ne+seg_off+8*i,'HHHH')
        t='CODE' if not sf&1 else 'DATA'
        print(f"   {i+1:2d} {t} file_off=0x{so<<align:06x} len={sl or 65536:6d} minalloc={smin or 65536:6d} flags=0x{sf:04x}{' MOVEABLE' if sf&0x10 else ''}{' PRELOAD' if sf&0x40 else ''}{' DISCARD' if sf&0x1000 else ''}")
    return b,ne,mods
if __name__=='__main__':
    for p in sys.argv[1:]: parse(p)
