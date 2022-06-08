NAME := app

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD) -lpcre

.PHONY: run pre

RESOURCE_PATH := $(WORK_DIR)/resource
SOURCE_FILES := $(shell find $(RESOURCE_PATH) -name **.c)
LOG_FILE := $(BUILD_DIR)/log.txt
HIGHLIGHT_FILE := $(BUILD_DIR)/highlight.txt

ARGS := -l $(LOG_FILE)

run:$(BINARY)
	@for source in $(SOURCE_FILES); do \
	echo run $(BINARY) $${source}; \
	$(BINARY) $(ARGS) $${source}; \
	done

# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run
