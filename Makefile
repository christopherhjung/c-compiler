BUILDDIR ?= build
CFG      ?= release
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
MAIN_SRC := $(SRCDIR)/lexerCore.h $(SRCDIR)/parserCore.h $(SRCDIR)/main.cpp
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

.PHONY: all clean

all:
	mkdir -p $(BINDIR)
	mkdir -p $(GENERATED_DIR)

	cmake -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) -B$(BINDIR) .
	make -C $(BINDIR) build
	$(BINDIR)/build "./resources/c.lexer" "./src/generated/GeneratedLexer.h"
	make -C $(BINDIR) c4


clean:
	@echo "===> CLEAN"
	$(Q)rm -fr $(BINDIR)
	$(Q)rm -fr $(GENERATED_DIR)


