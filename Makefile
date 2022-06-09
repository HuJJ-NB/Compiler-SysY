NAME := app

-include script/build.mk

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  += $(CFLAGS_BUILD)
LDFLAGS += $(CFLAGS_BUILD) -lpcre

.PHONY: run pre

INPUT_PATH := $(WORK_DIR)/resource
INPUT_FILES := $(shell find $(INPUT_PATH)/*)
OUTPUT_PATH := $(BUILD_DIR)/output

ARG := `echo "$(patsubst $(INPUT_PATH)/%,%,$(INPUT_FILES))" | sed 's/\([_0-9a-zA-Z]*\)\.\([_0-9a-zA-Z]*\)/$(subst /,\/,$(INPUT_PATH))\/\1.\2 -o $(subst /,\/,$(OUTPUT_PATH))\/\1.ast/g'`

run:$(BINARY)
	@mkdir -p $(OUTPUT_PATH)
	@$(BINARY) $(ARG)


# vs code clangd use compile_commands.json to work
pre:
	@echo + compile_commands.json
	@compiledb -n make run
