CC=gcc

CFLAGS=-std=c99 -Wno-conversion -pedantic -Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls -Wold-style-definition -Werror -ggdb3 -O0 -fno-omit-frame-pointer -ffloat-store -fno-common -fstrict-aliasing -lm

CFLAGS_BUILD=-O3 -Wall -pedantic -std=c99 -Wextra

LDFLAGS= #-lpthread -ldl
EXEC=ocre

SRC= $(wildcard *.c)
SRC+= $(wildcard binarisation/*.c)
SRC+= $(wildcard libbitmap/*.c)

OBJ= $(SRC:.c=.o)

all: $(EXEC)

build: CFLAGS=$(CFLAGS_BUILD)
build: clean all

$(EXEC): $(OBJ)
	@echo "Building the main binary file..."
	@$(CC) -o $@ $^ $(LDFLAGS)

# règle générique pour tous les dépendances en .o, avec un pipe pour les arguments de comp.
%.o: %.c
	@echo "Building $@..."
	@$(CC) -o $@ -c $< $(CFLAGS)
#.phony reconstruit toujours ses dépendances
.PHONY: clean mrproper
#vire tous les fichiers intermédiaires
clean:
	find . -type f -name '*.o' -delete
mrproper: clean
	rm -rf $(EXEC)
