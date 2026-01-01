import java.lang.instrument.ClassFileTransformer;
import java.lang.instrument.Instrumentation;
import java.security.ProtectionDomain;
import org.objectweb.asm.*;

public class QubicAgent {
    public static void premain(String AgentArgs, Instrumentation Inst) {
        System.out.println("[Qubic] Agent starting");
        
        Inst.addTransformer(new QubicTransformer());
        
        try {
            System.loadLibrary("qubic-core");
            QubicNative.Init();
            System.out.println("[Qubic] QubicNative loading");
        } catch (Exception e) {
            System.err.println("[Qubic] Failed to load native: " + e.getMessage());
            e.printStackTrace();
        }
    }
}