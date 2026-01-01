CXX = clang++
CC = clang
AR = ar

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj-lib
LIB_DIR = $(BUILD_DIR)/lib
MINIZ_DIR = include/miniz

LIB_NAME = qubic-api$(LIB_EXT)
LIB_PATH = $(LIB_DIR)/$(LIB_NAME)

JAVA_HOME ?= C:/Program Files/Java/jdk-23
JNI_INCLUDES = -I"$(JAVA_HOME)/include"

CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -I. -I./ -I../ -I$(INC_DIR) -Iinclude $(JNI_INCLUDES)
CFLAGS = -Wall -Wextra -Wno-unused-function -I. -I./ -I../ -I$(INC_DIR) -Iinclude

ifeq ($(OS),Windows_NT)
    DETECTED_OS = Windows
    JNI_INCLUDES += -I"$(JAVA_HOME)/include/win32"
    CXXFLAGS += -D_WIN32
    CFLAGS += -D_WIN32
    LIB_EXT = .lib
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS = Linux
        JNI_INCLUDES += -I"$(JAVA_HOME)/include/linux"
        LIB_EXT = .a
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS = macOS
        JNI_INCLUDES += -I"$(JAVA_HOME)/include/darwin"
        LIB_EXT = .a
    endif
endif

ifdef RELEASE
    CXXFLAGS += -O3 -DNDEBUG
    CFLAGS += -O3 -DNDEBUG
else
    CXXFLAGS += -g -O0
    CFLAGS += -g -O0
endif

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
    RMDIR = rmdir /S /Q
    FIXPATH = $(subst /,\,$1)
    SHELL = cmd
else
    RM = rm -f
    RMDIR = rm -rf
    FIXPATH = $1
endif

CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/**/*.cpp)

CPP_OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))

ALL_OBJECTS = $(CPP_OBJECTS)

.PHONY: all clean rebuild

all: $(LIB_PATH)

$(LIB_PATH): $(ALL_OBJECTS)
	@echo [AR] $(LIB_NAME)
ifeq ($(OS),Windows_NT)
	@if not exist $(call FIXPATH,$(LIB_DIR)) mkdir $(call FIXPATH,$(LIB_DIR))
else
	@mkdir -p $(LIB_DIR)
endif
	@$(AR) rcs $(LIB_DIR)/qubic-api.a $^
ifeq ($(OS),Windows_NT)
	@move $(call FIXPATH,$(LIB_DIR)/qubic-api.a) $(call FIXPATH,$(LIB_PATH)) >nul
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [CXX] $<
ifeq ($(OS),Windows_NT)
	@if not exist $(call FIXPATH,$(dir $@)) mkdir $(call FIXPATH,$(dir $@))
else
	@mkdir -p $(dir $@)
endif
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo Cleaning build artifacts
	@$(RMDIR) $(call FIXPATH,$(BUILD_DIR)) 2>nul || echo Clean complete

rebuild: clean all