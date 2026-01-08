#ifndef EVENT_MOUSE_BUTTON_EVENT_HPP
#define EVENT_MOUSE_BUTTON_EVENT_HPP

#include <qubic-api/inc/Qubic.hpp>

// GLFW Key codes
#define QUBIC_MOUSE_BUTTON_1      0
#define QUBIC_MOUSE_BUTTON_2      1
#define QUBIC_MOUSE_BUTTON_3      2
#define QUBIC_MOUSE_BUTTON_4      3
#define QUBIC_MOUSE_BUTTON_5      4
#define QUBIC_MOUSE_BUTTON_6      5
#define QUBIC_MOUSE_BUTTON_7      6
#define QUBIC_MOUSE_BUTTON_8      7
#define QUBIC_MOUSE_BUTTON_LEFT   QUBIC_MOUSE_BUTTON_1
#define QUBIC_MOUSE_BUTTON_RIGHT  QUBIC_MOUSE_BUTTON_2
#define QUBIC_MOUSE_BUTTON_MIDDLE QUBIC_MOUSE_BUTTON_3

namespace Qubic {
    struct MouseInputEvent final {
    public:
        int button;    /* QUBIC_MOUSE_BUTTON_* */
        int action;    /* QUBIC_PRESS, QUBIC_RELEASE, or QUBIC_REPEAT */
        int mods;      /* modifier keys (shift, ctrl, alt) */
        JNIEnv* env;
        
        bool IsPress() const { return action == QUBIC_PRESS; }
        bool IsRelease() const { return action == QUBIC_RELEASE; }
        bool IsRepeat() const { return action == QUBIC_REPEAT; }
        
        bool HasShift() const { return (mods & QUBIC_MOD_SHIFT) != 0; }
        bool HasCtrl() const { return (mods & QUBIC_MOD_CONTROL) != 0; }
        bool HasAlt() const { return (mods & QUBIC_MOD_ALT) != 0; }
        
        bool IsLeftButton() const { return button == QUBIC_MOUSE_BUTTON_LEFT; }
        bool IsRightButton() const { return button == QUBIC_MOUSE_BUTTON_RIGHT; }
        bool IsMiddleButton() const { return button == QUBIC_MOUSE_BUTTON_MIDDLE; }
    };
}

#endif