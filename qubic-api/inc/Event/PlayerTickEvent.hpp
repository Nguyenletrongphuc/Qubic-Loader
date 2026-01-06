#ifndef EVENT_PLAYER_TICK_EVENT_HPP
#define EVENT_PLAYER_TICK_EVENT_HPP

#include <qubic-api/inc/Qubic.hpp>

namespace Qubic {
    struct PlayerTickEvent final{
    public:
        jobject player;
        jobject world;
        JNIEnv* env;
    };
}

#endif