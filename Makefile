NAME := app
ARGS := 

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD)

.PHONY: run

run:$(BINARY)
	@echo run $(BINARY)
	@$(BINARY) $(ARGS)

# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run