// Decompile the functions that reference FsblHook / ps7 / key strings, plus full call-tree from entry
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import java.io.*;
import java.util.*;

public class DumpHooks extends GhidraScript {
    String[] keys = {"FsblHook","BitstreamDload","Handoff","ps7","PS7","mask poll","FPGA Done","Fabric"};
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/hooks.txt"));
        DecompInterface di = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        di.setOptions(opts);
        di.openProgram(currentProgram);
        // find defined strings
        FunctionManager fm = currentProgram.getFunctionManager();
        Listing listing = currentProgram.getListing();
        Set<Function> targets = new LinkedHashSet<>();
        DataIterator di2 = listing.getDefinedData(true);
        while (di2.hasNext()) {
            Data dat = di2.next();
            if (dat.hasStringValue()) {
                String v = dat.getValue().toString();
                for (String k : keys) {
                    if (v.contains(k)) {
                        ReferenceIterator ri = currentProgram.getReferenceManager().getReferencesTo(dat.getAddress());
                        while (ri.hasNext()) {
                            Reference r = ri.next();
                            Function f = getFunctionContaining(r.getFromAddress());
                            if (f != null) targets.add(f);
                        }
                    }
                }
            }
        }
        println("target functions: " + targets.size());
        for (Function f : targets) {
            DecompileResults res = di.decompileFunction(f, 60, monitor);
            if (res == null || !res.decompileCompleted()) continue;
            pw.println("//==================== " + f.getName() + " @ " + f.getEntryPoint() + " ====================");
            pw.println(res.getDecompiledFunction().getC());
            pw.println();
        }
        pw.close();
        di.dispose();
    }
}
