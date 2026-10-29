#!/usr/bin/env python3
"""neres.py FILE: the resources of an NE file: menus, the string table, dialogs with their
controls, and the version resource."""
import struct,sys
def rd(b,o,fmt): return struct.unpack_from('<'+fmt,b,o)
TYPES={1:'CURSOR',2:'BITMAP',3:'ICON',4:'MENU',5:'DIALOG',6:'STRING',7:'FONTDIR',8:'FONT',9:'ACCEL',10:'RCDATA',12:'GROUP_CURSOR',14:'GROUP_ICON',16:'VERSION'}
def res(path, dump=()):
    b=open(path,'rb').read(); ne=rd(b,0x3c,'I')[0]; res_off=rd(b,ne+0x24,'H')[0]; t=ne+res_off
    shift=rd(b,t,'H')[0]; o=t+2; out=[]
    def name(x):
        if x&0x8000: return TYPES.get(x&0x7fff,f'#{x&0x7fff}')
        p=t+x; return b[p+1:p+1+b[p]].decode('latin1')
    while True:
        tid,cnt=rd(b,o,'HH'); 
        if tid==0: break
        o+=8
        for i in range(cnt):
            off,ln,fl,rid=rd(b,o,'HHHH'); o+=12
            out.append((name(tid),(f'#{rid&0x7fff}' if rid&0x8000 else name(rid)),off<<shift,ln<<shift))
    return b,out
def menu(b,o,end):
    o+=4; depth=0; lines=[]
    while o<end:
        fl=rd(b,o,'H')[0]; o+=2
        if fl&0x10: # popup
            e=b.index(b'\0',o); s=b[o:e].decode('latin1'); o=e+1; lines.append('  '*depth+'+ '+s); depth+=1
        else:
            mid=rd(b,o,'H')[0]; o+=2; e=b.index(b'\0',o); s=b[o:e].decode('latin1'); o=e+1
            lines.append('  '*depth+(s if s else '---')+f'  [{mid}]')
        if fl&0x80: depth-=1
        if depth<0: break
        # handle multiple end-popup via lookahead not needed for printing
    return lines
def strings(b,o,end,rid):
    out=[]; base=(rid-1)*16; i=0
    while o<end and i<16:
        n=b[o]; o+=1
        if n: out.append((base+i,b[o:o+n].decode('latin1'))); o+=n
        i+=1
    return out
def dialog(b,o,end):
    style,=rd(b,o,'I'); n=b[o+4]; x,y,cx,cy=rd(b,o+5,'HHHH'); p=o+13
    def zs(p):
        e=b.index(b'\0',p); return b[p:e].decode('latin1'),e+1
    menu_,p=zs(p); cls,p=zs(p); cap,p=zs(p)
    if style&0x40: p+=2; fn,p=zs(p)
    items=[]
    for i in range(n):
        cx_,cy_,cw,ch,cid,cst=rd(b,p,'HHHHHI'); p+=14
        if b[p]&0x80: ccls=['BUTTON','EDIT','STATIC','LISTBOX','SCROLLBAR','COMBOBOX'][b[p]&0x7f] if (b[p]&0x7f)<6 else '?'; p+=1
        else: ccls,p=zs(p)
        txt,p=zs(p); p+=b[p]+1
        if txt: items.append(f'{ccls}:{txt}')
    return cap, items
if __name__=='__main__':
    path=sys.argv[1]; b,out=res(path)
    import collections
    c=collections.Counter(t for t,_,_,_ in out)
    print("resource types:",dict(c))
    for t,n,o,l in out:
        if t=='MENU':
            print(f"\n--- MENU {n}"); print("\n".join(menu(b,o,o+l)))
    print("\n--- STRINGTABLE")
    for t,n,o,l in out:
        if t=='STRING':
            for i,s in strings(b,o,o+l,int(n[1:])): print(f" {i:5d}: {s}")
    print("\n--- DIALOGS")
    for t,n,o,l in out:
        if t=='DIALOG':
            cap,items=dialog(b,o,o+l); print(f" [{n}] \"{cap}\": "+" | ".join(items))
    for t,n,o,l in out:
        if t=='VERSION':
            import re
            print("\n--- VERSION:", b" ".join(re.findall(rb'[\x20-\x7e]{4,}',b[o:o+l])).decode('latin1'))
