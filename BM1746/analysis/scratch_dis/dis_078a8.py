import capstone
data = open(r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746\parts\uImage","rb").read()
# find function: try payload offset 64 -> VA 0x40000000
def dis(off, va, n):
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    code = data[off:off+n*4]
    for i in md.disasm(code, va):
        print("%08x  %s %s" % (i.address, i.mnemonic, i.op_str))
for hdr in (64, 0):
    off = hdr + 0x78a8
    print("=== header", hdr)
    dis(off, 0x400078a8, 12)
