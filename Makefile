NAME := app

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD) -lpcre

.PHONY: run pre

ARGS := $(WORK_DIR)/resource/test.c -l $(BUILD_DIR)/log.txt --syntax=$(BUILD_DIR)/syntax.txt
run:$(BINARY)
	@echo run $(BINARY)
	@$(BINARY) $(ARGS)

# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run