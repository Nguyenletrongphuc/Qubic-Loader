#ifndef EVENT_MOUSE_SCROLL_EVENT_HPP
#define EVENT_MOUSE_SCROLL_EVENT_HPP

#include <qubic-api/inc/Qubic.hpp>

namespace Qubic {
    struct MouseScrollInputEvent final {
    public:
        double horizontal;  /* horizontal scroll (usually 0) */
        double vertical;    /* vertical scroll (positive = up, negative = down) */
        JNIEnv* env;
        
        bool IsScrollUp() const { return vertical > 0; }
        bool IsScrollDown() const { return vertical < 0; }
        
        bool HasHorizontalScroll() const { return horizontal != 0; }
    };
}

#endif