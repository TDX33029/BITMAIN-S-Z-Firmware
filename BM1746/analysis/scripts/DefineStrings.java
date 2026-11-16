import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.app.cmd.data.CreateStringCmd;
public class DefineStrings extends GhidraScript {
    @Override
    public void run() throws Exception {
        int created = 0;
        AddressSetView set = currentProgram.getMemory().getLoadedAndInitializedAddressSet();
        AddressIterator ai = set.getAddresses(true);
        while (ai.hasNext()) {
            Address a = ai.next();
            try {
                Data d = currentProgram.getListing().getDataAt(a);
                if (d != null) {
                    if (d.hasStringValue()) { created++; continue; }
                    if (d.getLength() > 1) continue;
                }
                byte b = (byte) currentProgram.getMemory().getByte(a);
                if (b >= 0x20 && b < 0x7f) {
                    CreateStringCmd cmd = new CreateStringCmd(a);
                    if (cmd.applyTo(currentProgram)) created++;
                }
            } catch (Exception e) { }
        }
        println("strings created: " + created);
    }
}
