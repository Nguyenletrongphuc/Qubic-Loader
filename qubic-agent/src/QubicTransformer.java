import java.lang.instrument.ClassFileTransformer;
import java.security.ProtectionDomain;
import org.objectweb.asm.*;

public class QubicTransformer implements ClassFileTransformer {
    @Override
    public byte[] transform(ClassLoader Loader, String ClassName, Class<?> ClassBeingRedefined, ProtectionDomain ProtectionDomain, byte[] ClassFileBuffer) {
        
        /* hook into title screen class */
        if (ClassName.equals("net/minecraft/client/gui/screens/TitleScreen")) {
            System.out.println("[Qubic] Transforming TitleScreen!");
            return TransformTitleScreen(ClassFileBuffer);
        }
        
        /* hook into glfw window class */
        if (ClassName.equals("com/mojang/blaze3d/platform/Window")) {
            System.out.println("[Qubic] Transforming Window class!");
            return TransformWindow(ClassFileBuffer);
        }
        
        /* hook into KeyboardHandler to intercept key events */
        if (ClassName.equals("net/minecraft/client/KeyboardHandler")) {
            System.out.println("[Qubic] Transforming KeyboardHandler for key input!");
            return TransformKeyboardHandler(ClassFileBuffer);
        }
        
        /* hook into item class */
        if (ClassName.equals("net/minecraft/world/item/Items")) {
            System.out.println("[Qubic] Transforming Items for mod item registration!");
            return TransformItems(ClassFileBuffer);
        }
        
        /* hook into pack repository to auto-load qubic resource packs */
        if (ClassName.equals("net/minecraft/server/packs/repository/PackRepository")) {
            System.out.println("[Qubic] Transforming PackRepository for auto-loading resource packs!");
            return TransformPackRepository(ClassFileBuffer);
        }

        /* hook into playerlist */
        if (ClassName.equals("net/minecraft/server/players/PlayerList")) {
            System.out.println("[Qubic] Transforming PlayerList for join events!");
            return TransformPlayerList(ClassFileBuffer);
        }

        /* hook into server player tick */
        if (ClassName.equals("net/minecraft/server/level/ServerPlayer")) {
            System.out.println("[Qubic] Transforming ServerPlayer for tick events!");
            return TransformServerPlayer(ClassFileBuffer);
        }
        
        /* hook into server tick */
        if (ClassName.equals("net/minecraft/server/MinecraftServer")) {
            System.out.println("[Qubic] Transforming MinecraftServer for mod tick!");
            return TransformMinecraftServer(ClassFileBuffer);
        }
        
        return null;
    }
    
    private byte[] TransformKeyboardHandler(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                /* hook into the keyPress() method uhh, signature: (JILnet/minecraft/client/input/KeyEvent;)V */
                if (Name.equals("keyPress") && Descriptor.equals("(JILnet/minecraft/client/input/KeyEvent;)V")) {
                    System.out.println("[Qubic] Found KeyboardHandler.keyPress, injecting key event!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitCode() {
                            super.visitCode();
                            
                            /* stack indices: this=0, long handle=1-2, int action=3, KeyEvent event=4 */
                            
                            /* get key from KeyEvent.key() */
                            super.visitVarInsn(Opcodes.ALOAD, 4);  // Load KeyEvent
                            super.visitMethodInsn(
                                Opcodes.INVOKEVIRTUAL,
                                "net/minecraft/client/input/KeyEvent",
                                "key",
                                "()I",
                                false
                            );
                            
                            /* get scancode from KeyEvent.scancode() */
                            super.visitVarInsn(Opcodes.ALOAD, 4);  // Load KeyEvent
                            super.visitMethodInsn(
                                Opcodes.INVOKEVIRTUAL,
                                "net/minecraft/client/input/KeyEvent",
                                "scancode",
                                "()I",
                                false
                            );
                            
                            /* get action from parameter */
                            super.visitVarInsn(Opcodes.ILOAD, 3);
                            
                            /* get modifiers from KeyEvent.modifiers() */
                            super.visitVarInsn(Opcodes.ALOAD, 4);  // Load KeyEvent
                            super.visitMethodInsn(
                                Opcodes.INVOKEVIRTUAL,
                                "net/minecraft/client/input/KeyEvent",
                                "modifiers",
                                "()I",
                                false
                            );
                            
                            /* call our native handler: QubicNative.OnKeyInput(key, scancode, action, mods) */
                            super.visitMethodInsn(
                                Opcodes.INVOKESTATIC,
                                "QubicNative",
                                "OnKeyInput",
                                "(IIII)V",
                                false
                            );
                        }
                    };
                }
                
                return Mv;
            }
        };
        
        Reader.accept(Visitor, 0);
        return Writer.toByteArray();
    }
    
    private byte[] TransformPackRepository(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                /* hook into reload() method */
                if (Name.equals("reload")) {
                    System.out.println("[Qubic] Found PackRepository.reload(), injecting pack loader!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitInsn(int Opcode) {
                            /* right before returning, inject pack loading */
                            if (Opcode == Opcodes.RETURN) {
                                System.out.println("[Qubic] Injecting QubicPackLoader.InjectQubicPack call");
                                
                                /* load 'this' (PackRepository instance) */
                                super.visitVarInsn(Opcodes.ALOAD, 0);
                                
                                /* call QubicPackLoader.InjectQubicPack(this) */
                                super.visitMethodInsn(
                                    Opcodes.INVOKESTATIC,
                                    "QubicPackLoader",
                                    "InjectQubicPack",
                                    "(Lnet/minecraft/server/packs/repository/PackRepository;)V",
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
                    System.out.println("[Qubic] Found Items.<clinit>, injecting mod loader call");
                    
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
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
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
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
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

    private byte[] TransformPlayerList(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                if (Name.equals("placeNewPlayer")) {
                    System.out.println("[Qubic] Found placeNewPlayer, injecting join event!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitInsn(int Opcode) {
                            /* inject code at the end of player join */
                            if (Opcode == Opcodes.RETURN) {
                                super.visitVarInsn(Opcodes.ALOAD, 2);  /* load player */
                                super.visitVarInsn(Opcodes.ALOAD, 2);  /* load player again */
                                super.visitMethodInsn(Opcodes.INVOKEVIRTUAL, 
                                    "net/minecraft/server/level/ServerPlayer", 
                                    "level", 
                                    "()Lnet/minecraft/world/level/Level;", 
                                    false
                                );
                                
                                super.visitMethodInsn(Opcodes.INVOKESTATIC,
                                    "QubicNative",
                                    "OnPlayerJoin",
                                    "(Ljava/lang/Object;Ljava/lang/Object;)V",
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

    private byte[] TransformServerPlayer(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                if (Name.equals("tick") && Descriptor.equals("()V")) {
                    System.out.println("[Qubic] Found ServerPlayer.tick(), injecting player tick event!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitCode() {
                            super.visitCode();
                            
                            /* at the start of tick() call: QubicNative.OnPlayerTick(this, this.level()) */
                            super.visitVarInsn(Opcodes.ALOAD, 0);  /* load player */
                            super.visitVarInsn(Opcodes.ALOAD, 0);  /* load player again */
                            super.visitMethodInsn(
                                Opcodes.INVOKEVIRTUAL,
                                "net/minecraft/server/level/ServerPlayer",
                                "level",
                                "()Lnet/minecraft/server/level/ServerLevel;",
                                false
                            );
                            
                            super.visitMethodInsn(
                                Opcodes.INVOKESTATIC,
                                "QubicNative",
                                "OnPlayerTick",
                                "(Ljava/lang/Object;Ljava/lang/Object;)V",
                                false
                            );
                        }
                    };
                }
                
                return Mv;
            }
        };
        
        Reader.accept(Visitor, 0);
        return Writer.toByteArray();
    }

    private byte[] TransformMinecraftServer(byte[] ClassBytes) {
        ClassReader Reader = new ClassReader(ClassBytes);
        ClassWriter Writer = new ClassWriter(Reader, ClassWriter.COMPUTE_MAXS | ClassWriter.COMPUTE_FRAMES);
        
        ClassVisitor Visitor = new ClassVisitor(Opcodes.ASM9, Writer) {
            @Override
            public MethodVisitor visitMethod(int Access, String Name, String Descriptor, String Signature, String[] Exceptions) {
                MethodVisitor Mv = super.visitMethod(Access, Name, Descriptor, Signature, Exceptions);
                
                if (Name.equals("tickServer") || Name.equals("tick")) {
                    System.out.println("[Qubic] Found MinecraftServer tick method, injecting mod tick!");
                    
                    return new MethodVisitor(Opcodes.ASM9, Mv) {
                        @Override
                        public void visitCode() {
                            super.visitCode();
                            
                            /* call QubicNative.OnServerTick() at start of tick */
                            super.visitMethodInsn(
                                Opcodes.INVOKESTATIC,
                                "QubicNative",
                                "OnServerTick",
                                "()V",
                                false
                            );
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