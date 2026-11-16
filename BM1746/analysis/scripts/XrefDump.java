import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.app.decompiler.*;
import java.io.*;
import java.util.LinkedHashSet;
import java.util.Set;
public class XrefDump extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        String[] addrs = getScriptArgs()[1].split(",");
        DecompInterface di = new DecompInterface();
        di.setOptions(new DecompileOptions());
        di.openProgram(currentProgram);
        PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/xref_dump.c"));
        for (String sa : addrs) {
            Address a = currentProgram.getAddressFactory().getAddress(sa);
            pw.println("//#### refs to " + sa + " ####");
            ReferenceIterator ri = currentProgram.getReferenceManager().getReferencesTo(a);
            Set<Function> fs = new LinkedHashSet<>();
            while (ri.hasNext()) {
                Reference r = ri.next();
                Function f = getFunctionContaining(r.getFromAddress());
                if (f != null) fs.add(f);
                else pw.println("//  raw ref from " + r.getFromAddress());
            }
            for (Function f : fs) {
                DecompileResults res = di.decompileFunction(f, 60, monitor);
                if (res != null && res.decompileCompleted()) {
                    pw.println("//==== " + f.getName() + " @ " + f.getEntryPoint());
                    pw.println(res.getDecompiledFunction().getC());
                    pw.println();
                }
            }
        }
        pw.close(); di.dispose();
        println("xref dump done");
    }
}
