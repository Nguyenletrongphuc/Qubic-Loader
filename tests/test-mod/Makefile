MOD_NAME := test-mod


MAKEFLAGS += -j --no-print-directory

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    .SHELLFLAGS := /c
    RM := del /q
    RMDIR := rd /s /q
    MKDIR := mkdir
    PATH_SEP := \\
    DETECTED_OS := Windows
    DLL_EXT := .dll
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
        DLL_EXT := .so
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        DLL_EXT := .dylib
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    PATH_SEP := /
endif

SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj
OUTPUT := $(BUILD_DIR)/$(MOD_NAME)$(DLL_EXT)

CC := clang
CXX := clang++

COMMON_FLAGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wunused -Wformat=2 -Wno-language-extension-token -Wno-unused-parameter

JAVA_HOME := C:\Program Files\Java\jdk-21
QUBIC_API_DIR := ../../qubic-api
QUBIC_LIB := ./qubic-api.lib

ifeq ($(DETECTED_OS),Windows)
    INC_DIRS := src include ./ ../../ $(QUBIC_API_DIR)
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32"
    CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -m64 $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -m64 $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    LDFLAGS := -m64 -shared -fuse-ld=lld-link "$(QUBIC_LIB)"
else ifeq ($(DETECTED_OS),Linux)
    INC_DIRS := src include ./ ../../ $(QUBIC_API_DIR)
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"
    CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    LDFLAGS := -shared $(QUBIC_LIB)
else ifeq ($(DETECTED_OS),macOS)
    INC_DIRS := src include ./ ../../ $(QUBIC_API_DIR)
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/darwin"
    CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    LDFLAGS := -shared $(QUBIC_LIB)
endif

RWILDCARD = $(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$d,$2) $(filter $(subst *,%,$2),$d))

C_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.c)
CPP_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.cpp)

C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)

all: $(OUTPUT)
	@echo [MOD] Built $(MOD_NAME)$(DLL_EXT)

$(OUTPUT): $(OBJECTS) $(QUBIC_LIB)
	@echo [LD] $(notdir $@)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(BUILD_DIR))" $(MKDIR) "$(subst /,\,$(BUILD_DIR))"
else
	@$(MKDIR) $(BUILD_DIR)
endif
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo [CC] $(notdir $<)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(dir $@))" $(MKDIR) "$(subst /,\,$(dir $@))"
else
	@$(MKDIR) $(dir $@)
endif
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CXX] $(notdir $<)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(dir $@))" $(MKDIR) "$(subst /,\,$(dir $@))"
else
	@$(MKDIR) $(dir $@)
endif
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BUILD_DIR))" $(RMDIR) "$(subst /,\,$(BUILD_DIR))"
else
	@$(RMDIR) $(BUILD_DIR)
endif

rebuild: clean all

install: $(OUTPUT)
	@echo [INSTALL] Copying to mods folder
ifeq ($(OS),Windows_NT)
	@if not exist "..\..\build\bin\mods" $(MKDIR) "..\..\build\bin\mods"
	@copy /Y "$(subst /,\,$(OUTPUT))" "..\..\build\bin\mods\$(MOD_NAME)$(DLL_EXT)"
else
	@mkdir -p ../../build/bin/mods
	@cp $(OUTPUT) ../../build/bin/mods/$(MOD_NAME)$(DLL_EXT)
endif

.PHONY: all clean rebuild install