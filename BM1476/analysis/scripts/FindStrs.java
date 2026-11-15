import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import java.io.*;
public class FindStrs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : ".";
        PrintWriter pw = new PrintWriter(new FileWriter(outDir + "/strings_map.txt"));
        Listing listing = currentProgram.getListing();
        DataIterator di = listing.getDefinedData(true);
        int n=0;
        while (di.hasNext()) {
            Data dat = di.next();
            if (dat.hasStringValue()) {
                String v = dat.getValue().toString();
                ReferenceIterator ri = currentProgram.getReferenceManager().getReferencesTo(dat.getAddress());
                StringBuilder refs = new StringBuilder();
                while (ri.hasNext()) {
                    Reference r = ri.next();
                    refs.append(r.getFromAddress()).append(" ");
                }
                pw.println(dat.getAddress() + " | " + v.replace("\n","\n").substring(0, Math.min(90, v.length())) + " | " + refs);
                n++;
            }
        }
        pw.close();
        println("strings dumped: " + n);
    }
}
