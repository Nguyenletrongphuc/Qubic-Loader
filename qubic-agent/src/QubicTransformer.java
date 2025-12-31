import java.lang.instrument.ClassFileTransformer;
import java.security.ProtectionDomain;
import org.objectweb.asm.*;

public class QubicTransformer implements ClassFileTransformer {
    @Override
    public byte[] transform(ClassLoader Loader, String ClassName, Class<?> ClassBeingRedefined, ProtectionDomain ProtectionDomain, byte[] ClassFileBuffer) {
        if (ClassName.equals("net/minecraft/client/gui/screens/TitleScreen")) {
            System.out.println("[Qubic] Transforming TitleScreen!");
            return TransformTitleScreen(ClassFileBuffer);
        }
        
        if (ClassName.equals("com/mojang/blaze3d/platform/Window")) {
            System.out.println("[Qubic] Transforming Window class!");
            return TransformWindow(ClassFileBuffer);
        }
        
        /* hook into 'items class static initializer; this is where vanilla items are made */
        if (ClassName.equals("net/minecraft/world/item/Items")) {
            System.out.println("[Qubic] Transforming Items for mod item registration!");
            return TransformItems(ClassFileBuffer);
        }
        
        return null;
    }
    
    private byte[] TransformItems(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor,
                                            String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                /* hook into the static initializer <clinit> */
                if (Name.equals("<clinit>")) {
                    System.out.println("[Qubic] Found Items.<clinit>, injecting mod loader call at the END!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitInsn(int Opcode) {
                            /* right before returning, inject the mod call */
                            if (Opcode == Opcodes.RETURN) {
                                System.out.println("[Qubic] Injecting RegisterModItems call at end of Items.<clinit>");
                                
                                /* call QubicNative.RegisterModItems() */
                                super.visitMethodInsn(
                                    Opcodes.INVOKESTATIC, 
                                    "QubicNative", 
                                    "RegisterModItems", 
                                    "()V", 
                                    false
                                );
                            }
                            super.visitInsn(Opcode);
                        }
                    };
                }
                
                return Mv;
            }
        };
        
        Reader.accept(Visitor, 0);
        return Writer.toByteArray();
    }
    
    private byte[] TransformWindow(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor,
                                            String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                return new MethodVisitor(Opcodes.ASM9, Mv) {
                    @Override
                    public void visitLdcInsn(Object Value) {
                        if (Value instanceof String) {
                            String Str = (String) Value;
                            if (Str.contains("Minecraft") || Str.contains("minecraft")) {
                                super.visitLdcInsn("Minecraft 1.21.11 (Qubic Loader)");
                                return;
                            }
                        }
                        super.visitLdcInsn(Value);
                    }
                    
                    @Override
                    public void visitMethodInsn(int Opcode, String Owner, String Name, String Descriptor, boolean IsInterface) {
                        if (Name.equals("glfwSetWindowTitle")) {
                            super.visitInsn(Opcodes.POP);
                            super.visitLdcInsn("Minecraft 1.21.11 (Qubic Loader)");
                        }
                        super.visitMethodInsn(Opcode, Owner, Name, Descriptor, IsInterface);
                    }
                };
            }
        };
        
        Reader.accept(Visitor, 0);
        return Writer.toByteArray();
    }
    
    private byte[] TransformTitleScreen(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor,
                                            String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                if (Name.equals("render")) {
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        private int AStoreCount = 0;
                        
                        @Override
                        public void visitVarInsn(int Opcode, int Var) {
                            if (Opcode == Opcodes.ASTORE && Var == 6) {
                                AStoreCount++;
                                if (AStoreCount == 4) {
                                    super.visitInsn(Opcodes.POP);
                                    super.visitLdcInsn("Minecraft 1.21.11 (Qubic Loader) ");
                                }
                            }
                            super.visitVarInsn(Opcode, Var);
                        }
                    };
                }
                
                return Mv;
            }
        };
        
        Reader.accept(Visitor, 0);
        return Writer.toByteArray();
    }
}