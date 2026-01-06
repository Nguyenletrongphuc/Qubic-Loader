#ifndef EVENT_PLAYER_JOIN_EVENT_HPP
#define EVENT_PLAYER_JOIN_EVENT_HPP

#include <qubic-api/inc/Qubic.hpp>

namespace Qubic {
    struct PlayerJoinEvent final {
    public:
        jobject player;
        jobject world;
        JNIEnv* env;
    };
}

#endif