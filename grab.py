import os
from pathlib import Path
import fnmatch

output_file = "grab.txt"
excluded_dirs = {".git", "node_modules", ".vscode", ".idea", "__pycache__", ".vs", "build", "artifacts", "backup", "lib", "nlohmann"}
excluded_files = {"LICENSE", "README.md", "ReadMe.md", ".gitignore", ".gitattributes", "test.cpp", "resource.h", "CMakeLists.txt", output_file}
excluded_extensions = {
    ".rc", ".aps", ".sln", ".vcxproj", ".filters", ".user", ".lbc", ".bc", ".ll", ".obj", 
    ".lib", ".o", ".exe", ".dll", ".idb", ".lastbuildstate", ".recipe", ".tlog", ".pdb", 
    ".log", ".bat", ".txt", ".so", ".bin", ".pyc", ".pyo", ".class", ".jar", ".zip", 
    ".tar", ".gz", ".rar", ".7z", ".pdf", ".png", ".jpg", ".jpeg", ".gif", ".bmp", 
    ".ico", ".svg", ".mp3", ".mp4", ".avi", ".mov", ".wav", ".a", ".dylib", ".py", ".lasm", ".bat"
}

gitignore_patterns = []
if Path(".gitignore").exists():
    with open(".gitignore", "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):
                gitignore_patterns.append(line)

def matches_gitignore(file_path):
    path_str = str(file_path).replace("\\", "/")
    for pattern in gitignore_patterns:
        pattern = pattern.rstrip("/")
        if pattern.startswith("**/"):
            pattern = pattern[3:]
        if fnmatch.fnmatch(path_str, f"*{pattern}*") or fnmatch.fnmatch(path_str, f"*{pattern}"):
            return True
        if fnmatch.fnmatch(file_path.name, pattern):
            return True
        for parent in file_path.parents:
            if fnmatch.fnmatch(parent.name, pattern):
                return True
    return False

def should_skip(file_path):
    if file_path.name in excluded_files:
        return True
    if file_path.suffix.lower() in excluded_extensions:
        return True
    for parent in file_path.parents:
        if parent.name in excluded_dirs:
            return True
    if matches_gitignore(file_path):
        return True
    return False

with open(output_file, "w", encoding="utf-8", errors="ignore") as out:
    for file_path in Path(".").rglob("*"):
        if file_path.is_file() and not should_skip(file_path):
            print(f"Processing: {file_path}")
            out.write(f"{file_path}\n\n")
            try:
                with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                    out.write(f.read())
            except:
                pass
            out.write("\n\n================================\n\n")
while True:
	pass