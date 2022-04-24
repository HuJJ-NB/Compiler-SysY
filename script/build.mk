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

#dir
WORK_DIR := $(shell pwd)
BUILD_DIR := $(WORK_DIR)/build

INC_PATH := $(WORK_DIR)/include $(INC_PATH)
OBJ_DIR  = $(BUILD_DIR)/obj-$(NAME)
BINARY   = $(BUILD_DIR)/$(NAME)

CLEAN_TARGET := $(BUILD_DIR)

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o) $(CXXSRC:%.cc=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: %.c
	@echo + CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.cc
	@echo + CXX $<
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<

#target
app: $(BINARY)

$(BINARY): $(OBJS)
	@echo + LD $<
	@$(LD) $(LDFLAGS) -o $@ $^

clean:
	@echo clean $(CLEAN_TARGET)
	-@rm -rf $(CLEAN_TARGET)
