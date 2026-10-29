#!/usr/bin/env python3
"""neimp.py FILE...: the functions an NE file imports, per module, by name or by ordinal, read from
the relocation records of its segments."""
import struct, sys, collections
def rd(b,o,fmt): return struct.unpack_from('<'+fmt,b,o)
def imports(path):
    b=open(path,'rb').read(); ne=rd(b,0x3c,'I')[0]
    nseg,nmod=rd(b,ne+0x1c,'HH'); seg_off,res_off,resname_off,modref_off,impname_off=rd(b,ne+0x22,'HHHHH'); align=rd(b,ne+0x32,'H')[0]
    mods=[]
    for i in range(nmod):
        off=rd(b,ne+modref_off+2*i,'H')[0]; o=ne+impname_off+off; mods.append(b[o+1:o+1+b[o]].decode('latin1'))
    byname=collections.defaultdict(set); byord=collections.defaultdict(set)
    for i in range(nseg):
        so,sl,sf,smin=rd(b,ne+seg_off+8*i,'HHHH')
        if not (sf&0x100): continue
        o=(so<<align)+(sl or 65536); n=rd(b,o,'H')[0]; o+=2
        for k in range(n):
            atype,rtype,off,a,c=rd(b,o,'BBHHH'); o+=8
            t=rtype&3
            if t==1: byord[mods[a-1]].add(c)
            elif t==2:
                p=ne+impname_off+c; byname[mods[a-1]].add(b[p+1:p+1+b[p]].decode('latin1'))
    print("==",path)
    for m in mods:
        print(f" {m}: {len(byname[m])} by name, {len(byord[m])} by ordinal")
        if byname[m]: print("   names:", " ".join(sorted(byname[m])))
        if byord[m]: print("   ordinals:", " ".join(map(str,sorted(byord[m]))))
for p in sys.argv[1:]: imports(p)
