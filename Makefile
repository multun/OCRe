ARGS?=bitmap/tests/test_art.bmp

CC?=gcc

LOG_LEVEL?=HIGH

CFLAGS=-DDEBUG=$(LOG_LEVEL) -std=c99 -Wconversion -pedantic -Wall -Wextra -Wformat=2 -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=4 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Werror -ggdb3 -O0 -fno-omit-frame-pointer -fstrict-aliasing

FLAGS_BUILD=-O3 -Wall -pedantic -std=c99 -Wextra -fomit-frame-pointer

# Ignore warnings from gtk headers
CFLAGS += $(subst -I,-isystem,$(shell pkg-config --cflags gtk+-3.0))
LDFLAGS = $(shell pkg-config --libs gtk+-3.0) -lm

EXEC=ocre

SRC  = $(wildcard *.c)
SRC += $(wildcard base_structs/*.c)
SRC += $(wildcard block_to_line/*.c)
SRC += $(wildcard binarisation/*.c)
SRC += $(wildcard bitmap/*.c)
SRC += $(wildcard gtk/*.c)
SRC += $(wildcard boxing/*.c)
SRC += $(wildcard neural_network/*.c)

OBJ = $(SRC:.c=.o)
DEP = ${SRC:.c=.d}


print-%:
	@echo '$*=$($*)'

all: $(EXEC)

build: CFLAGS=$(CFLAGS_BUILD)
build: clean all

shared: CFLAGS  += -fPIC
shared: LDFLAGS += -shared
shared: clean all

$(EXEC): $(OBJ)
	@echo "Linking the main binary file..."
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "Building $@..."
	@$(CC) -MMD -o $@ -c $< $(CFLAGS)

-include ${DEP}

.PHONY: clean valgrind gtkdbg
clean:
	rm -f $(EXEC) $(DEP) $(OBJ)
gdbrun: all
	gdb --args ./$(EXEC) $(ARGS)

run: all
	./$(EXEC) $(ARGS)
suppr:
	test -d GNOME.supp || git clone https://github.com/dtrebbien/GNOME.supp.git
	(cd GNOME.supp && make)
	test -f gtk.suppression || curl -L -O http://www.gnome.org/~johan/gtk.suppression

SUPPRS = $(addprefix --suppressions=, gtk-cairo-custom.supp gtk.suppression $(shell find GNOME.supp -type f -name '*.supp'))

gtkdbg:
	G_DEBUG=all GTK_DEBUG=interactive make gdbrun

valgrind: $(EXEC)
	G_DEBUG=resident-modules G_SLICE=always-malloc valgrind --gen-suppressions=all $(SUPPRS) --dsymutil=yes --tool=memcheck  --leak-check=full --track-origins=yes ./$(EXEC) $(ARGS)
