Q ?= @

LLVM_CONFIG := llvm-config

LLVM_CXXFLAGS  := $(subst -I,-isystem,$(shell $(LLVM_CONFIG) --cppflags))
LLVM_LDFLAGS := $(shell $(LLVM_CONFIG) --ldflags --system-libs) -pthread
CXXFLAGS  += -Wall -W -std=c++14 -O0 -g -Wno-unused-parameter $(LLVM_CXXFLAGS)

ifeq ($(LLVM_SHARED), yes)
	LLVM_LIBS := -lLLVM-$(shell $(LLVM_CONFIG) --version)
else
	LLVM_LIBS := $(shell $(LLVM_CONFIG) --libs core)
endif

LDFLAGS  += $(LLVM_LDFLAGS) $(LLVM_LIBS)

.PHONY: all clean archive

all: custom

custom: custom.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

fac: fac.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

struct: struct.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

main: main.cpp
	@echo "===> CXX $<"
	$(Q)$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@echo "===> CLEAN"
	$(Q)rm -f fac max main struct ir-gen-examples.tgz

ir-gen-examples.tgz: Makefile max.cpp fac.cpp struct.cpp main.cpp
	@echo "===> ARCHIVE"
	$(Q)tar -czvf $@ $^

archive: ir-gen-examples.tgz
