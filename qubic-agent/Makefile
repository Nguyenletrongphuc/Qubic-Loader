MAKEFLAGS += --no-print-directory

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    .SHELLFLAGS := /c
    RM := del /q
    RMDIR := rd /s /q
    MKDIR := mkdir
    PATH_SEP := \\
    DETECTED_OS := Windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS := Linux
    endif
    ifeq ($(UNAME_S),Darwin)
        DETECTED_OS := macOS
    endif
    RM := rm -f
    RMDIR := rm -rf
    MKDIR := mkdir -p
    PATH_SEP := /
endif

PROJECT_NAME := qubic-agent
SRC_DIR := src
JAR_DIR := jar
BUILD_DIR := ../build
CLASS_DIR := $(BUILD_DIR)/classes
BIN_DIR := $(BUILD_DIR)/bin/java
OUTPUT := $(BIN_DIR)/$(PROJECT_NAME).jar
MANIFEST := MANIFEST.MF

JAVAC := javac
JAR := jar

ASM_JAR := $(JAR_DIR)/asm-9.6.jar
CLASSPATH_FILE := ../build/bin/logs/classpath.txt

JAVA_FILES := $(wildcard $(SRC_DIR)/*.java)

# read the classpath at makefile parse time
ifeq ($(OS),Windows_NT)
    MC_CLASSPATH := $(shell type $(subst /,\,$(CLASSPATH_FILE)) 2>nul)
else
    MC_CLASSPATH := $(shell cat $(CLASSPATH_FILE) 2>/dev/null)
endif

all: check-classpath $(OUTPUT)

check-classpath:
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(CLASSPATH_FILE))" ( \
		echo [ERROR] Classpath file not found: $(CLASSPATH_FILE) && \
		echo [INFO] Please run 'qubic.exe --generate-classpath-only' first && \
		exit 1 \
	)
else
	@if [ ! -f "$(CLASSPATH_FILE)" ]; then \
		echo "[ERROR] Classpath file not found: $(CLASSPATH_FILE)"; \
		echo "[INFO] Please run './qubic --generate-classpath-only' first"; \
		exit 1; \
	fi
endif

$(OUTPUT): $(MANIFEST) $(JAVA_FILES)
	@echo [JAVAC] Compiling all Java sources
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(CLASS_DIR))" $(MKDIR) "$(subst /,\,$(CLASS_DIR))"
	@$(JAVAC) -cp "$(subst /,\,$(ASM_JAR));$(MC_CLASSPATH)" -d "$(subst /,\,$(CLASS_DIR))" $(subst /,\,$(JAVA_FILES))
else
	@$(MKDIR) $(CLASS_DIR)
	@$(JAVAC) -cp "$(ASM_JAR):$(MC_CLASSPATH)" -d $(CLASS_DIR) $(JAVA_FILES)
endif
	@echo [JAR] $(notdir $@)
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(BIN_DIR))" $(MKDIR) "$(subst /,\,$(BIN_DIR))"
	@cd "$(subst /,\,$(CLASS_DIR))" && $(JAR) cvfm "$(subst /,\,$(abspath $(OUTPUT)))" "$(subst /,\,$(abspath $(MANIFEST)))" *.class >nul
	@copy /y "$(subst /,\,$(ASM_JAR))" "$(subst /,\,$(BIN_DIR))" >nul
else
	@$(MKDIR) $(BIN_DIR)
	@cd $(CLASS_DIR) && $(JAR) cvfm $(abspath $(OUTPUT)) $(abspath $(MANIFEST)) *.class
	@cp $(ASM_JAR) $(BIN_DIR)/
endif

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(CLASS_DIR))" $(RMDIR) "$(subst /,\,$(CLASS_DIR))"
	@if exist "$(subst /,\,$(OUTPUT))" $(RM) "$(subst /,\,$(OUTPUT))"
else
	@$(RMDIR) $(CLASS_DIR)
	@$(RM) $(OUTPUT)
endif

rebuild: clean all
.PHONY: all clean rebuild check-classpath