import struct, capstone

ELF = r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746\analysis\cgminer_z15_bm1746.elf"
data = open(ELF, "rb").read()

# parse ELF32 section headers to map VA -> file offset
e_shoff = struct.unpack_from("<I", data, 0x20)[0]
e_shentsize = struct.unpack_from("<H", data, 0x2E)[0]
e_shnum = struct.unpack_from("<H", data, 0x30)[0]
secs = []
for i in range(e_shnum):
    off = e_shoff + i * e_shentsize
    sh_addr, sh_offset, sh_size = struct.unpack_from("<III", data, off + 0x0C)
    secs.append((sh_addr, sh_offset, sh_size))

def va2off(va):
    for addr, off, size in secs:
        if addr and addr <= va < addr + size:
            return off + (va - addr)
    raise SystemExit("VA not found: %x" % va)

def dis(va, n):
    off = va2off(va)
    md = capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM)
    code = data[off:off + n * 4]
    for i in md.disasm(code, va):
        print("%08x  %s %s" % (i.address, i.mnemonic, i.op_str))

print("=== FUN_00042958 (get_bit) ===")
dis(0x42958, 90)
print()
print("=== FUN_00049824 (leading_zeros_256bit) ===")
dis(0x49824, 100)
