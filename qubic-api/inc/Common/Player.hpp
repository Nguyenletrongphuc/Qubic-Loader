#ifndef QUBIC_PLAYER_HPP
#define QUBIC_PLAYER_HPP

#include <jni.h>
#include <string>
#include <optional>

namespace Qubic {
    struct Vec3;

    class Player {
    private:
        jobject player_obj;
        JNIEnv* env;
        
        jclass player_class;
    public:
        Player(jobject player, JNIEnv* env);
        ~Player();
        
        /* get players name */
        std::string GetName();

        /* get position */
        Qubic::Vec3 GetPosition();

        /* get players health */
        float GetHealth();
        
        /* get max health */
        float GetMaxHealth();
        
        /* set health */
        void SetHealth(float health);
        
        /* check abilities */
        bool IsCreative();
        
        /* check if sneaking */
        bool IsSneaking();
        
        /* check if sprinting */
        bool IsSprinting();
        
        /* send message */
        void SendMessage(const std::string& msg);
        
        /* get xp level */
        int GetExperienceLevel();
        
        /* set xp level */
        void SetExperienceLevel(int level);
        
        /* get food level */
        int GetFoodLevel();
        
        /* check on ground */
        bool IsOnGround();
        
        /* get raw object */
        jobject GetJobject() const {
            return this->player_obj;
        }
        
        /* get jni environment */
        JNIEnv* GetEnv() const {
            return this->env;
        }
    };
}

#endif