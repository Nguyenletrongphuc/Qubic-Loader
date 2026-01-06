import org.lwjgl.glfw.GLFW;
import org.lwjgl.glfw.GLFWKeyCallback;
import com.mojang.blaze3d.platform.Window;

public class QubicInput {
    private static GLFWKeyCallback previousCallback = null;
    
    // Changed parameter type from Object to Window
    public static void setupKeyCallback(Window window) {
        try {
            /* get GLFW window handle */
            java.lang.reflect.Field windowField = window.getClass().getDeclaredField("window");
            windowField.setAccessible(true);
            long windowHandle = windowField.getLong(window);
            
            /* set our key callback that chains to the original */
            previousCallback = GLFW.glfwSetKeyCallback(windowHandle, (win, key, scancode, action, mods) -> {
                /* call handler */
                QubicNative.OnKeyInput(key, scancode, action, mods);
                
                if (previousCallback != null) {
                    previousCallback.invoke(win, key, scancode, action, mods);
                }
            });
            
            System.out.println("[Qubic] Key input callback installed!");
            
        } catch (Exception e) {
            System.err.println("[Qubic] Failed to setup key callback: " + e.getMessage());
            e.printStackTrace();
        }
    }
}