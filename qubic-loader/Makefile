MAKEFLAGS += -j --no-print-directory

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    .SHELLFLAGS := /c
    RM := del /q
    RMDIR := rd /s /q
    MKDIR := mkdir
    PATH_SEP := \\
    DETECTED_OS := Windows
    EXE_EXT := .exe
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
        EXE_EXT :=
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        EXE_EXT :=
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    PATH_SEP := /
endif

PROJECT_NAME := qubic
SRC_DIR := ./src
BUILD_DIR := ../build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
OUTPUT := $(BIN_DIR)/$(PROJECT_NAME)$(EXE_EXT)

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

CXXFLAGS := -std=c++17 $(COMMON_FLAGS) -O2 -g $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)
CFLAGS := -std=c11 $(COMMON_FLAGS) -O2 -g $(addprefix -I,$(INC_DIRS)) $(JAVA_INC)

ifeq ($(DETECTED_OS),Windows)
    CXXFLAGS += -m64
    CFLAGS += -m64
    LDFLAGS := -m64 -fuse-ld=lld-link "$(JAVA_HOME)/lib/jvm.lib"
else ifeq ($(DETECTED_OS),Linux)
    LDFLAGS := -L"$(JAVA_HOME)/lib/server" -ljvm -lpthread -ldl -lm
else ifeq ($(DETECTED_OS),macOS)
    LDFLAGS := -L"$(JAVA_HOME)/lib/server" -ljvm -lpthread -ldl
endif

RWILDCARD = $(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$d,$2) $(filter $(subst *,%,$2),$d))

C_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.c)
CPP_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.cpp)

C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)

all: $(OUTPUT)
	@$(MAKE) copy-dlls

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

copy-dlls:
ifeq ($(OS),Windows_NT)
	@echo [COPY] JVM DLLs
	@copy /y "$(subst /,\,$(JAVA_HOME))\bin\server\jvm.dll" "$(subst /,\,$(BIN_DIR))" >nul 2>&1
	@copy /y "$(subst /,\,$(JAVA_HOME))\bin\java.dll" "$(subst /,\,$(BIN_DIR))" >nul 2>&1
else
	@echo [COPY] JVM libraries
	@cp "$(JAVA_HOME)/lib/server/libjvm.so" "$(BIN_DIR)/" 2>/dev/null || true
endif

-include $(OBJECTS:.o=.d)

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BUILD_DIR))" $(RMDIR) "$(subst /,\,$(BUILD_DIR))"
else
	@$(RMDIR) $(BUILD_DIR)
endif

rebuild: clean all
.PHONY: all clean rebuild copy-dlls