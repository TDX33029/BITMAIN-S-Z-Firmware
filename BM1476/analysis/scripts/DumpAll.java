// Dump ALL decompiled functions to one file for offline analysis
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
public class DumpAll extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/all_decomp.c"));
        DecompInterface di = new DecompInterface();
        di.setOptions(new DecompileOptions());
        di.openProgram(currentProgram);
        FunctionIterator fi = currentProgram.getFunctionManager().getFunctions(true);
        int n=0;
        while (fi.hasNext()) {
            Function f = fi.next();
            if (f.isThunk()) continue;
            DecompileResults res = di.decompileFunction(f, 45, monitor);
            n++;
            if (res == null || !res.decompileCompleted()) continue;
            pw.println("//==================== " + f.getName() + " @ " + f.getEntryPoint() + " ====================");
            pw.println(res.getDecompiledFunction().getC());
            pw.println();
        }
        pw.close();
        di.dispose();
        println("functions decompiled: " + n);
    }
}
