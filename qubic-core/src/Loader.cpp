
#include <qubic-api/inc/Qubic.hpp>
#include <src/core/Binary.h>
#include <src/Loader.hpp>

#include <unordered_map>
#include <include/miniz_cpp.hpp>

struct CentralAssetManager final {
public:
    std::unordered_map<std::string, std::vector<uint8_t>> Assets;
    fs::path ResourcePackPath;
    
    CentralAssetManager() {
#ifdef _WIN32
        char* tempDir = nullptr;
        size_t len = 0;
        _dupenv_s(&tempDir, &len, "TEMP");
        
        ResourcePackPath = fs::path(tempDir) / "qubic_loader_resources";

        free(tempDir);
#else
        ResourcePackPath = fs::temp_directory_path() / "qubic_loader_resources";
#endif
        
        if (fs::exists(ResourcePackPath))
            fs::remove_all(ResourcePackPath);
        
        fs::create_directories(ResourcePackPath);
        this->CreatePackMeta();
    }

    ~CentralAssetManager() {
        if (fs::exists(ResourcePackPath))
            fs::remove_all(ResourcePackPath);
    }
    
    void CreatePackMeta() {
        fs::path MetaPath = ResourcePackPath / "pack.mcmeta";
        std::ofstream MetaFile(MetaPath);
        
        if (MetaFile.is_open()) {
            MetaFile << R"(
        {
          "pack": {
            "pack_format": 48,
            "description": "Qubic Loader Mod Resources"
          }
        })";
            MetaFile.close();
            printf("[Qubic] Created pack.mcmeta at: %s\n", MetaPath.string().c_str());
        }
    }
    
    void AddModAssets(const std::string& ModId, const fs::path& PackPath) {
        if (!fs::exists(PackPath)) {
            printf("[Qubic] Pack file not found: %s\n", PackPath.string().c_str());
            return;
        }
        
        try {
            miniz_cpp::zip_file Zip(PackPath.string());
            auto FileList = Zip.infolist();
            
            printf("[Qubic] Extracting assets for mod: %s\n", ModId.c_str());
            
            for (const auto& Info : FileList) {
                std::string Filename = Info.filename;
                if (Filename.back() == '/') continue;
                
                auto Data = Zip.read(Info);
                std::vector<uint8_t> FileData(Data.begin(), Data.end());
                
                Assets[Filename] = FileData;
                
                fs::path OutputPath = ResourcePackPath / Filename;
                fs::create_directories(OutputPath.parent_path());
                
                std::ofstream OutFile(OutputPath, std::ios::binary);
                if (OutFile.is_open()) {
                    OutFile.write(reinterpret_cast<const char*>(FileData.data()), FileData.size());
                    OutFile.close();
                    printf("[Qubic] Extracted: %s (%zu bytes)\n", Filename.c_str(), FileData.size());
                }
            }
            
            printf("[Qubic] Successfully extracted %zu assets for %s\n", FileList.size(), ModId.c_str());
        } catch (const std::exception& E) {
            printf("[Qubic] Failed to load pack for %s: %s\n", ModId.c_str(), E.what());
        }
    }
    
    const std::vector<uint8_t>* GetAsset(const std::string& Path) {
        auto it = Assets.find(Path);
        if (it != Assets.end())
            return &it->second;
        return nullptr;
    }
    
    bool HasAsset(const std::string& Path) {
        return Assets.find(Path) != Assets.end();
    }
    
    std::string GetResourcePackPath() const {
        return ResourcePackPath.string();
    }
};
static CentralAssetManager* gAssetManager = nullptr;

bool Qubic::ModLoader::LoadMod(const fs::path& Directory) {
    void* Handle = LoadBin(Directory.string().c_str());
    if (!Handle) return false;

    ModLoadFunc mod_load = (ModLoadFunc)GET_FUNC(Handle, "mod_load");
    if (!mod_load) {
        CloseBin(Handle);
        return false;
    }

    Qubic::ModState* State = new Qubic::ModState();
    State->jvm_ptr = jvm_ptr;
    State->jni_env = jni_env;
    State->jvmti_env = jvmti_env;
    State->mod_id = nullptr;
    State->data_ptr = nullptr;

    Qubic::ModState* Result = mod_load(State);
    if (!Result || !Result->mod_id) {
        
        delete State;

        CloseBin(Handle);
        return false;
    }
    
    for (const auto& Mod : this->ModVector) {
        if (strcmp(Mod.State.mod_id, Result->mod_id) == 0) {
            printf("[Qubic] ERROR: Duplicate mod_id detected: %s\n", Result->mod_id);
            
            delete State;

            CloseBin(Handle);
            return false;
        }
    }

    fs::path PackPath = Directory.parent_path() / (Directory.stem().string() + ".pck");
    /* create if not available */
    if (!gAssetManager)
        gAssetManager = new CentralAssetManager();
    
    /* load mod assets into the global asset manager */
    if (fs::exists(PackPath))
        gAssetManager->AddModAssets(Result->mod_id, PackPath);
    else
        printf("[Qubic] WARNING: No pack file found for mod: %s\n", Result->mod_id);
    
    this->ModVector.push_back({Result->mod_id, *Result, Handle});
    printf("[Qubic] DEBUG: Loaded ModId: %s\n", Result->mod_id);
    return true;
}

extern "C" {
    DLL_EXPORT JNIEXPORT jbyteArray JNICALL Java_QubicNative_GetModResource(JNIEnv* env, jclass clazz, jstring jpath) {
        if (!gAssetManager) return nullptr;
        
        const char* Path = env->GetStringUTFChars(jpath, nullptr);
        const std::vector<uint8_t>* Data = gAssetManager->GetAsset(Path);
        env->ReleaseStringUTFChars(jpath, Path);
        
        if (!Data) return nullptr;
        
        jbyteArray Result = env->NewByteArray(Data->size());
        env->SetByteArrayRegion(Result, 0, Data->size(), 
                            reinterpret_cast<const jbyte*>(Data->data()));
        
        return Result;
    }

    DLL_EXPORT JNIEXPORT jboolean JNICALL Java_QubicNative_HasModResource(JNIEnv* env, jclass clazz, jstring jpath) {
        if (!gAssetManager) return false;
        
        const char* Path = env->GetStringUTFChars(jpath, nullptr);
        bool Exists = gAssetManager->HasAsset(Path);
        env->ReleaseStringUTFChars(jpath, Path);
        
        return Exists;
    }
    
    DLL_EXPORT JNIEXPORT jstring JNICALL Java_QubicNative_GetResourcePackPath(JNIEnv* env, jclass clazz) {
        if (!gAssetManager) return nullptr;
        
        std::string Path = gAssetManager->GetResourcePackPath();
        return env->NewStringUTF(Path.c_str());
    }
}