import struct, sys
elf = open(r"D:\Document\tempPrj\BITMAIN-S-Z-Firmware\BM1746\analysis\cgminer_z15_bm1746.elf","rb").read()
assert elf[:4] == b"\x7fELF"
e_shoff = struct.unpack_from("<I", elf, 0x20)[0]
e_shentsize = struct.unpack_from("<H", elf, 0x2e)[0]
e_shnum = struct.unpack_from("<H", elf, 0x30)[0]
secs = []
for i in range(e_shnum):
    off = e_shoff + i*e_shentsize
    sh_addr = struct.unpack_from("<I", elf, off+0x0c)[0]
    sh_off  = struct.unpack_from("<I", elf, off+0x10)[0]
    sh_size = struct.unpack_from("<I", elf, off+0x14)[0]
    sh_type = struct.unpack_from("<I", elf, off+0x04)[0]
    if sh_type == 1 and sh_addr <= 0x30500 < sh_addr+sh_size:
        secs.append((sh_addr, sh_off, sh_size))
if not secs:
    print("no section found"); sys.exit(1)
sh_addr, sh_off, sh_size = secs[0]
def rd(vaddr):
    return sh_off + (vaddr - sh_addr)
def dec(w):
    op = w >> 26
    rs = (w>>21)&31; rt=(w>>16)&31; rd_=(w>>11)&31
    imm = w & 0xffff
    simm = imm-0x10000 if imm>=0x8000 else imm
    fn = w & 63
    tgt = (w & 0x3ffffff) << 2
    if op == 0:
        names={0x21:"addu",0x23:"addu",0x25:"or",0x24:"and",0x00:"sll",0x02:"srl",0x03:"sra",0x2a:"slt",0x2b:"sltu",0x08:"jr",0x12:"mflo",0x10:"mfhi",0x18:"mult",0x19:"multu",0x1a:"div",0x1b:"divu"}
        s=f"{names.get(fn,'r2?')}"
        if fn==0: return f"sll r{rd_},r{rt},{(w>>6)&31}"
        if fn==8: return f"jr r{rs}"
        return f"{s} r{rd_},r{rs},r{rt}" if fn not in (0x12,0x10) else f"{s} r{rd_}"
    opn={2:"j",3:"jal",4:"beq",5:"bne",6:"blez",7:"bgtz",9:"addiu",8:"addi",0xa:"slti",0xb:"sltiu",0xc:"andi",0xd:"ori",0xe:"xori",0xf:"lui",0x20:"lb",0x21:"lh",0x23:"lw",0x24:"lbu",0x25:"lhu",0x28:"sb",0x29:"sh",0x2b:"sw",0x2f:"cache",0x30:"ll",0x38:"sc",0x1c:"special3",0x1f:"special3"}
    if op in (4,5):
        return f"{opn[op]} r{rt},r{rs},0x{tgt:04x}" if imm&0x8000 else f"{opn[op]} r{rt},r{rs},0x{tgt:04x}"
    if op in (6,7):
        return f"{opn[op]} r{rs},0x{tgt:04x}"
    if op==0xf: return f"lui r{rt},0x{imm:04x}"
    return f"{opn.get(op,f'op{op}')} r{rt},r{rs},0x{imm&0xffff:04x}({simm})"
start, end = 0x30500, 0x30660
for v in range(start, end, 4):
    w = struct.unpack_from("<I", elf, rd(v))[0]
    print(f"{v:08x}: {w:08x}  {dec(w)}")
