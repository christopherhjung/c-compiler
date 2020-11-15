BUILDDIR ?= build
CFG      ?= debug
NAME     ?= c4
BUILD_NAME     ?= build
SRCDIR   ?= src
RESDIR   ?= resources
GENERATED   ?= generated

all:

-include config/$(CFG).cfg

# Be verbose about the build.
Q ?= @

BINDIR := $(BUILDDIR)/$(CFG)
GENERATED_DIR := $(SRCDIR)/$(GENERATED)
BIN    := $(BINDIR)/$(NAME)
BUILD_BIN    := $(BINDIR)/$(BUILD_NAME)
RES_BUILD    := $(BINDIR)/$(RESDIR)
SRC    := $(sort $(wildcard $(SRCDIR)/**/*.cpp))
MAIN_SRC := $(SRCDIR)/main.cpp
BUILD_SRC := $(SRCDIR)/build.cpp
OBJ    := $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
MAIN_OBJ    := $(MAIN_SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
BUILD_OBJ    := $(BUILD_SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
DEP    := $(OBJ:%.o=%.d)



# Try to locate llvm-config, a tool that produces command line flags for the
# build process.
ifneq ("$(wildcard llvm/install/bin/llvm-config)","")
	LLVM_CONFIG  ?= llvm/install/bin/llvm-config
else
	LLVM_CONFIG  ?= llvm-config
endif

# Obtain the LLVM build flags if we found llvm-config.
ifeq (,$(wildcard $(LLVM_CONFIG)))
	LLVM_CFLAGS  :=
	LLVM_LDFLAGS :=
else
	LLVM_CFLAGS  := $(shell $(LLVM_CONFIG) --cppflags)
	LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags --libs --system-libs)
endif

CFLAGS   := $(LLVM_CFLAGS) -Wall -W $(CFLAGS)
CXXFLAGS += $(CFLAGS) -std=c++17
LDFLAGS  += $(LLVM_LDFLAGS)

DUMMY := $(shell mkdir -p $(sort $(dir $(OBJ))))

.PHONY: all clean develop

all: build main

build: $(BUILD_BIN)

main: before $(BIN)

before:
	@echo "===> generate lexer"
	$(Q)mkdir -p $(GENERATED_DIR)
	$(Q)$(BUILD_BIN) "./resources/c.lexer" "./src/generated/GeneratedLexer.h"

after:
	$(Q)rm -fr $(GENERATED_DIR)

-include $(DEP)

clean:
	@echo "===> CLEAN"
	$(Q)rm -fr $(BINDIR)
	$(Q)rm -fr $(GENERATED_DIR)

$(BIN): $(OBJ)  $(MAIN_OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BIN) $(OBJ) $(MAIN_OBJ)  $(LDFLAGS)

$(BUILD_BIN): $(OBJ) $(BUILD_OBJ)
	@echo "===> LD $@"
	$(Q)$(CXX) -o $(BUILD_BIN) $(OBJ) $(BUILD_OBJ) $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c -o $@ $<

$(RES_BUILD): $(RESDIR)
	@echo "===> CP $< to $@"
	$(Q)cp -r $< $(BINDIR)

test: all
	./build/debug/c4 --tokenize test/test2.c

