#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

void StripJarSignature(const std::string& JarPath) {
    std::string TempDir = "./temp_unsigned";
    std::string UnsignedJar = JarPath + ".unsigned";
    
    std::cout << "[Qubic Loader] Creating temp directory" << std::endl;
    fs::create_directories(TempDir);
    
    std::cout << "[Qubic Loader] Extracting JAR (this may take 30-60 seconds)" << std::endl;
    std::string ExtractCmd = "jar xf \"" + JarPath + "\" 2>nul";
    int ExtractResult = system(("cd /d \"" + fs::absolute(TempDir).string() + "\" && " + ExtractCmd).c_str());
    
    if (ExtractResult != 0) {
        std::cerr << "[Qubic Loader] Failed to extract JAR" << std::endl;
        fs::remove_all(TempDir);
        return;
    }
    
    std::cout << "[Qubic Loader] Removing signatures" << std::endl;
    std::string MetaInfPath = TempDir + "\\META-INF";
    if (fs::exists(MetaInfPath)) {
        for (const auto& Entry : fs::directory_iterator(MetaInfPath)) {
            if (Entry.is_regular_file()) {
                std::string Ext = Entry.path().extension().string();
                if (Ext == ".SF" || Ext == ".RSA" || Ext == ".DSA" || Ext == ".EC")
                    fs::remove(Entry.path());
            }
        }
    }
    
    std::cout << "[Qubic Loader] Repacking JAR (this may take 30-60 seconds)" << std::endl;
    std::string RepackCmd = "jar cf \"" + UnsignedJar + "\" -C \"" + TempDir + "\" . 2>nul";
    system(RepackCmd.c_str());
    
    std::cout << "[Qubic Loader] Cleaning up" << std::endl;
    fs::remove_all(TempDir);
    fs::remove(JarPath);
    fs::rename(UnsignedJar, JarPath);
    
    std::cout << "[Qubic Loader] Signature stripping complete!" << std::endl;
}

std::string BuildClasspath(const std::string& MinecraftDir, const std::string& VersionJson, const std::string& Version) {
    std::ifstream File(VersionJson);
    if (!File.is_open()) {
        std::cerr << "[Qubic Loader] Failed to open version JSON" << std::endl;
        return "";
    }
    
    nlohmann::json Json;
    File >> Json;
    File.close();
    
    std::vector<std::string> ClasspathEntries;
    
    if (Json.contains("libraries")) {
        for (const auto& Library : Json["libraries"]) {
            if (Library.contains("downloads") && Library["downloads"].contains("artifact")) {
                std::string Path = Library["downloads"]["artifact"]["path"];
                std::string FullPath = MinecraftDir + "\\libraries\\" + Path;
                
                std::replace(FullPath.begin(), FullPath.end(), '/', '\\');
                
                if (fs::exists(FullPath))
                    ClasspathEntries.push_back(FullPath);
            }
        }
    }
    
    std::string VersionJar = MinecraftDir + "\\versions\\" + Version + "\\" + Version + ".jar";
    
    if (!fs::exists(VersionJar)) {
        std::cerr << "[Qubic Loader] JAR not found: " << VersionJar << std::endl;
        return "";
    }
    
    std::string UnsignedMarker = VersionJar + ".unsigned_marker";
    if (!fs::exists(UnsignedMarker)) {
        std::cout << "[Qubic Loader] Stripping JAR signatures" << std::endl;
        StripJarSignature(VersionJar);
        std::ofstream Marker(UnsignedMarker);
        Marker.close();
    }
    
    ClasspathEntries.push_back(VersionJar);
    
    std::string Classpath;
    for (size_t I = 0; I < ClasspathEntries.size(); I++) {
        Classpath += ClasspathEntries[I];
        if (I < ClasspathEntries.size() - 1)
            Classpath += ";";
    }
    
    return Classpath;
}

std::string GetMainClass(const std::string& VersionJson) {
    std::ifstream File(VersionJson);
    if (!File.is_open())
        return "net.minecraft.client.Main";
    
    nlohmann::json Json;
    File >> Json;
    File.close();
    
    if (Json.contains("mainClass"))
        return Json["mainClass"];
    
    return "net.minecraft.client.Main";
}

bool WriteClasspathFile(const std::string& Classpath, const std::string& OutputPath) {
    fs::path path(OutputPath);

    if (path.has_parent_path())
        fs::create_directories(path.parent_path()); 

    std::ofstream file(path);
    if (!file.is_open())
        return false;

    file << Classpath;
    return true;
}

int main(int argc, char** argv, char** envp) {
    std::string JavaPath = "java";
    std::string AgentPath = "./java/qubic-agent.jar";
    std::string MinecraftDir = R"(C:\Users\side_\AppData\Roaming\.minecraft)";
    std::string JarVersion = "1.21.11_unobfuscated";
    std::string JsonVersion = "1_21_11_unobfuscated";
    std::string VersionJson = MinecraftDir + "\\versions\\" + JsonVersion + "\\" + JsonVersion + ".json";
    std::string LibPath = ".";
    std::string ClasspathFile = "./logs/classpath.txt";
    
    /* build classpath first */
    std::cout << "[Qubic Loader] Building classpath" << std::endl;
    std::string Classpath = BuildClasspath(MinecraftDir, VersionJson, JarVersion);
    
    if (Classpath.empty()) {
        std::cerr << "[Qubic Loader] Failed to build classpath" << std::endl;
        return 1;
    }
    
    std::cout << "[Qubic Loader] Writing classpath file" << std::endl;
    if (!WriteClasspathFile(Classpath, ClasspathFile)) {
        std::cerr << "[Qubic Loader] Failed to write classpath file" << std::endl;
        return 1;
    }
    std::cout << "[Qubic Loader] Classpath file written: " << ClasspathFile << std::endl;
    
    /* if --generate-classpath-only flag is passed then exit here */
    if (argc > 1 && std::string(argv[1]) == "--generate-classpath-only") {
        std::cout << "[Qubic Loader] Classpath generation complete, exiting" << std::endl;
        return 0;
    }
    
    std::string MainClass = GetMainClass(VersionJson);

    std::string GameArgs = " --username Player"
                          " --version " + JarVersion +
                          " --gameDir \"" + MinecraftDir + "\""
                          " --assetsDir \"" + MinecraftDir + "\\assets\""
                          " --assetIndex 1.21"
                          " --accessToken 0"
                          " --userType legacy";

    std::string Command = JavaPath + 
                        " -Djava.library.path=" + LibPath +
                        " -Xbootclasspath/a:java/asm-9.6.jar" +
                        " -cp @" + ClasspathFile +
                        " -javaagent:" + AgentPath +
                        " " + MainClass +
                        GameArgs;
    
    std::cout << "[Qubic Loader] Executing: " << Command << std::endl;
    
    int Result = system(Command.c_str());
    fs::remove(ClasspathFile);
    
    #ifndef _WIN32
    if (WIFEXITED(Result))
        return WEXITSTATUS(Result);
    #endif
    
    return Result;
}