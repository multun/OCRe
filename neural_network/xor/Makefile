CC?=gcc

CFLAGS =-std=c99 -Wconversion -pedantic -Wall -Wextra -Wformat=2 \
	-Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast \
	-Wstrict-overflow=4 -Wstrict-prototypes -Winline -Wundef \
	-Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
	-Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition \
	-Werror -ggdb3 -O0 -fno-omit-frame-pointer -fstrict-aliasing

FLAGS_BUILD=-O3 -Wall -pedantic -std=c99 -Wextra -fomit-frame-pointer

LDFLAGS = -lm

EXEC=xor

SRC  = main.c
SRC += $(wildcard ../*.c)

OBJ = $(SRC:.c=.o)
DEP = ${SRC:.c=.d}

all: $(EXEC)

build: CFLAGS=$(CFLAGS_BUILD)
build: clean all

$(EXEC): $(OBJ)
	@echo "Linking the main binary file..."
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@echo "Building $@..."
	@$(CC) -MMD -o $@ -c $< $(CFLAGS)

-include ${DEP}

.PHONY: clean

clean:
	rm -f $(EXEC) $(DEP) $(OBJ)
run: all
	@mkdir -p xor_data
	./$(EXEC) $(ARGS)
