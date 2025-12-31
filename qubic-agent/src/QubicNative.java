public class QubicNative {
    public static native void Init();
    
    /* this will be called during BuiltInRegistries.<clinit> */
    /* mc's item registries exist but shouldnt be frozen at this point */
    public static native void RegisterModItems();
}