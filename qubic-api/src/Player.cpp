#include <inc/Common/Player.hpp>
#include <inc/Qubic.hpp>

Qubic::Player::Player(jobject player, JNIEnv* env) : player_obj(player), env(env) {
    player_class = env->GetObjectClass(player_obj);
}

Qubic::Player::~Player() {
    if (player_class)
        env->DeleteLocalRef(player_class);
}

std::string Qubic::Player::GetName() {
    /* get the nameAndId() method which returns NameAndId object */
    jmethodID getNameAndIdMethod = env->GetMethodID(
        player_class, 
        "nameAndId", 
        "()Lnet/minecraft/server/players/NameAndId;"
    );
    
    if (!getNameAndIdMethod)
        return "Unknown";
    
    jobject nameAndId = env->CallObjectMethod(player_obj, getNameAndIdMethod);
    if (!nameAndId)
        return "Unknown";
    
    /* get the NameAndId class and its name() method */
    jclass nameAndIdClass = env->GetObjectClass(nameAndId);
    jmethodID getNameMethod = env->GetMethodID(
        nameAndIdClass, 
        "name", 
        "()Ljava/lang/String;"
    );
    
    if (!getNameMethod) {
        env->DeleteLocalRef(nameAndIdClass);
        env->DeleteLocalRef(nameAndId);
        return "Unknown";
    }
    
    jstring nameStr = (jstring)env->CallObjectMethod(nameAndId, getNameMethod);
    if (!nameStr) {
        env->DeleteLocalRef(nameAndIdClass);
        env->DeleteLocalRef(nameAndId);
        return "Unknown";
    }

    const char* name = env->GetStringUTFChars(nameStr, nullptr);
    std::string result(name);
    
    env->ReleaseStringUTFChars(nameStr, name);
    env->DeleteLocalRef(nameStr);
    env->DeleteLocalRef(nameAndIdClass);
    env->DeleteLocalRef(nameAndId);
    
    return result;
}

Qubic::Vec3 Qubic::Player::GetPosition() {
    jmethodID getXMethod = env->GetMethodID(player_class, "getX", "()D");
    jmethodID getYMethod = env->GetMethodID(player_class, "getY", "()D");
    jmethodID getZMethod = env->GetMethodID(player_class, "getZ", "()D");
    
    Qubic::Vec3 pos;
    pos.x = env->CallDoubleMethod(player_obj, getXMethod);
    pos.y = env->CallDoubleMethod(player_obj, getYMethod);
    pos.z = env->CallDoubleMethod(player_obj, getZMethod);
    
    return pos;
}

float Qubic::Player::GetHealth() {
    jmethodID getHealthMethod = env->GetMethodID(player_class, "getHealth", "()F");
    return env->CallFloatMethod(player_obj, getHealthMethod);
}

float Qubic::Player::GetMaxHealth() {
    jmethodID getMaxHealthMethod = env->GetMethodID(player_class, "getMaxHealth", "()F");
    return env->CallFloatMethod(player_obj, getMaxHealthMethod);
}

void Qubic::Player::SetHealth(float health) {
    jmethodID setHealthMethod = env->GetMethodID(player_class, "setHealth", "(F)V");
    env->CallVoidMethod(player_obj, setHealthMethod, health);
}

bool Qubic::Player::IsCreative() {
    jmethodID getAbilitiesMethod = env->GetMethodID(
        player_class, 
        "getAbilities", 
        "()Lnet/minecraft/world/entity/player/Abilities;"
    );
    
    jobject abilities = env->CallObjectMethod(player_obj, getAbilitiesMethod);
    if (!abilities) return false;
    
    jclass abilitiesClass = env->GetObjectClass(abilities);
    jfieldID creativeField = env->GetFieldID(abilitiesClass, "instabuild", "Z");
    jboolean isCreative = env->GetBooleanField(abilities, creativeField);
    
    env->DeleteLocalRef(abilitiesClass);
    env->DeleteLocalRef(abilities);
    
    return isCreative;
}

bool Qubic::Player::IsSneaking() {
    jmethodID isSneakingMethod = env->GetMethodID(player_class, "isCrouching", "()Z");
    return env->CallBooleanMethod(player_obj, isSneakingMethod);
}

bool Qubic::Player::IsSprinting() {
    jmethodID isSprintingMethod = env->GetMethodID(player_class, "isSprinting", "()Z");
    return env->CallBooleanMethod(player_obj, isSprintingMethod);
}

void Qubic::Player::SendMessage(const std::string& msg) {
    /* get Component.literal() method */
    jclass componentClass = env->FindClass("net/minecraft/network/chat/Component");
    jmethodID literalMethod = env->GetStaticMethodID(
        componentClass, 
        "literal", 
        "(Ljava/lang/String;)Lnet/minecraft/network/chat/MutableComponent;"
    );
    
    jstring msgStr = env->NewStringUTF(msg.c_str());
    jobject component = env->CallStaticObjectMethod(componentClass, literalMethod, msgStr);
    
    /* call player.sendSystemMessage() */
    jmethodID sendMessageMethod = env->GetMethodID(
        player_class, 
        "sendSystemMessage", 
        "(Lnet/minecraft/network/chat/Component;)V"
    );
    
    env->CallVoidMethod(player_obj, sendMessageMethod, component);
    
    env->DeleteLocalRef(msgStr);
    env->DeleteLocalRef(component);
    env->DeleteLocalRef(componentClass);
}

int Qubic::Player::GetExperienceLevel() {
    jfieldID expLevelField = env->GetFieldID(player_class, "experienceLevel", "I");
    return env->GetIntField(player_obj, expLevelField);
}

void Qubic::Player::SetExperienceLevel(int level) {
    jfieldID expLevelField = env->GetFieldID(player_class, "experienceLevel", "I");
    env->SetIntField(player_obj, expLevelField, level);
}

int Qubic::Player::GetFoodLevel() {
    jmethodID getFoodDataMethod = env->GetMethodID(
        player_class, 
        "getFoodData", 
        "()Lnet/minecraft/world/food/FoodData;"
    );
    
    jobject foodData = env->CallObjectMethod(player_obj, getFoodDataMethod);
    jclass foodDataClass = env->GetObjectClass(foodData);
    jmethodID getFoodLevelMethod = env->GetMethodID(foodDataClass, "getFoodLevel", "()I");
    
    int foodLevel = env->CallIntMethod(foodData, getFoodLevelMethod);
    
    env->DeleteLocalRef(foodDataClass);
    env->DeleteLocalRef(foodData);
    
    return foodLevel;
}

bool Qubic::Player::IsOnGround() {
    jmethodID isOnGroundMethod = env->GetMethodID(player_class, "onGround", "()Z");
    return env->CallBooleanMethod(player_obj, isOnGroundMethod);
}