MAKEFLAGS += -j --no-print-directory

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    .SHELLFLAGS := /c
    RM := del /q
    RMDIR := rd /s /q
    MKDIR := mkdir
    PATH_SEP := \\
    DETECTED_OS := Windows
    LIB_EXT := .lib
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
        LIB_EXT := .a
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
        LIB_EXT := .a
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    PATH_SEP := /
endif

PROJECT_NAME := qubic-api
SRC_DIR := ./src
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj-lib
BIN_DIR := $(BUILD_DIR)/bin
OUTPUT := $(BIN_DIR)/$(PROJECT_NAME)$(LIB_EXT)

CC := clang
CXX := clang++
AR := llvm-ar

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
else
    CXXFLAGS += -fPIC
    CFLAGS += -fPIC
endif

RWILDCARD = $(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$d,$2) $(filter $(subst *,%,$2),$d))

C_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.c)
CPP_SOURCES := $(call RWILDCARD,$(SRC_DIR),*.cpp)

C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	@echo [AR] $(notdir $@)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(BIN_DIR))" $(MKDIR) "$(subst /,\,$(BIN_DIR))"
else
	@$(MKDIR) $(BIN_DIR)
endif
	@$(AR) rcs $@ $^

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
	@if exist "$(subst /,\,$(OBJ_DIR))" $(RMDIR) "$(subst /,\,$(OBJ_DIR))"
	@if exist "$(subst /,\,$(OUTPUT))" $(RM) "$(subst /,\,$(OUTPUT))"
else
	@$(RMDIR) $(OBJ_DIR)
	@$(RM) $(OUTPUT)
endif

rebuild: clean all
.PHONY: all clean rebuild