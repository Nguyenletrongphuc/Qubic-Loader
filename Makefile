MAKEFLAGS += --no-print-directory

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    .SHELLFLAGS := /c
    RM := del /q
    RMDIR := rd /s /q
    MKDIR := mkdir
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
    DETECTED_OS := Unix
endif

BUILD_DIR := build
CLASSPATH_FILE := $(BUILD_DIR)/bin/logs/classpath.txt

.PHONY: all clean qubic-loader qubic-agent qubic-api qubic-core test-mod generate-classpath

all: qubic-loader generate-classpath qubic-agent qubic-api qubic-core test-mod

qubic-loader:
	@echo [MAKE] qubic-loader
	@$(MAKE) -C qubic-loader || echo [SKIP] qubic-loader

generate-classpath: qubic-loader
	@echo [CLASSPATH] Generating classpath file
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(CLASSPATH_FILE))" ( \
		echo [CLASSPATH] Running loader to generate classpath... && \
		cd "$(BUILD_DIR)\bin" && qubic.exe --generate-classpath-only \
	) else ( \
		echo [CLASSPATH] Classpath file already exists \
	)
else
	@if [ ! -f "$(CLASSPATH_FILE)" ]; then \
		echo "[CLASSPATH] Running loader to generate classpath..."; \
		cd $(BUILD_DIR)/bin && ./qubic --generate-classpath-only; \
	else \
		echo "[CLASSPATH] Classpath file already exists"; \
	fi
endif

# qubic-agent depends on classpath being generated
qubic-agent: generate-classpath
	@echo [MAKE] qubic-agent
	@$(MAKE) -C qubic-agent || echo [SKIP] qubic-agent

qubic-api:
	@echo [MAKE] qubic-api
	@$(MAKE) -C qubic-api || echo [SKIP] qubic-api

qubic-core: qubic-api
	@echo [MAKE] qubic-core
	@$(MAKE) -C qubic-core || echo [SKIP] qubic-core

test-mod: qubic-api
	@echo [MAKE] test-mod
	@$(MAKE) -C tests/test-mod || echo [SKIP] test-mod

clean:
	@echo [CLEAN] Cleaning build directory
ifeq ($(OS),Windows_NT)
	@if exist "$(BUILD_DIR)" $(RMDIR) "$(BUILD_DIR)" 2>nul
else
	@$(RMDIR) $(BUILD_DIR)
endif
	@echo [CLEAN] Complete

rebuild: clean all

.PHONY: loader agent api core mod
loader: qubic-loader
agent: qubic-loader generate-classpath qubic-agent
api: qubic-api
core: qubic-api qubic-core
mod: qubic-api test-mod