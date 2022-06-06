NAME := app

#dir
WORK_DIR := $(shell pwd)
BUILD_DIR := $(WORK_DIR)/build

OBJ_DIR  = $(BUILD_DIR)/obj-$(NAME)
BINARY   = $(BUILD_DIR)/$(NAME)

CLEAN_TARGET := $(BUILD_DIR)

#compiler
CC := gcc
ifeq ($(CC), clang)
CXX := clang++
else
CXX := g++
endif
LD := $(CXX)

CFLAGS_BUILD += -flto
CFLAGS_BUILD += -Og -ggdb3
CFLAGS_BUILD += -fsanitize=address
CFLAGS  := -O2 -MMD -Wall -Werror $(CFLAGS) $(CFLAGS_BUILD)
LDFLAGS := -O2 $(LDFLAGS) $(CFLAGS_BUILD) -lpcre

SRCS = $(shell find -name "*.cc")
OBJS = $(SRCS:%.cc=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.cc
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

# Depencies
-include $(OBJS:%.o=%.d)


.PHONY: run all clean

all: $(BINARY)

$(BINARY): main.o lexer.o
	@echo + LD $@
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo clean $(CLEAN_TARGET)
	-@rm -rf $(CLEAN_TARGET)/*

run:$(BINARY)
	@echo run $(BINARY)
	@$(BINARY)
