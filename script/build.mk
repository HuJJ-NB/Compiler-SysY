#dir
WORK_DIR := $(shell pwd)
BUILD_DIR := $(WORK_DIR)/build

INC_PATH := $(WORK_DIR)/include $(INC_PATH)
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

INCLUDES = $(addprefix -I, $(INC_PATH))
CFLAGS  := -O2 -MMD -Wall -Werror $(INCLUDES) $(CFLAGS)
LDFLAGS := -O2 $(LDFLAGS)

SRC_PATH = src
CXXSRC = $(shell find $(SRC_PATH) -name "*.cc")
SRCS = $(shell find $(SRC_PATH) -name "*.c")
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o) $(CXXSRC:%.cc=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.c
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cc
	@echo + CXX $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

# Depencies
-include $(OBJS:%.o=%.d)

#target
.PHONY: all clean

app: $(BINARY)

$(BINARY): $(OBJS)
	@echo + LD $@
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo clean $(CLEAN_TARGET)
	-@rm -rf $(CLEAN_TARGET)/*
