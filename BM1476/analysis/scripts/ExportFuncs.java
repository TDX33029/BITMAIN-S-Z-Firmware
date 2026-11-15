// Export every function as its own .c file plus callgraph and string-xref index.
// args: <outDir>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class ExportFuncs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        new File(outDir).mkdirs();

        DecompInterface di = new DecompInterface();
        di.setOptions(new DecompileOptions());
        di.openProgram(currentProgram);

        // pass 1: decompile all functions, collect call edges
        Map<String, StringBuilder> bodies = new LinkedHashMap<>();
        Map<String, Set<String>> calls = new LinkedHashMap<>();
        Map<String, String> addr = new LinkedHashMap<>();
        FunctionIterator fi = currentProgram.getFunctionManager().getFunctions(true);
        int n = 0;
        while (fi.hasNext()) {
            Function f = fi.next();
            if (f.isThunk()) continue;
            DecompileResults res = di.decompileFunction(f, 45, monitor);
            String name = f.getName();
            addr.put(name, f.getEntryPoint().toString());
            calls.put(name, new TreeSet<String>());
            if (res == null || !res.decompileCompleted()) continue;
            String c = res.getDecompiledFunction().getC();
            StringBuilder sb = new StringBuilder();
            sb.append("//==================== " + name + " @ " + f.getEntryPoint() + " ====================\n");
            sb.append(c);
            bodies.put(name, sb);
            for (Function cf : f.getCalledFunctions(monitor)) {
                if (!cf.isThunk()) calls.get(name).add(cf.getName());
            }
            n++;
        }
        di.dispose();

        // per-function files
        for (Map.Entry<String, StringBuilder> e : bodies.entrySet()) {
            String safe = e.getKey().replaceAll("[^A-Za-z0-9_.-]", "_");
            PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/" + safe + ".c"));
            pw.print(e.getValue());
            pw.close();
        }

        // callgraph edges
        PrintWriter cg = new PrintWriter(new FileWriter(outDir + "/_callgraph.txt"));
        for (Map.Entry<String, Set<String>> e : calls.entrySet()) {
            for (String t : e.getValue()) {
                cg.println(e.getKey() + " -> " + t);
            }
        }
        cg.close();

        // string xref index: walk defined string data and find referencing functions
        PrintWriter sx = new PrintWriter(new FileWriter(outDir + "/_string_xref.txt"));
        Map<String, Set<String>> strByFunc = new LinkedHashMap<>();
        ghidra.program.model.address.AddressSetView mem = currentProgram.getMemory().getLoadedAndInitializedAddressSet();
        ghidra.program.model.listing.DataIterator dit = currentProgram.getListing().getDefinedData(true);
        int scount = 0;
        while (dit.hasNext()) {
            Data d = dit.next();
            if (!d.hasStringValue()) continue;
            Object v = d.getValue();
            String s = (String) v;
            if (s == null || s.length() < 4) continue;
            if (s.length() > 120) s = s.substring(0, 120);
            s = s.replace('\n', ' ').replace('\t', ' ');
            Set<String> fnames = new TreeSet<String>();
            for (ReferenceIterator riter = currentProgram.getReferenceManager().getReferencesTo(d.getAddress()); riter.hasNext();) {
                Reference r = riter.next();
                Function fn = getFunctionContaining(r.getFromAddress());
                if (fn != null) fnames.add(fn.getName());
            }
            for (String k : fnames) {
                if (!strByFunc.containsKey(k)) strByFunc.put(k, new TreeSet<String>());
                strByFunc.get(k).add(s);
            }
            scount++;
        }
        for (Map.Entry<String, Set<String>> e : strByFunc.entrySet()) {
            for (String s : e.getValue()) {
                sx.println(e.getKey() + "	" + s);
            }
        }
        sx.close();

        // function index with address
        PrintWriter ix = new PrintWriter(new FileWriter(outDir + "/_func_index.txt"));
        for (Map.Entry<String, String> e : addr.entrySet()) {
            ix.println(e.getValue() + " " + e.getKey()
                + " calls=" + calls.get(e.getKey()).size());
        }
        ix.close();
        println("exported " + n + " functions to " + outDir);
    }
}
