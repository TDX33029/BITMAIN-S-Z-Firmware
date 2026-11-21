import sys
ELF=r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746\parts\Zynq7007_miner.elf"
f=open(ELF,'rb').read()
def cstr(vaddr,maxn=300):
    d=f[vaddr:vaddr+maxn]
    i=d.find(b'\0')
    if i<0: i=len(d)
    return d[:i]
if __name__=='__main__':
    for a in sys.argv[1:]:
        va=int(a,16)
        print("0x%08x: %r"%(va,cstr(va)))
