BIN		:= coolgame
LIBS	:= $(shell pkg-config --libs glfw3 gl) -ldl -lz -lm 
BINDIR	:= .
SRCDIR	:= src
CC		:= gcc
CFLAGS	:= -MMD -std=c99 -Wall -g -Og
LDFLAGS	:= $(LIBS)

vertsrc := $(shell find $(SRCDIR)/shaders -name "*.vert")
vertobj := $(vertsrc:.vert=_v.o)
fragsrc := $(shell find $(SRCDIR)/shaders -name "*.frag")
fragobj := $(fragsrc:.frag=_f.o)

src	:= $(shell find $(SRCDIR) -name "*.c")
obj	:= $(src:.c=.o)
dep	:= $(obj:.o=.d)

obj += $(vertobj)
obj += $(fragobj)

%_v.o: %.vert
	objcopy --readonly-text --input-target binary --output-target elf64-x86-64 $< $@
%_f.o: %.frag
	objcopy --readonly-text --input-target binary --output-target elf64-x86-64 $< $@

$(BINDIR)/$(BIN): $(obj)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

-include $(dep)

build: $(BINDIR)/$(BIN)

clean:
	rm -f $(BINDIR)/$(BIN)
	rm -f $(obj) $(dep)

test: build
	./$(BINDIR)/$(BIN)

rebuild: clean build

.PHONY: build clean test rebuild
