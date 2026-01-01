public class QubicNative {
    public static native void Init();
    
    /* this will be called during BuiltInRegistries.<clinit> */
    /* mc's item registries exist but shouldnt be frozen at this point */
    public static native void RegisterModItems();
    
    /* resource access methods */
    public static native String GetResourcePackPath();
    public static native byte[] GetModResource(String path);
    public static native boolean HasModResource(String path);
}