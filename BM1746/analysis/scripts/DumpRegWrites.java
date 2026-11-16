// Ghidra script: dump functions + all load/store to Zynq PS register space (0xF8000000-0xF8FFFFFF, 0xE0000000-0xE02FFFFF)
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import java.io.*;
import java.util.*;

public class DumpRegWrites extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/reg_writes.txt"));
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        FunctionIterator fi = currentProgram.getFunctionManager().getFunctions(true);
        while (fi.hasNext()) {
            Function f = fi.next();
            if (f.isThunk()) continue;
            DecompileResults res = di.decompileFunction(f, 30, monitor);
            if (res == null || !res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            boolean hasReg = c.matches("(?s).*(0x[fF]8[0-9a-fA-F]{6}|0x[eE]0[0-9a-fA-F]{6}).*");
            if (hasReg) {
                pw.println("//==== " + f.getName() + " @ " + f.getEntryPoint());
                pw.println(c);
                pw.println();
            }
        }
        pw.close();
        di.dispose();
        println("done");
    }
}
