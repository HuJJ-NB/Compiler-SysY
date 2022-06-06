NAME := app

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD) -lpcre

.PHONY: run pre

SOURCE_FILE := $(WORK_DIR)/resource/test.c
LOG_FILE := $(BUILD_DIR)/log.txt
HIGHLIGHT_FILE := $(BUILD_DIR)/highlight.txt

ARGS := $(SOURCE_FILE) -l $(LOG_FILE)

run:$(BINARY)
	@echo run $(BINARY)
	@$(BINARY) $(ARGS)

# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run