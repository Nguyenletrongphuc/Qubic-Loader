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

PROJECT_NAME := qubic-core
SRC_DIR := ./src
BUILD_DIR := ../build
OBJ_DIR := $(BUILD_DIR)/obj-dll
BIN_DIR := $(BUILD_DIR)/bin
OUTPUT := $(BIN_DIR)/$(PROJECT_NAME)$(DLL_EXT)

CC := clang
CXX := clang++

COMMON_FLAGS := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wunused -Wformat=2 -Wno-language-extension-token -Wno-unused-parameter -Wno-unused-private-field

JAVA_HOME := C:\Program Files\Java\jdk-21

ifeq ($(DETECTED_OS),Windows)
    INC_DIRS := src include ./ ../
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/win32"
else ifeq ($(DETECTED_OS),Linux)
    INC_DIRS := src include ./ ../
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/linux"
else ifeq ($(DETECTED_OS),macOS)
    INC_DIRS := src include ./ ../
    JAVA_INC := -I"$(JAVA_HOME)/include" -I"$(JAVA_HOME)/include/darwin"
endif

INC_DIRS += ../include

ifeq ($(DETECTED_OS),Windows)
    CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -g -m64 $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -g -m64 $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
else
    CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -g -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
    CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -g -fPIC $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
endif

ifeq ($(DETECTED_OS),Windows)
    LDFLAGS := -m64 -shared -fuse-ld=lld-link "$(JAVA_HOME)/lib/jvm.lib"
else ifeq ($(DETECTED_OS),Linux)
    LDFLAGS := -shared -L"$(JAVA_HOME)/lib/server" -ljvm -lpthread -ldl -lm
else ifeq ($(DETECTED_OS),macOS)
    LDFLAGS := -shared -L"$(JAVA_HOME)/lib/server" -ljvm -lpthread -ldl
endif

RWILDCARD = $(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$d,$2) $(filter $(subst *,%,$2),$d))

C_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.c)
CPP_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.cpp)

C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)

all: $(OUTPUT)
	@$(MAKE) cleanup

$(OUTPUT): $(OBJECTS)
	@echo [LD] $(notdir $@)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(BIN_DIR))" $(MKDIR) "$(subst /,\,$(BIN_DIR))"
else
	@$(MKDIR) $(BIN_DIR)
endif
	@$(CXX) -o $@ $^ $(LDFLAGS)

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

cleanup:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BIN_DIR))\$(PROJECT_NAME).lib" $(RM) "$(subst /,\,$(BIN_DIR))\$(PROJECT_NAME).lib" >nul 2>&1
endif

-include $(OBJECTS:.o=.d)

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(OBJ_DIR))" $(RMDIR) "$(subst /,\,$(OBJ_DIR))"
	@if exist "$(subst /,\,$(OUTPUT))" $(RM) "$(subst /,\,$(OUTPUT))"
	@if exist "$(subst /,\,$(BIN_DIR))\$(PROJECT_NAME).lib" $(RM) "$(subst /,\,$(BIN_DIR))\$(PROJECT_NAME).lib"
else
	@$(RMDIR) $(OBJ_DIR)
	@$(RM) $(OUTPUT)
endif

rebuild: clean all
.PHONY: all clean rebuild cleanup