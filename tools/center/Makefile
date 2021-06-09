# Copyright (c) 2021 mataha
# 
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
# 
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

PROGRAM     = center
DESCRIPTION = Centers an executable's main window on the current display

SRC_DIR = src

COMPILER       = gcc
COMPILER_FLAGS = -pedantic --std=c17 -Werror -O2 -finline-functions
LINKER_FLAGS   = -mconsole -s -lkernel32

# ------------------------------------------------------------------------------

# Do not use make's built-in rules and variables
MAKEFLAGS += --no-builtin-rules --no-builtin-variables

# Encapsulate contents of a given list in double quotes
override enquote = $(foreach element,$(1),"$(element)")


ROOT ?= .
MAIN ?= main

BIN_DIR ?= bin
OBJ_DIR ?= obj
DEP_DIR ?= dep

BIN_PATH := $(ROOT)/$(BIN_DIR)
SRC_PATH := $(ROOT)/$(SRC_DIR)
OBJ_PATH := $(ROOT)/$(OBJ_DIR)
DEP_PATH := $(ROOT)/$(DEP_DIR)

SRCS := $(foreach path,$(SRC_PATH),$(wildcard $(addprefix $(path)/*,.c*)))
OBJS := $(addprefix $(OBJ_PATH)/,$(addsuffix .o,$(notdir $(basename $(SRCS)))))
DEPS := $(OBJS:$(OBJ_PATH)/%.o=$(DEP_PATH)/%.d)

DIR_LIST := $(BIN_PATH) $(OBJ_PATH) $(DEP_PATH)
INC_LIST := $(sort $(dir $(wildcard $(SRC_PATH)/*/))) $(ROOT)/

CC       := $(COMPILER)
CFLAGS   := -Wall -Wextra -Wundef -Wno-unused-parameter $(COMPILER_FLAGS)
CPPFLAGS := $(addprefix -I,$(call enquote,$(INC_LIST:/=))) -MMD -MP
LDFLAGS  := $(LINKER_FLAGS)
MKDIR    := mkdir -p
RM       := rm -rf

ifeq ($(OS),Windows_NT)
    ifeq ($(suffix $(PROGRAM)),)
        PROGRAM := $(addsuffix .exe,$(PROGRAM))
    endif

    ifneq ($(CHARSET),ANSI)
        CFLAGS  += -DUNICODE -D_UNICODE
        LDFLAGS += -municode

        $(OBJ_PATH)/$(MAIN).o: CFLAGS += -Wno-missing-prototypes
    endif

    CFLAGS += -DWIN32_LEAN_AND_MEAN
endif

ifdef DEBUG
    CFLAGS += -DDEBUG_BUILD
endif


default: mkdir all

$(BIN_PATH)/$(PROGRAM): $(OBJS)
	$(CC) -o "$@" $(call enquote,$^) $(LDFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CFLAGS) -o "$@" -c "$<" $(CPPFLAGS) -MF "$(DEP_PATH)/$*.d"

PHONY := all
all: $(BIN_PATH)/$(PROGRAM)

PHONY += mkdir
mkdir:
	@$(MKDIR) $(call enquote,$(DIR_LIST))

PHONY += clean
clean:
	@$(RM)    $(call enquote,$(DIR_LIST))

.PHONY: $(PHONY)

# Include the .d makefiles if they exist (and suppress the errors)
-include $(DEPS)
