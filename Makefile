NAME := app

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD) -lpcre

.PHONY: run pre

LOG_FILE := $(BUILD_DIR)/log.txt
SYNTAX_FILE := $(BUILD_DIR)/syntax.txt
ARGS := $(WORK_DIR)/resource/test.c -l $(LOG_FILE) --syntax=$(SYNTAX_FILE)
run:$(BINARY)
	@echo run $(BINARY)
	@$(BINARY) $(ARGS)
	@cat $(SYNTAX_FILE)

# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run